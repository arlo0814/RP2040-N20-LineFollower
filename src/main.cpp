#include <Arduino.h>
#include "motors.h"

void setup()
{
    Serial.begin(115200);

    motors.begin();

    Serial.println();
    Serial.println("================================");
    Serial.println("RP2040 N20 MOTOR TEST");
    Serial.println("================================");

    delay(2000);
}

void loop()
{
    Serial.println("LEFT MOTOR FORWARD");
    motors.setLeft(80);
    delay(2000);

    motors.stop();
    delay(1000);

    Serial.println("LEFT MOTOR REVERSE");
    motors.setLeft(-80);
    delay(2000);

    motors.stop();
    delay(1000);

    Serial.println("RIGHT MOTOR FORWARD");
    motors.setRight(80);
    delay(2000);

    motors.stop();
    delay(1000);

    Serial.println("RIGHT MOTOR REVERSE");
    motors.setRight(-80);
    delay(2000);

    motors.stop();
    delay(2000);
}