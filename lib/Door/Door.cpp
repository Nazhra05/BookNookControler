#include <Door.h>

Door::Door(byte servoPin, byte selenoidPin, byte closeDegree, byte openDegree)
{
    Door::_servoPin = servoPin;
    Door::_selenoidPin = selenoidPin;
    Door::_closeDegree = closeDegree;
    Door::_openDegree = openDegree;
}

void Door::initialize()
{
    Door::_servo.attach(Door::_servoPin);
    pinMode(Door::_selenoidPin, OUTPUT);
    Door::_servo.write(_closeDegree);
}

void Door::open()
{
    digitalWrite(Door::_selenoidPin, HIGH);
    delay(500); // Tunggu agar solenoid terbuka

    (_closeDegree < _openDegree) ? Door::_degreeIncreament(_closeDegree, _openDegree) : Door::_degreeDecreament(_closeDegree, _openDegree);

    digitalWrite(Door::_selenoidPin, LOW);

    Door::_isOpened = true;
}

void Door::close()
{
    (_closeDegree < _openDegree) ? Door::_degreeDecreament(_openDegree, _closeDegree) : Door::_degreeIncreament(_openDegree, _closeDegree);

    Door::_isOpened = false;
}

bool Door::isOpen()
{
    return Door::_isOpened;
}

void Door::_degreeIncreament(byte start, byte end)
{
    for (int pos = start; pos <= end; pos++)
    {
        Door::_servo.write(pos);
        delay(10); // Membuat gerakan lebih cepat
    }
}

void Door::_degreeDecreament(byte start, byte end)
{
    for (int pos = start; pos >= end; pos--)
    {
        Door::_servo.write(pos);
        delay(10); // Membuat gerakan lebih cepat
    }
}
