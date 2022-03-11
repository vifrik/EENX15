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
    String s = Serial.readStringUntil(byte(0));
    double rotZ = s.toDouble();

    servo.write((rotZ + 3.14 / 2) * 180 / 3.14);
}