#define CONFIG_ARDUHAL_LOG_COLORS 1
#include <Arduino.h>
#include <Door.h>
#include <Barcode.h>
#include <RFID.h>
#include <env.h>

Barcode *barcode = new Barcode(BARCODE_RX_PIN, BARCODE_TX_PIN);
RFID *rfid = new RFID(RFID_RX_PIN, RFID_TX_PIN);
Door *door = new Door(SERVO_LEFT_PIN, SOLENOID_LEFT_PIN, SERVO_RIGHT_PIN, SOLENOID_RIGHT_PIN);

String barcodeData = "";

void setup()
{
  Serial.begin(115200);
  rfid->initialize();
  barcode->initialize();

  door->setLeftDoorDegree(DOOR_LEFT_CLOSE_DEGREE, DOOR_LEFT_OPEN_DEGREE);
  door->setRightDoorDegree(DOOR_RIGHT_CLOSE_DEGREE, DOOR_RIGHT_OPEN_DEGREE);
  door->initialize();

  Serial.println("System Ready");
}

void loop()
{
  barcodeData = barcode->read();
  if (barcodeData == "Buka" && !door->isOpen())
  {
    door->open();
  }
  else if (barcodeData == "Tutup" && door->isOpen())
  {
    door->close();
  }
  else if (barcodeData == "Baca")
  {
    rfid->read();
  }
  delay(100);
}