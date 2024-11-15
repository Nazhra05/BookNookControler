#ifndef Door_H
#define Door_H
#include <Arduino.h>
#include <ESP32Servo.h>

class Door
{
private:
    Servo _servoLeftDoor;
    byte _servoLeftPin;
    byte _solenoidLeftPin;
    byte _openLeftDegree;
    byte _closeLeftDegree;
    bool _isOpenCloseLeftDegreeDefined = false;

    Servo _servoRightDoor;
    byte _servoRightPin;
    byte _solenoidRightPin;
    byte _closeRightDegree;
    byte _openRightDegree;
    bool _isOpenCloseRightDegreeDefined = false;

    bool _isOpened = false;

    void _degreeIncreament(byte start, byte end);
    void _degreeDecreament(byte start, byte end);

public:
    Door(byte servoLeftPin, byte selenoidLeftPin, byte servoRightPin, byte solenoidRightPin);
    void setLeftDoorDegree(byte closeDegree, byte openDegree);
    void setRightDoorDegree(byte closeDegree, byte openDegree);
    void initialize();
    void open();
    void close();
    bool isOpen();
};

#endif