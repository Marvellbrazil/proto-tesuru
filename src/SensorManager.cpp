#include "SensorManager.h"

SensorManager::SensorManager(uint8_t dhtPin, uint8_t dhtType)
    : _dht(dhtPin, dhtType) {}

void SensorManager::init() {
    _dht.begin();
}

bool SensorManager::readData(float &temperature, float &humidity) {
    temperature = _dht.readTemperature();
    humidity = _dht.readHumidity();
    
    if (isnan(temperature) || isnan(humidity)) {
        return false;
    }
    
    return true;
}