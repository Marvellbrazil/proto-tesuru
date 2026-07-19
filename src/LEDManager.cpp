#include "LEDManager.h"

LEDManager::LEDManager(uint8_t ledPin):
    _ledPin(ledPin),
    _previousLedMillis(0),
    _ledState(false)
{}

void LEDManager::init() {
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);
}

void LEDManager::activate() {
    digitalWrite(_ledPin, HIGH);
    _ledState = true;
}

void LEDManager::deactivate() {
    digitalWrite(_ledPin, LOW);
    _ledState = false;
}

void LEDManager::setBlink(int onInterval, int offInterval) {
    unsigned long currentMillis = millis();
    unsigned long targetInterval = _ledState ? onInterval : offInterval;
    
    if (currentMillis - _previousLedMillis >= targetInterval) {
        _previousLedMillis = currentMillis;
        _ledState = !_ledState;
        digitalWrite(_ledPin, _ledState ? HIGH : LOW);
    }
}