#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class DisplayManager {
    private:
        LiquidCrystal_I2C _lcd;
        uint8_t _sdaPin;
        uint8_t _sclPin;
        
    public:
        DisplayManager(uint8_t lcdAddr, uint8_t sdaPin, uint8_t sclPin);
        
        void init();
        void setDisplay(String topLine, String bottomLine);
        void showSensorData(float temperature, float humidity, bool isError);
        void clearDisplay();
};

#endif