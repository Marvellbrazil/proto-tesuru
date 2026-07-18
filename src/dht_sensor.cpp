#include "dht_sensor.h"
#include <DHT.h>

const int DHTPIN = 4;
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void initSensor() {
    dht.begin();
}

bool readSensorData(float &temperature, float &humidity) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    
    if (isnan(temperature) || isnan(humidity)) {
        return false;
    }
    
    return true;
}

void printSensorData(float temperature, float humidity) {
    Serial.print("\nTemperature: ");
    Serial.println(temperature, 1);
    Serial.print("Humidity: ");
    Serial.println(humidity, 1);
}