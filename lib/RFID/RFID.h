#ifndef RFID_H
#define RFID_H
#include <Arduino.h>
#include <HardwareSerial.h>
#include <DynamicArray.h>
#include <BookChangeResult.h>

class RFID
{
private:
    byte _rxPin;
    byte _txPin;
    u_int16_t _baudRate;
    String _rfidTag;
    byte _duration = 10;
    DynamicArray<String> _currentBooks;
    DynamicArray<String> _scannedBooks;
    HardwareSerial *_rfid = new HardwareSerial(2);
    void _readBytesFromSerial(uint8_t data[], int length);

public:
    RFID(uint16_t baudRate, byte RXPin, byte TXPin);
    ~RFID();
    void initialize();
    void addScannedBooks(const String &bookUid);
    BookChangeResult detectBookChange();
    DynamicArray<String> findAddedBooks();
    DynamicArray<String> findRemoveBooks();
    BookChangeResult read();
};

#endif