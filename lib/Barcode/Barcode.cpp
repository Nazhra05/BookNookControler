#include <Barcode.h>

Barcode::Barcode(byte RXPin, byte TXPin)
{
    Barcode::_rxPin = RXPin;
    Barcode::_txPin = TXPin;
}

Barcode::~Barcode()
{
}

void Barcode::initialize()
{
    Barcode::_barcode->begin(9600, SERIAL_8N1, _rxPin, _txPin);
}

String Barcode::read()
{
    if (_barcode->available() < 1)
        return "";

    String barcodeData = _barcode->readStringUntil('\n');
    barcodeData.trim(); // Menghapus spasi atau newline di akhir data
    Serial.println("Data Barcode: " + barcodeData);
    return barcodeData;
}
