#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <Arduino.h>

enum class SystemState {
    READING_SENSOR,
    PROCESSING_DATA,
    IDLE
};

void initSensor();
bool readSensorData(float &temperature, float &humidity);
void printSensorData(float temperature, float humidity);

#endif