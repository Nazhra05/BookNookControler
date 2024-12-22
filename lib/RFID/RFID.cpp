#include <RFID.h>
#include <algorithm>

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

BookChangeResult RFID::read()
{
    _scannedBooks.clear();

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
                Serial.println(dataUID);
            }
            if (dataUID != "")
            {
                addScannedBooks(dataUID);
            }
        }
        delay(100);
    }
    BookChangeResult result = detectBookChange();
    Serial.printf("Status : %s\n", result.status);
    if (result.status != "unchanged")
    {
        for (const String &bookUid : result.data)
        {
            Serial.println(bookUid);
        }
    }
    Serial.println();
    return result;
}

void RFID::addScannedBooks(const String &bookUid)
{
    for (const String &existingUid : _scannedBooks)
    {
        if (existingUid == bookUid)
            return;
    }
    _scannedBooks.push_back(bookUid);
}

BookChangeResult RFID::detectBookChange()
{
    BookChangeResult result;
    size_t currentBookCount = _currentBooks.size();
    size_t scannedBookCount = _scannedBooks.size();

    if (currentBookCount < scannedBookCount)
    {
        result.status = "added";
        result.data = findAddedBooks();
    }
    else if (currentBookCount > scannedBookCount)
    {
        result.status = "remove";
        result.data = findRemoveBooks();
    }
    else
    {
        result.status = "unchanged";
    }
    return result;
}

DynamicArray<String> RFID::findAddedBooks()
{
    DynamicArray<String> addedBooks;
    for (const String &bookUid : _scannedBooks)
    {
        if (std::find(_currentBooks.begin(), _currentBooks.end(), bookUid) == _currentBooks.end())
        {
            addedBooks.push_back(bookUid);
        }
    }

    return addedBooks;
}

DynamicArray<String> RFID::findRemoveBooks()
{
    DynamicArray<String> removeBooks;
    for (const String &bookUid : _currentBooks)
    {
        if (std::find(_scannedBooks.begin(), _scannedBooks.end(), bookUid) == _scannedBooks.end())
        {
            removeBooks.push_back(bookUid);
        }
    }

    return removeBooks;
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