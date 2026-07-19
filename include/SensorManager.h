#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <DHT.h>

class SensorManager {
    private:
        DHT _dht;
    
    public:
        SensorManager(uint8_t dhtPin, uint8_t dhtType);
        
        void init();
        bool readData(float &temperature, float &humidity);
};

#endif