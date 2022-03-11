#include <ArduinoSerialProtocol.h>

// The payload that will be sent to the other device
struct __attribute__((packed)) SamplePayload
{
    uint8_t B;
} payload;

ArduinoSerialProtocol protocol(&Serial, (uint8_t *)&payload, sizeof(payload));
uint8_t receiveState;

void setup()
{
    Serial.begin(9600);
    payload.B = 0;
}

void loop()
{
    receiveState = protocol.receive();

    if (receiveState == ProtocolState::SUCCESS)
    {
        // perform something, e.g. servo motor
    }
}