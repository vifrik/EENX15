#include <cmath>
#include <SPI.h>
#include <Wire.h>

#include "shared/servoManager.h"

#define SERVO_PIN 11
#define ZERO_ANGLE 0


ServoManager servoManager = ServoManager(SERVO_PIN);

void setup() {
    servoManager.writeAngle(ZERO_ANGLE);
    Serial.begin(9600);
}

void loop() {
    servoManager.writeAngle(20);
    Serial.println(20);
    delay(500);
    servoManager.writeAngle(-20);
    Serial.println(-20);
    delay(500);
}