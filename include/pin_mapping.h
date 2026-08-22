#pragma once

#include <Arduino.h>

// ============================================================
// TC1508A MOTOR DRIVER
// ============================================================

constexpr uint8_t MOTOR_A_IN1 = 2;
constexpr uint8_t MOTOR_A_IN2 = 3;

constexpr uint8_t MOTOR_B_IN1 = 4;
constexpr uint8_t MOTOR_B_IN2 = 5;


// ============================================================
// 5-CHANNEL IR SENSOR
// ============================================================

constexpr uint8_t SENSOR_S1 = 14;
constexpr uint8_t SENSOR_S2 = 15;
constexpr uint8_t SENSOR_S3 = 26;
constexpr uint8_t SENSOR_S4 = 27;
constexpr uint8_t SENSOR_S5 = 28;

constexpr uint8_t SENSOR_COUNT = 5;


// ============================================================
// OPTIONAL PERIPHERALS
// ============================================================

constexpr uint8_t STATUS_LED = 16;