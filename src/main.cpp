#define CONFIG_ARDUHAL_LOG_COLORS 1
#include <Arduino.h>
#include <WifiService.h>
#include <FirebaseService.h>
// #include <Door.h>
#include <Barcode.h>
// #include <RFID.h>
#include <env.h>

Barcode *barcode = new Barcode(BARCODE_RX_PIN, BARCODE_TX_PIN);
// RFID *rfid = new RFID(RFID_RX_PIN, RFID_TX_PIN);
// Door *door = new Door(SERVO_LEFT_PIN, SOLENOID_LEFT_PIN, SERVO_RIGHT_PIN, SOLENOID_RIGHT_PIN);

WifiService *wifi = new WifiService(WIFI_SSID, WIFI_PASSWORD);
FirebaseService *fbs = new FirebaseService(API_KEY, PROJECT_ID, USER_EMAIL, USER_PASSWORD);

int taskComplete = 0;

String barcodeData = "";
uint64_t prev = 0;
uint16_t delayRead = 100; // in ms

void setup()
{
    Serial.begin(115200);
    // rfid->initialize();
    barcode->initialize();

    // door->setLeftDoorDegree(DOOR_LEFT_CLOSE_DEGREE, DOOR_LEFT_OPEN_DEGREE);
    // door->setRightDoorDegree(DOOR_RIGHT_CLOSE_DEGREE, DOOR_RIGHT_OPEN_DEGREE);
    // door->initialize();

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

        if (fbs->isReady() && !barcodeData.isEmpty())
        {
            String uid = fbs->validateBarcode(barcodeData);

            if (!uid.isEmpty())
            {
                if (fbs->addHistory(uid.c_str(), wifi->getISOTime().c_str()))
                {
                    Serial.println("Berhasil Menambahkan History");
                }
                else
                {
                    Serial.println("Gagal Menambahkan History");
                }
            }
        }
    }
}