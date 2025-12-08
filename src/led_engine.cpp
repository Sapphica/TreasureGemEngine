#include "led_engine.h"
#include <Arduino.h>
#include <math.h>

// ---------------- Original Defines ----------------
#define LED_PIN             4
#define NUM_LEDS            15
#define COLOR_ORDER         NEO_GRBW

#define MAX_BRIGHTNESS      255
#define BASE_BRIGHTNESS     0.30

// ★★★★★ 2× SPEED PATCHES ★★★★★
#define FLASH_BRIGHTNESS    1.0
#define FLASH_TIME_MS       500
#define MIN_INTERVAL_MS     750
#define MAX_INTERVAL_MS     2000
#define COLOR_SHIFT_TIME    45000
#define FLARE_INTERVAL_MS   3500
#define BREATH_PERIOD_MS    15000

// ------------------- CLUSTER LAYOUT (2-LED GROUPS) --------------------
static const uint8_t clusterStart[LedEngine::CLUSTER_COUNT] = { 
  0, 2, 4, 6, 8, 10, 12
};

static const uint8_t clusterSize[LedEngine::CLUSTER_COUNT] = { 
  2, 2, 2, 2, 2, 2, 3     // last cluster uses remaining LEDs
};

// ---------------------------------------------------------------------

#define TEST_MODE   1

#if TEST_MODE
  #define MULTICOLOR_RUN_MS   (15UL * 1000UL)
  #define WHITE_ONLY_RUN_MS   (3UL  * 1000UL)
#else
  #define MULTICOLOR_RUN_MS   (15UL * 60UL * 1000UL)
  #define WHITE_ONLY_RUN_MS   (3UL  * 60UL * 1000UL)
#endif

// -------------------------------------------------------
// ctor – just wires up the strip + default state
// -------------------------------------------------------
LedEngine::LedEngine()
    : strip(NUM_LEDS, LED_PIN, COLOR_ORDER + NEO_KHZ800),
      whiteMode(false),
      modeStart(0),
      lastFlare(0),
      lastReset(0)
{
}

// -------------------------------------------------------
// HSV → RGB  (same as original)
// -------------------------------------------------------
void LedEngine::hsvToRgb(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
  float c = v * s;
  float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
  float m = v - c;
  float r1, g1, b1;

  if      (h < 60)  { r1 = c; g1 = x; b1 = 0; }
  else if (h < 120) { r1 = x; g1 = c; b1 = 0; }
  else if (h < 180) { r1 = 0; g1 = c; b1 = x; }
  else if (h < 240) { r1 = 0; g1 = x; b1 = c; }
  else if (h < 300) { r1 = x; g1 = 0; b1 = c; }
  else              { r1 = c; g1 = 0; b1 = x; }

  r = (uint8_t)((r1 + m) * 255);
  g = (uint8_t)((g1 + m) * 255);
  b = (uint8_t)((b1 + m) * 255);
}

// -------------------------------------------------------
// Assign new palette (same logic, now a method)
// -------------------------------------------------------
void LedEngine::assignNewPalette() {
  unsigned long now = millis();
  for (int c = 0; c < CLUSTER_COUNT; c++) {
    clusters[c].flashing   = false;
    clusters[c].hue        = random(0, 360);
    clusters[c].hueDrift   = (random(0, 2) ? 1 : -1) * (random(2, 6) / 1000.0f);
    clusters[c].facetAngle = random(0, 360);
    clusters[c].flashMode  = 0;
    clusters[c].nextFlash  = now + random(MIN_INTERVAL_MS, MAX_INTERVAL_MS);
  }
}

void LedEngine::triggerClusterFlare() {
  unsigned long now = millis();
  int flareCount = random(1, 3);
  for (int n = 0; n < flareCount; n++) {
    int c = random(0, CLUSTER_COUNT);
    clusters[c].flashing   = true;
    clusters[c].startTime  = now;
    clusters[c].flashMode  = random(0, 3);
  }
}

// -------------------------------------------------------
// begin() = your original setup()
// -------------------------------------------------------
void LedEngine::begin() {
  strip.begin();
  strip.show();
  randomSeed(analogRead(0));
  assignNewPalette();
  modeStart = millis();
}

