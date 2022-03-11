#include <ArduinoSerialProtocol.h>
#include <Servo.h>

#include "shared/servoManager.h"

// The payload that will be sent to the other device
struct __attribute__((packed)) SamplePayload
{
    uint64_t B;
} payload;

ArduinoSerialProtocol protocol(&Serial, (uint8_t *)&payload, sizeof(payload));
uint8_t receiveState;

// ServoManager servoManager = ServoManager(11);
Servo servo;

void setup()
{
    Serial.begin(9600);
    payload.B = 0;

    servo.attach(11);
    servo.write(90);

    // servoManager.writeAngle(-90);
}

void loop()
{
    byte bytesBuffer[20]; // 13 should be enough
    int size = Serial.readBytesUntil(byte(0), bytesBuffer, 13);

    float x, y, rz;
    memcpy(&x, bytesBuffer, 4);
    memcpy(&y, bytesBuffer + 4, 4);
    memcpy(&rz, bytesBuffer + 8, 4);

    servo.write((rz + 3.14 / 2) * 180 / 3.14);
}