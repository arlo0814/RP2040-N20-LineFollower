#pragma once

#include <Arduino.h>

struct SensorState
{
    bool s1;
    bool s2;
    bool s3;
    bool s4;
    bool s5;
};

class LineSensors
{
public:
    void begin();

    SensorState read();

private:
    bool readSensor(uint8_t pin);
};

extern LineSensors sensors;