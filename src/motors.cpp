#include "motors.h"
#include "pin_mapping.h"

MotorController motors;

void MotorController::begin()
{
    pinMode(MOTOR_A_IN1, OUTPUT);
    pinMode(MOTOR_A_IN2, OUTPUT);

    pinMode(MOTOR_B_IN1, OUTPUT);
    pinMode(MOTOR_B_IN2, OUTPUT);

    stop();
}

void MotorController::setMotor(uint8_t pin1, uint8_t pin2, int speed)
{
    speed = constrain(speed, -255, 255);

    if (speed > 0)
    {
        analogWrite(pin1, speed);
        digitalWrite(pin2, LOW);
    }
    else if (speed < 0)
    {
        digitalWrite(pin1, LOW);
        analogWrite(pin2, -speed);
    }
    else
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
    }
}

void MotorController::setLeft(int speed)
{
    setMotor(MOTOR_B_IN1, MOTOR_B_IN2, speed);
}

void MotorController::setRight(int speed)
{
    setMotor(MOTOR_A_IN1, MOTOR_A_IN2, speed);
}

void MotorController::drive(int leftSpeed, int rightSpeed)
{
    setLeft(leftSpeed);
    setRight(rightSpeed);
}

void MotorController::stop()
{
    setLeft(0);
    setRight(0);
}