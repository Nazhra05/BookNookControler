#ifndef Door_H
#define Door_H
#include <Arduino.h>
#include <ESP32Servo.h>

class Door
{
private:
    Servo _servo;
    byte _servoPin;
    byte _selenoidPin;
    byte _closeDegree;
    byte _openDegree;
    bool _isOpened = false;

    void _degreeIncreament(byte start, byte end);
    void _degreeDecreament(byte start, byte end);

public:
    Door(byte servoPin, byte selenoidPin, byte closeDegree, byte openDegree);
    void initialize();
    void open();
    void close();
    bool isOpen();
};

#endif