// -------------------------------------------------------
// update() = your original loop()
// -------------------------------------------------------
void LedEngine::update() {
  unsigned long now = millis();

  // MODE switching (unchanged)
  unsigned long elapsed = now - modeStart;
  if (!whiteMode && elapsed >= MULTICOLOR_RUN_MS) {
    whiteMode = true;
    modeStart = now;
  }
  else if (whiteMode && elapsed >= WHITE_ONLY_RUN_MS) {
    whiteMode = false;
    modeStart = now;
  }

  float breathBase = (2 * PI * now) / (float)BREATH_PERIOD_MS;

  // ★ 2× shimmer speed
  float facetBaseDeg = (float)now / 12.0f;

  long flareJitter = random(-1000, 1000);
  if (now - lastFlare > FLARE_INTERVAL_MS + flareJitter) {
    triggerClusterFlare();
    lastFlare = now;
  }

  // -------------------------------------------------------
  // PER-CLUSTER LOGIC  (identical, just using members)
// -------------------------------------------------------
  for (int c = 0; c < CLUSTER_COUNT; c++) {

    clusters[c].hue += clusters[c].hueDrift;
    if (clusters[c].hue < 0) clusters[c].hue += 360;
    if (clusters[c].hue >= 360) clusters[c].hue -= 360;

    if (!clusters[c].flashing && now >= clusters[c].nextFlash) {
      clusters[c].flashing  = true;
      clusters[c].startTime = now;
      clusters[c].flashMode = random(0, 3);
    }

    float phase = c * 0.8f;
    float globalMod = 1.0f + 0.15f * sin(breathBase + phase);

    float brightness = BASE_BRIGHTNESS * globalMod;
    float whiteBlend = 0.0f;

    if (clusters[c].flashing) {
      float t = (float)(now - clusters[c].startTime) / FLASH_TIME_MS;

      if (t >= 1.0f) {
        clusters[c].flashing  = false;
        clusters[c].nextFlash = now + random(MIN_INTERVAL_MS, MAX_INTERVAL_MS);
      } else {
        float wave;
        switch (clusters[c].flashMode) {
          case 0: wave = sin(t * PI); break;
          case 1: wave = (t < 0.5f) ? t * 2.0f : 2.0f * (1 - t); break;
          case 2: wave = pow(sin(t * PI), 3); break;
        }
        brightness = (BASE_BRIGHTNESS +
                     (FLASH_BRIGHTNESS - BASE_BRIGHTNESS) * wave)
                     * globalMod;
        whiteBlend = wave;
      }
    }

    float angleDeg = facetBaseDeg + clusters[c].facetAngle;
    float angleRad = angleDeg * (PI / 180.0f);
    float facetCos = cos(angleRad);
    if (facetCos < 0) facetCos = 0;
    brightness *= (0.8f + 0.2f * facetCos);

    int jitterCluster = random(-2, 3);
    brightness *= (1.0f + (jitterCluster * 0.01f));

    float brightnessRGB = pow(brightness, 0.85f);

    // -------------------------------------------------------
    // PER-LED WITHIN CLUSTER
    // -------------------------------------------------------
    uint8_t start = clusterStart[c];
    uint8_t count = clusterSize[c];

    for (int k = 0; k < count; k++) {
      int i = start + k;

      float ledBrightness = brightnessRGB;
      float ledWhiteBlend = whiteBlend;

      int jitterLed = random(-3, 4);
      ledBrightness *= (1.0f + (jitterLed * 0.01f));
      if (ledBrightness < 0) ledBrightness = 0;

      float hueForColor = clusters[c].hue;
      if (clusters[c].flashing) {
        float dir = (i % 2 == 0) ? 1.0f : -1.0f;
        hueForColor += ledWhiteBlend * 20.0f * dir;
        if (hueForColor < 0) hueForColor += 360;
        if (hueForColor >= 360) hueForColor -= 360;
      }

      uint8_t r, g, b, w = 0;
      hsvToRgb(hueForColor, 1.0f, 1.0f, r, g, b);

      float whiteSoft = 0.22f;

      float rF = r * ledBrightness + 255 * ledWhiteBlend * whiteSoft;
      float gF = g * ledBrightness + 255 * ledWhiteBlend * whiteSoft;
      float bF = b * ledBrightness + 255 * ledWhiteBlend * whiteSoft;

      rF = constrain(rF, 0, 255);
      gF = constrain(gF, 0, 255);
      bF = constrain(bF, 0, 255);

      r = (uint8_t)rF;
      g = (uint8_t)gF;
      b = (uint8_t)bF;

      if (whiteMode) {
        r = g = b = 0;
        float spec = pow(ledWhiteBlend, 2);
        float wFloat = 255 * brightness + 255 * spec;
        wFloat = constrain(wFloat, 0, 255);
        w = (uint8_t)wFloat;
      }

      strip.setPixelColor(i, strip.Color(r, g, b, w));
    }
  }

  strip.show();

  if (now - lastReset > COLOR_SHIFT_TIME) {
    assignNewPalette();
    lastReset = now;
  }
}
