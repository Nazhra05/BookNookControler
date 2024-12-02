#ifndef RESPONSE_QUERY_H
#define RESPONSE_QUERY_H
#include <ArduinoJson.h>

struct ResponseQuery
{
    bool success;
    JsonDocument data;
    String message;
};

#endif