#ifndef WIFI_STREAMER_H
#define WIFI_STREAMER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

class WiFiStreamer {
    private:
        String _ssid;
        String _password;
        IPAddress _laptopIP;
        uint16_t _port;
        WiFiUDP _udp;
        unsigned long _lastReconnectAttempt;
        uint32_t _sequence;
        uint32_t _sentCount;

    public:
        WiFiStreamer();

        void init(const String &ssid, const String &password, IPAddress laptopIP, uint16_t port);
        void connect();
        void maintain(unsigned long retryIntervalMillis);
        bool isConnected();
        void sendTelemetry(float temperature, float humidity);
        uint32_t getSentCount();
};

#endif