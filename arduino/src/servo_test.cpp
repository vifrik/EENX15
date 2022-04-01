#include <cmath>
#include <SPI.h>
#include <Wire.h>

#include "shared/servoManager.h"
#include "shared/motor.h"

#define SERVO_PIN 11
#define MOTOR_FIRST_DIR_PIN 2
#define MOTOR_SECOND_DIR_PIN 3
#define MOTOR_SPEED_PIN 4
#define MOTOR_SPEED 125

#define ZERO_ANGLE 0


ServoManager servoManager = ServoManager(SERVO_PIN);
Motor motor = Motor(MOTOR_FIRST_DIR_PIN, MOTOR_SECOND_DIR_PIN, MOTOR_SPEED_PIN);


void setup() {
    servoManager.writeAngle(ZERO_ANGLE);
    Serial.begin(9600);

    motor.setBackwards();
    motor.setSpeed(MOTOR_SPEED);
}

void loop() {
    servoManager.writeAngle(20);
    Serial.println(20);
    delay(500);
    servoManager.writeAngle(-20);
    Serial.println(-20);
    delay(500);
}