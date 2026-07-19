#include "DisplayManager.h"

DisplayManager::DisplayManager(uint8_t lcdAddr, uint8_t sdaPin, uint8_t sclPin): 
    _lcd(lcdAddr, 16, 2),
    _sdaPin(sdaPin),
    _sclPin(sclPin),
    _lastScrollTime(0),
    _stringStartPos(0),
    _currentMessage("")
{}

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
        setDisplay("Sensor Error!", "Cannot get data");
        return;
    }
    
    setDisplay("Temp  : " + String(temperature, 1) + " C", "Humid : " + String(humidity, 1) + " %");
}

void DisplayManager::clearDisplay() {
    _lcd.clear();
}

void DisplayManager::setRunningText(String message) {
    if (_currentMessage != message) {
        _currentMessage = message;
        _stringStartPos = 0;
        _lastScrollTime = 0;
    }
}

void DisplayManager::updateRunningText(String staticText, bool isTopDynamic, unsigned long intervalMillis) {
    uint8_t staticRow = isTopDynamic ? 1 : 0;
    uint8_t runningRow = isTopDynamic ? 0 : 1;
    
    _lcd.setCursor(0, staticRow);
    String staticBuffer = staticText;
    
    while (staticBuffer.length() < 16) {
        staticBuffer += " ";
    }
    
    _lcd.print(staticBuffer);
    
    String spaces = "                ";
    String fullText = spaces + _currentMessage + spaces;
    
    unsigned long currentMillis = millis();
    
    if (currentMillis - _lastScrollTime >= intervalMillis) {
        _lastScrollTime = currentMillis;
        
        String visibleText = fullText.substring(_stringStartPos, _stringStartPos + 16);
        
        _lcd.setCursor(0, runningRow);
        _lcd.print(visibleText);
        
        _stringStartPos++;
        
        if (_stringStartPos >= fullText.length() - 16) {
            _stringStartPos = 0;
        }
    }
}

void DisplayManager::showLoading(int startRow, int startCol, int dotCount, int delayMillis) {
    for (int i = 0; i < dotCount; i++) {
        _lcd.setCursor(startCol + i, startRow);
        _lcd.print(".");
        delay(delayMillis);
    }
}