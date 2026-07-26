#include "WiFiStreamer.h"

WiFiStreamer::WiFiStreamer() 
    : _ssid(""), _password(""), _port(0) {}

void WiFiStreamer::init(const String &ssid, const String &password, IPAddress laptopIP, uint16_t port) {
    _ssid = ssid;
    _password = password;
    _laptopIP = laptopIP;
    _port = port;
}

void WiFiStreamer::connect() {
    if (_ssid.isEmpty() || _password.isEmpty()) {
        Serial.println("[ERR] WiFi credentials not initialized!");
        return;
    }

    Serial.printf("[WIFI] Connecting to %s", _ssid.c_str());
    WiFi.begin(_ssid.c_str(), _password.c_str());
    
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
        delay(500);
        Serial.print(".");
        timeout++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WIFI] Connected successfully!");
        Serial.print("[WIFI] ESP32 IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\n[ERR] WiFi Connection Failed!");
    }
}

void WiFiStreamer::sendTelemetry(float temperature, float humidity) {
    if (WiFi.status() == WL_CONNECTED) {
        _udp.beginPacket(_laptopIP, _port);
        _udp.printf("%.1f,%.1f\n", temperature, humidity);
        _udp.endPacket();
        Serial.println("[NET] Telemetry sent to Excel Streamer!");
    }
}