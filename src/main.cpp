#define CONFIG_ARDUHAL_LOG_COLORS 1
#include <Arduino.h>
#include <WifiService.h>
#include <FirebaseService.h>
#include <Door.h>
#include <Barcode.h>
#include <RFID.h>
#include <env.h>

Barcode *barcode = new Barcode(BARCODE_RX_PIN, BARCODE_TX_PIN);
RFID *rfid = new RFID(RFID_BAUD_RATE, RFID_RX_PIN, RFID_TX_PIN);
Door *door = new Door(SERVO_LEFT_PIN, SOLENOID_LEFT_PIN, SERVO_RIGHT_PIN, SOLENOID_RIGHT_PIN);

WifiService *wifi = new WifiService(WIFI_SSID, WIFI_PASSWORD);
FirebaseService *fbs = new FirebaseService(API_KEY, PROJECT_ID, USER_EMAIL, USER_PASSWORD);

int taskComplete = 0;

String barcodeData = "";
String responseValidateBarcode = "";
uint64_t prev = 0;
uint16_t delayRead = 100; // in ms
BookChangeResult resultRfid;

void setup()
{
    Serial.begin(115200);
    rfid->initialize();
    barcode->initialize();

    door->setLeftDoorDegree(DOOR_LEFT_CLOSE_DEGREE, DOOR_LEFT_OPEN_DEGREE);
    door->setRightDoorDegree(DOOR_RIGHT_CLOSE_DEGREE, DOOR_RIGHT_OPEN_DEGREE);
    door->initialize();

    wifi->connect();
    wifi->setNTP(NTP_SERVER, GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC);

    if (wifi->ready())
    {
        fbs->initialize();
    }

    Serial.println("System Ready");
}

void loop()
{
    fbs->appLoop();

    if (millis() - prev > delayRead)
    {
        prev = millis();
        barcodeData = barcode->read();

        if (!barcodeData.isEmpty())
        {
            responseValidateBarcode = fbs->validateBarcode(barcodeData.c_str());
            if (!responseValidateBarcode.isEmpty())
            {
                // open door if barcode validate
                if (!door->isOpen())
                {
                    door->open();
                }
            }
        }

        // close the door and start reading RFID if the barcode is validated, as the door will only open when the barcode is validated.
        if (door->isOpen())
        {
            door->close();
            resultRfid = rfid->read();
            if (resultRfid.status != "unchanged")
            {
                // TODO Update available status book

                // TODO Add Loan history
            }
        }
    }
}