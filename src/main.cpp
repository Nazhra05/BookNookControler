#define CONFIG_ARDUHAL_LOG_COLORS 1
#include <Arduino.h>
#include <WifiService.h>
#include <FirebaseService.h>
// #include <Door.h>
// #include <Barcode.h>
// #include <RFID.h>
#include <env.h>

// Barcode *barcode = new Barcode(BARCODE_RX_PIN, BARCODE_TX_PIN);
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

    if (wifi->ready())
    {
        fbs->initialize();
    }

    Serial.println("System Ready");
}

void loop()
{
    fbs->appLoop();

    if (fbs->isReady() && taskComplete < 1)
    {
        fbs->validateBarcode("barcode-EKNomuQddtThBpW0a5UkVgOyLfH2");
        delay(1000);
        taskComplete++;
    }

    // if (millis() - prev > delayRead)
    // {
    //     prev = millis();
    //     barcodeData = barcode->read();
    //     if (barcodeData == "Buka" && !door->isOpen())
    //     {
    //         door->open();
    //     }
    //     else if (barcodeData == "Tutup" && door->isOpen())
    //     {
    //         door->close();
    //     }
    //     else if (barcodeData == "Baca")
    //     {
    //         Serial.println(fbs->getData("aa/bb"));
    //         rfid->read();
    //     }
    // }
}