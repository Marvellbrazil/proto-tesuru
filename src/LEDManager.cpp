#include "LEDManager.h"

LEDManager::LEDManager(uint8_t ledPin) : _ledPin(ledPin) {}

void LEDManager::init() {
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);
}

void LEDManager::activate() {
    digitalWrite(_ledPin, HIGH);
}

void LEDManager::deactivate() {
    digitalWrite(_ledPin, LOW);
}

void LEDManager::setBlink(int onInterval, int offInterval) {
    delay(offInterval);
    digitalWrite(_ledPin, HIGH);
    delay(onInterval);
    digitalWrite(_ledPin, LOW);
}