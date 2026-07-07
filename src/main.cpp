#include <Arduino.h>
#include "pin_mapping.h"

// PID Tuning Constants
float Kp = 150.0;
float Kd = 80.0;
int lastError = 0;
int baseSpeed = 600; // ~60% power base cruise speed

void setup() {
    // 1. Initialize Sensor Inputs
    for (int i = 0; i < 5; i++) {
        pinMode(SENSOR_PINS[i], INPUT);
    }

    // 2. Initialize Motor Driver Outputs
    pinMode(MOTOR_LEFT_PWM, OUTPUT);
    pinMode(MOTOR_LEFT_A, OUTPUT);
    pinMode(MOTOR_LEFT_B, OUTPUT);
    pinMode(MOTOR_RIGHT_PWM, OUTPUT);
    pinMode(MOTOR_RIGHT_A, OUTPUT);
    pinMode(MOTOR_RIGHT_B, OUTPUT);

    // 3. Set Default Forward Direction
    digitalWrite(MOTOR_LEFT_A, HIGH);
    digitalWrite(MOTOR_LEFT_B, LOW);
    digitalWrite(MOTOR_RIGHT_A, HIGH);
    digitalWrite(MOTOR_RIGHT_B, LOW);

    // 4. Force RP2040 Hardware Timers to Silent 20kHz mode
    analogWriteFreq(PWM_FREQ);
    analogWriteRange(PWM_RANGE);
}

void loop() {
    int sensorValues[5];
    int activeSensors = 0;
    long totalWeight = 0;
    int weights[5] = {-2, -1, 0, 1, 2};

    // Read lines (Assumes 1 = Black Line, 0 = White Track Surface)
    for (int i = 0; i < 5; i++) {
        sensorValues[i] = (digitalRead(SENSOR_PINS[i]) == HIGH) ? 1 : 0;
    }

    int error = 0;
    for (int i = 0; i < 5; i++) {
        if (sensorValues[i] == 1) {
            error += weights[i];
            activeSensors++;
        }
    }

    // Handle losing the line entirely (Memory Recovery Fallback)
    if (activeSensors == 0) {
        if (lastError < 0) error = -2;      // Keep pivoting hard left
        else if (lastError > 0) error = 2;  // Keep pivoting hard right
    } else {
        error = error / activeSensors;       // Center point average
    }

    // PD Controller Calculation Block
    int derivative = error - lastError;
    int correction = (Kp * error) + (Kd * derivative);
    lastError = error;

    // Calculate Differential Steering Speeds
    int leftSpeed  = baseSpeed + correction;
    int rightSpeed = baseSpeed - correction;

    // Clamp values inside 10-bit boundaries (0 to 1023)
    leftSpeed  = constrain(leftSpeed, 0, PWM_RANGE);
    rightSpeed = constrain(rightSpeed, 0, PWM_RANGE);

    // Write PWM Duty Cycles to L293D Enable Inputs
    analogWrite(MOTOR_LEFT_PWM, leftSpeed);
    analogWrite(MOTOR_RIGHT_PWM, rightSpeed);

    delay(1); // Run execution loop at an unthrottled ~1kHz refresh cycle
}