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

    public:
        WiFiStreamer();
        
        void init(const String &ssid, const String &password, IPAddress laptopIP, uint16_t port);
        void connect();
        void sendTelemetry(float temperature, float humidity);
};

#endif