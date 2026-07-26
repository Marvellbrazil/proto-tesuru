#include "WiFiStreamer.h"

WiFiStreamer::WiFiStreamer()
    : _ssid(""), _password(""), _port(0), _lastReconnectAttempt(0), _sequence(0), _sentCount(0) {}

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

    _lastReconnectAttempt = millis();
}

bool WiFiStreamer::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiStreamer::maintain(unsigned long retryIntervalMillis) {
    if (isConnected()) {
        return;
    }

    unsigned long currentMillis = millis();
    if (currentMillis - _lastReconnectAttempt < retryIntervalMillis) {
        return;
    }
    _lastReconnectAttempt = currentMillis;

    Serial.println("[WIFI] Connection lost, attempting reconnect...");
    WiFi.disconnect();
    WiFi.begin(_ssid.c_str(), _password.c_str());
}

void WiFiStreamer::sendTelemetry(float temperature, float humidity) {
    _sequence++;

    if (!isConnected()) {
        Serial.printf("[ERR] Reading #%lu dropped, WiFi offline!\n", (unsigned long)_sequence);
        return;
    }

    _udp.beginPacket(_laptopIP, _port);
    _udp.printf("%lu,%.1f,%.1f\n", (unsigned long)_sequence, temperature, humidity);
    _udp.endPacket();
    _sentCount++;

    Serial.printf("[NET] Telemetry #%lu sent to Excel Streamer!\n", (unsigned long)_sequence);
}

uint32_t WiFiStreamer::getSentCount() {
    return _sentCount;
}