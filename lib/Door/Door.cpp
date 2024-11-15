#include <Door.h>

Door::Door(byte servoLeftPin, byte solenoidLeftPin, byte servoRightPin, byte solenoidRightPin)
{
    Door::_servoLeftPin = servoLeftPin;
    Door::_servoRightPin = servoRightPin;
    Door::_solenoidLeftPin = solenoidLeftPin;
    Door::_solenoidRightPin = solenoidRightPin;
}

void Door::setLeftDoorDegree(byte closeDegree, byte openDegree)
{
    Door::_closeLeftDegree = closeDegree;
    Door::_openLeftDegree = openDegree;
    Door::_isOpenCloseLeftDegreeDefined = true;
}
void Door::setRightDoorDegree(byte closeDegree, byte openDegree)
{
    Door::_closeRightDegree = closeDegree;
    Door::_openRightDegree = openDegree;
    Door::_isOpenCloseRightDegreeDefined = true;
}

void Door::initialize()
{
    if (!Door::_isOpenCloseLeftDegreeDefined)
    {
        ESP_LOGE("LEFT DOOR", "Degree for Left Open and Close Left Door Undefined\nPlease define using function setLeftDoorDegree");
        while (true)
            ;
    }
    Door::_servoLeftDoor.attach(Door::_servoLeftPin);
    pinMode(Door::_solenoidLeftPin, OUTPUT);
    Door::_servoLeftDoor.write(_closeLeftDegree);

    if (!Door::_isOpenCloseRightDegreeDefined)
    {
        ESP_LOGE("RIGHT DOOR", "Degree for Right Open and Close Right Door Undefined\nPlease define using function setRightDoorDegree");
        while (true)
            ;
    }
    Door::_servoRightDoor.attach(Door::_servoRightPin);
    pinMode(Door::_solenoidRightPin, OUTPUT);
    Door::_servoRightDoor.write(_closeRightDegree);
}

void Door::open()
{
    digitalWrite(Door::_solenoidLeftPin, HIGH);
    digitalWrite(Door::_solenoidRightPin, HIGH);
    delay(500); // Tunggu agar solenoid terbuka

    for (int degree = _closeLeftDegree; degree >= _openLeftDegree; degree--)
    {
        Door::_servoLeftDoor.write(degree);
        Door::_servoRightDoor.write(_openRightDegree - degree);
    }

    digitalWrite(Door::_solenoidLeftPin, LOW);
    digitalWrite(Door::_solenoidRightPin, LOW);

    Door::_isOpened = true;
}

void Door::close()
{
    for (int degree = _openLeftDegree; degree <= _closeLeftDegree; degree++)
    {
        Door::_servoLeftDoor.write(degree);
        Door::_servoRightDoor.write(_openRightDegree - degree);
    }
    Door::_isOpened = false;
}

bool Door::isOpen()
{
    return Door::_isOpened;
}