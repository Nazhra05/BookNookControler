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
uint16_t delayReadBarcode = 100;     // in ms
uint16_t delayReadDoorStatus = 5000; // in ms
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

    // read barcode only when door is closed
    if (millis() - prev > delayReadBarcode && !door->isOpen())
    {
        prev = millis();
        barcodeData = barcode->read();

        if (!barcodeData.isEmpty())
        {
            responseValidateBarcode = fbs->validateBarcode(barcodeData.c_str());
            if (!responseValidateBarcode.isEmpty())
            {
                // open door if barcode validate
                door->open();
                // set  door is open to firebase
                fbs->setDoorStatusOpen(responseValidateBarcode.c_str(), wifi->getISOTime().c_str());
            }
        }
    }

    // check doorStatus and scan rfid if doorStatus is "closed" and door is open
    if (millis() - prev > delayReadDoorStatus && door->isOpen())
    {
        prev = millis();
        // check status door
        String doorStatus = fbs->getDoorStatus();
        // check if doorStatus is closed
        if (!doorStatus.isEmpty() && doorStatus == "closed")
        {
            door->close();
            resultRfid = rfid->read();
            if (resultRfid.status != "unchanged")
            {
                // Update available status book
                if (fbs->updateBookAvailable(resultRfid))
                {
                    Serial.println("Success Update Available Books");
                }

                // only add history when borrowing
                if (resultRfid.status == "remove")
                {
                    // Add Loan history
                    if (fbs->addHistory(responseValidateBarcode.c_str(), resultRfid, wifi->getISOTime().c_str(), wifi->getISOTimeWithAddition(72).c_str()))
                    {
                        Serial.println("Success Add Loan History");
                    }
                }
            }
        }
    }
}