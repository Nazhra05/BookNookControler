#include "WifiService.h"

WifiService::WifiService(const char *ssid, const char *password, bool autoReconnect) : _ssid(ssid), _password(password), _autoReconnect(autoReconnect)
{
}

void WifiService::setNTP(const char *ntpServer, uint16_t gmtOffsetSec, uint16_t dayLightOffsetSec)
{
    configTime(gmtOffsetSec, dayLightOffsetSec, ntpServer);
    Serial.print("Initialize Time : ");
    Serial.println(WifiService::getISOTime());
}

String WifiService::getISOTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return "";
    }
    char buff[30];
    strftime(buff, sizeof(buff), "%Y-%m-%dT%H:%M:%S.00+07:00", &timeinfo);

    return buff;
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
