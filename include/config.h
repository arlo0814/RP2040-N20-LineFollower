#pragma once

#include <Arduino.h>

// ============================================================
// MOTOR CONFIGURATION
// ============================================================

constexpr uint16_t MOTOR_PWM_FREQUENCY = 20000;
constexpr uint8_t MOTOR_PWM_RESOLUTION = 8;

constexpr int MOTOR_TEST_SPEED = 100;


// ============================================================
// SENSOR CONFIGURATION
// ============================================================

// BFD-1000-style sensor:
// Set true if the sensor output is LOW when detecting the line.
constexpr bool SENSOR_ACTIVE_LOW = true;