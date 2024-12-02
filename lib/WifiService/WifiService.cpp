#include "WifiService.h"

WifiService::WifiService(const char *ssid, const char *password, bool autoReconnect) : _ssid(ssid), _password(password), _autoReconnect(autoReconnect)
{
}

void WiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi Connected");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
}

void WifiService::connect()
{
    WiFi.onEvent(WiFiConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.setAutoReconnect(_autoReconnect);
    WiFi.begin(_ssid, _password);

    Serial.printf("Connecting to %s\n", _ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(". ");
        delay(1000);
    }
    Serial.print("Connection Sucessfull with ");
    Serial.println(WiFi.RSSI());
}

bool WifiService::ready()
{
    return WiFi.status() == WL_CONNECTED;
}
