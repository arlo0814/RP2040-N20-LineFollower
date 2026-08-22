#include <Arduino.h>

#include "motors.h"
#include "sensors.h"
#include "pin_mapping.h"

void setup()
{
    Serial.begin(115200);

    pinMode(STATUS_LED, OUTPUT);

    motors.begin();
    sensors.begin();

    Serial.println();
    Serial.println("================================");
    Serial.println("RP2040 N20 Line Follower");
    Serial.println("Firmware: Hardware Bring-Up");
    Serial.println("================================");
}

void loop()
{
    SensorState state = sensors.read();

    Serial.print("S1: ");
    Serial.print(state.s1);

    Serial.print("  S2: ");
    Serial.print(state.s2);

    Serial.print("  S3: ");
    Serial.print(state.s3);

    Serial.print("  S4: ");
    Serial.print(state.s4);

    Serial.print("  S5: ");
    Serial.println(state.s5);

    delay(100);
}