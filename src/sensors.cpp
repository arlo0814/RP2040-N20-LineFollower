#include "sensors.h"
#include "pin_mapping.h"
#include "config.h"

LineSensors sensors;

void LineSensors::begin()
{
    pinMode(SENSOR_S1, INPUT);
    pinMode(SENSOR_S2, INPUT);
    pinMode(SENSOR_S3, INPUT);
    pinMode(SENSOR_S4, INPUT);
    pinMode(SENSOR_S5, INPUT);
}

bool LineSensors::readSensor(uint8_t pin)
{
    bool value = digitalRead(pin);

    if (SENSOR_ACTIVE_LOW)
    {
        return !value;
    }

    return value;
}

SensorState LineSensors::read()
{
    return {
        readSensor(SENSOR_S1),
        readSensor(SENSOR_S2),
        readSensor(SENSOR_S3),
        readSensor(SENSOR_S4),
        readSensor(SENSOR_S5)
    };
}