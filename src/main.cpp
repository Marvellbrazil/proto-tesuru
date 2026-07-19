#include <Arduino.h>
#include "DisplayManager.h"
#include "SensorManager.h"
#include "LEDManager.h"

void checkPauseButton();
void processSensorCycle();

#define SDA_PIN   14
#define SCL_PIN   27
#define DHT_PIN   33
#define DHT_TYPE  DHT22
#define LED_PIN   32

SensorManager sensor(DHT_PIN, DHT_TYPE);
DisplayManager display(0x27, SDA_PIN, SCL_PIN);
LEDManager led(LED_PIN);

unsigned long previousMillis = 0;
const long interval = 2000;
bool isPaused = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);
  
  delay(1000);
  
  led.init();
  sensor.init();
  display.init();
  
  delay(250);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkPauseButton();
  
  if (isPaused) {
    led.setBlink(2000, 4000);
    return;
  }
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    processSensorCycle();
  }
}

void processSensorCycle() {
  float temp = 0.0f;
  float humid = 0.0f;
  
  Serial.println("[SYS] Fetching data...");
  led.activate();
  delay(100);
  
  bool isSuccess = sensor.readData(temp, humid);
  
  if (isSuccess) {
    Serial.printf("[INF] Temperature: %.1f C | Humidity: %.1f %%\n", temp, humid);
  } else {
    Serial.println("[ERR] Failed to receive DHT22 data!");
  }
  
  display.showSensorData(temp, humid, isSuccess);
  led.deactivate();
}

void checkPauseButton() {
  if (digitalRead(0) == LOW) {
    delay(50);
    if (digitalRead(0) == LOW) {
      isPaused = !isPaused;
      
      if (isPaused) {
        Serial.println("[SYS] Program is paused");
        display.setDisplay("System Paused", "BOOT to continue");
      } else {
        Serial.println("[SYS] Program is resumed...");
        display.setDisplay("System Resumed", "Processing...");
      }
      
      while (digitalRead(0) == LOW) {
        delay(10);
      }
    }
  }
}