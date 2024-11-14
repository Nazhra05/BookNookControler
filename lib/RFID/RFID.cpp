#include <RFID.h>

RFID::RFID(byte RXPin, byte TXPin)
{
    RFID::_rxPin = RXPin;
    RFID::_txPin = TXPin;
}

RFID::~RFID()
{
}

void RFID::initialize()
{
    RFID::_rfid->begin(9600, SERIAL_8N1, _rxPin, _txPin);
}

String RFID::read()
{
    _rfidTag = "";

    for (int a = 0; a < _duration; a++)
    {
        if (_rfid->available())
        {
            _rfidTag = _rfid->readString();
            _rfidTag.trim(); // Menghapus spasi atau karakter newline

            // Cek jika tag terbaca dan tidak kosong
            if (!_rfidTag.isEmpty())
            {
                Serial.println("RFID Tag Terbaca: " + _rfidTag);
            }
        }
        delay(100);
    }

    return _rfidTag;
}