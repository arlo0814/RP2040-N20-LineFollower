#pragma once

#include <Arduino.h>

class MotorController
{
public:
    void begin();

    void setLeft(int speed);
    void setRight(int speed);

    void drive(int leftSpeed, int rightSpeed);

    void stop();

private:
    void setMotor(uint8_t pin1, uint8_t pin2, int speed);
};

extern MotorController motors;