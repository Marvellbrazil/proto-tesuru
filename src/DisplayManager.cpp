#include "DisplayManager.h"

DisplayManager::DisplayManager(uint8_t lcdAddr, uint8_t sdaPin, uint8_t sclPin)
    : _lcd(lcdAddr, 16, 2), _sdaPin(sdaPin), _sclPin(sclPin) {}

void DisplayManager::init() {
    Wire.begin(_sdaPin, _sclPin);
    
    _lcd.init();
    _lcd.backlight();
    _lcd.clear();
    
    delay(1150);
    
    _lcd.setCursor(0, 0);
    _lcd.print("Tesuru Prototype");
    _lcd.setCursor(0, 1);
    _lcd.print("Starting");
    
    for (int i = 8; i <= 11; i++) {
        _lcd.setCursor(i, 1);
        _lcd.print(".");
        delay(1000);
    }
}

void DisplayManager::setDisplay(String topLine, String bottomLine) {
    if (topLine.length() > 16 || bottomLine.length() > 16) {
        Serial.println("[WARN] Character length cannot exceed 16!");
        return;
    }
    
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print(topLine);
    _lcd.setCursor(0, 1);
    _lcd.print(bottomLine);
}

void DisplayManager::showSensorData(float temperature, float humidity, bool isSuccess) {
    _lcd.clear();
    
    if (!isSuccess) {
        DisplayManager::setDisplay("Sensor Error!", "Cannot get data");
        return;
    }
    
    DisplayManager::setDisplay("Temp  : " + String(temperature, 1) + " C", "Humid : " + String(humidity, 1) + " %");
}

void DisplayManager::clearDisplay() {
    _lcd.clear();
}