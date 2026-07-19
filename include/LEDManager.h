#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>

class LEDManager {
    private:
        uint8_t _ledPin;

    public:
        LEDManager(uint8_t ledPin);
        
        void init();
        void activate();
        void deactivate();
        void setBlink(int onInterval, int offInterval);
};

#endif