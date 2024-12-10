#include <RFID.h>

RFID::RFID(uint16_t baudRate, byte RXPin, byte TXPin) : _baudRate(baudRate), _rxPin(RXPin), _txPin(TXPin)
{
}

RFID::~RFID()
{
}

void RFID::initialize()
{
    RFID::_rfid->begin(_baudRate, SERIAL_8N1, _rxPin, _txPin);
}

String RFID::read()
{
    _rfidTag = "";
    Serial.println("================= Start Read RFID ======================");
    for (int a = 0; a < _duration; a++)
    {
        if (_rfid->available())
        {
            // 1. Dapatkan panjang data
            uint8_t length[1];
            RFID::_readBytesFromSerial(length, 1);

            // 2. Dapatkan data
            const int dataLength = length[0];
            uint8_t data[dataLength];
            RFID::_readBytesFromSerial(data, dataLength);

            // 3. Respons lengkap
            const int responseLength = 1 + dataLength;
            uint8_t response[responseLength];

            memcpy(response, length, 1);
            memcpy(response + 1, data, dataLength);

            // 4. Validasi respons
            const int status = response[3];
            const int command = response[2];

            // 5. Dapatkan data tag
            const int tagLength = responseLength - 2 - 4; // 2: panjang checksum, 4: header
            uint8_t tag[tagLength];
            memcpy(tag, &response[4], tagLength);

            // 6. Cek apakah UID yang terdeteksi adalah target UID
            String dataUID = "";
            if (status == 0x00 && command == 0xEE)
            {
                // Tampilkan UID tag jika bukan target UID
                Serial.print("UID: ");
                for (int i = 0; i < tagLength; i++)
                {
                    if (tag[i] < 16)
                    {
                        Serial.print("0");
                        dataUID += "0";
                    }
                    Serial.print(tag[i], HEX);
                    dataUID += String(tag[i], HEX);
                    if (i < tagLength - 1)
                    {
                        Serial.print(" ");
                        dataUID += " ";
                    }
                }
                Serial.println();
            }
            Serial.print("DATA UID IN STRING : ");
            Serial.println(dataUID);
        }
        delay(100);
    }

    Serial.println("================= End Read RFID ======================");

    return _rfidTag;
}

void RFID::_readBytesFromSerial(uint8_t data[], int length)
{
    for (int i = 0; i < length; i++)
    {
        while (!_rfid->available())
        {
            delay(1);
        }
        data[i] = _rfid->read();
    }
}