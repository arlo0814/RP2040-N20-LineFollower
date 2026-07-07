#ifndef PIN_MAPPING_H
#define PIN_MAPPING_H

#include <Arduino.h>

// --- 5-Channel IR Sensor Array ---
const int SENSOR_PINS[5] = {0, 1, 2, 3, 4}; // GP0, GP1, GP2, GP3, GP4

// --- Left Motor Control (L293D Channel 1 & 2) ---
const int MOTOR_LEFT_PWM = 5;  // GP5 (Enable 1,2)
const int MOTOR_LEFT_A   = 6;  // GP6 (Input 1 - Back Flat Pad)
const int MOTOR_LEFT_B   = 7;  // GP7 (Input 2 - Back Flat Pad)

// --- Right Motor Control (L293D Channel 3 & 4) ---
const int MOTOR_RIGHT_A  = 26; // GP26 (Input 3)
const int MOTOR_RIGHT_B  = 27; // GP27 (Input 4)
const int MOTOR_RIGHT_PWM = 28; // GP28 (Enable 3,4)

// --- Control Constraints ---
const int PWM_FREQ = 20000;    // 20kHz Ultrasonic PWM (eliminates motor whine)
const int PWM_RANGE = 1023;    // 10-bit PWM Resolution (0 to 1023 steps)

#endif // PIN_MAPPING_H