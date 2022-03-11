#include <SPI.h>
#include <Wire.h>
#include <Servo.h>

#include "shared/servoManager.h"

// The payload that will be sent to the other device
struct PositionData
{
    float x;
    float y;
    float rz;
} positionData;

// ServoManager servoManager = ServoManager(11);
Servo servo;

void setup()
{
    Serial.begin(9600);

    servo.attach(11);
    servo.write(90);

    // servoManager.writeAngle(-90);
}

void loop()
{
    byte bytesBuffer[13]; // 4 bytes * 3 floats + null terminator, 4+4+4+1 = 13
    int size = Serial.readBytesUntil(byte(0), bytesBuffer, 13);

    memcpy(&positionData, bytesBuffer, 12);

    // servo.write((rz + 3.14 / 2) * 180 / 3.14);
    servo.write(positionData.x * 90);
}