#include <Arduino.h>
#include "DisplayManager.h"
#include "SensorManager.h"
#include "LEDManager.h"
#include "WiFiStreamer.h"

#if __has_include("../.env")
    #include "../.env"
#else
    #error "Cannot found .env, please copy the example and adjust its value"
#endif

void checkPauseButton();
void processSensorCycle();

#define SDA_PIN   14
#define SCL_PIN   27
#define DHT_PIN   33
#define DHT_TYPE  DHT22
#define LED_PIN   32

const String SSID = WIFI_SSID;
const String PASS = WIFI_PASS;
const String LAPTOPIP = LAPTOP_IP;
const uint16_t UDP_PORT = 5005;
const unsigned long WIFI_RETRY_INTERVAL = 10000;

IPAddress laptopIPAddr;

SensorManager sensor(DHT_PIN, DHT_TYPE);
DisplayManager display(0x27, SDA_PIN, SCL_PIN);
LEDManager led(LED_PIN);
WiFiStreamer wifiStreamer;

unsigned long previousMillis = 0;
const long interval = 2000;
bool isPaused = false;

void setup() {
  Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);
  delay(1000);
  
  Serial.printf("[INF] Loaded SSID      : '%s'\n", SSID.c_str());
  Serial.printf("[INF] Loaded PASS      : '%s'\n", PASS.c_str());
  Serial.printf("[INF] Loaded LAPTOP IP : '%s'\n", LAPTOPIP.c_str());
  
  laptopIPAddr.fromString(LAPTOPIP);
  
  led.init();
  sensor.init();
  display.init();
  
  wifiStreamer.init(SSID, PASS, laptopIPAddr, UDP_PORT);
  wifiStreamer.connect();
  
  delay(250);
}

void loop() {
  checkPauseButton();
  wifiStreamer.maintain(WIFI_RETRY_INTERVAL);

  if (isPaused) {
    led.setBlink(2000, 4000);
    display.updateRunningText("[INFO]:", false, 225); 
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
    wifiStreamer.sendTelemetry(temp, humid);
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
        display.setRunningText("Program is paused. Hold BOOT to continue...");
      } else {
        Serial.println("[SYS] Program is resumed...");
        display.setDisplay("System Resumed", "Processing");
        display.showLoading(1, 10, 4, 750);
        delay(1500);
      }
      
      while (digitalRead(0) == LOW) {
        delay(10);
      }
    }
  }
}