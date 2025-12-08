#pragma once

#include <Adafruit_NeoPixel.h>

class LedEngine {
public:
    LedEngine();

    // run once at startup
    void begin();

    // called repeatedly in loop()
    void update();

private:
    struct ClusterState {
        unsigned long nextFlash;
        unsigned long startTime;
        bool   flashing;
        float  hue;
        float  hueDrift;
        float  facetAngle;
        uint8_t flashMode;
    };

    static const int CLUSTER_COUNT = 7;

    // helpers (same logic as your original functions)
    void hsvToRgb(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b);
    void assignNewPalette();
    void triggerClusterFlare();

    // state that used to be global / static
    Adafruit_NeoPixel strip;
    ClusterState clusters[CLUSTER_COUNT];

    bool whiteMode;
    unsigned long modeStart;
    unsigned long lastFlare;
    unsigned long lastReset;
};
