#ifndef RFID_H
#define RFID_H
#include <Arduino.h>
#include <HardwareSerial.h>

class RFID
{
private:
    byte _rxPin;
    byte _txPin;
    uint16_t _baudRate;
    String _rfidTag;
    byte _duration = 10;
    HardwareSerial *_rfid = new HardwareSerial(2);
    void _readBytesFromSerial(uint8_t data[], int length);

public:
    RFID(uint16_t baudRate, byte RXPin, byte TXPin);
    ~RFID();
    void initialize();
    String read();
};

#endif