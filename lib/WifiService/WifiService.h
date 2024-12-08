#ifndef Wifi_Service_H
#define Wifi_service_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

class WifiService
{
private:
    String _ssid;
    String _password;
    String _autoReconnect;

    friend void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
    friend void WiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info);

public:
    WifiService(const char *ssid, const char *password, bool autoReconnect = true);
    void setNTP(const char *ntpServer, uint16_t gmtOffsetSec, uint16_t dayLightOffsetSec);
    String getISOTime();
    void connect();
    bool ready();
};

#endif