#ifndef Barcode_H
#define Barcode_h
#include <Arduino.h>
#include <HardwareSerial.h>

class Barcode
{
private:
    byte _rxPin;
    byte _txPin;
    HardwareSerial *_barcode = new HardwareSerial(1);

public:
    Barcode(byte RXPin, byte TXPin);
    ~Barcode();
    void initialize();
    String read();
};

#endif