#include <Arduino.h>
#include "led_engine.h"

LedEngine engine;

void setup() {
    engine.begin();
}

void loop() {
    engine.update();
}
