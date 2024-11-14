#ifndef RFID_H
#define RFID_H
#include <Arduino.h>
#include <HardwareSerial.h>

class RFID
{
private:
    byte _rxPin;
    byte _txPin;
    String _rfidTag;
    byte _duration = 10;
    HardwareSerial *_rfid = new HardwareSerial(2);

public:
    RFID(byte RXPin, byte TXPin);
    ~RFID();
    void initialize();
    String read();
};

#endif