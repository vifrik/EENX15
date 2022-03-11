#include <ArduinoSerialProtocol.h>

// The payload that will be sent to the other device
struct SamplePayload
{
    int8_t A;
    uint8_t B;
    int16_t C;
    uint16_t D;
} payload;

ArduinoSerialProtocol protocol(&Serial, (uint8_t *)&payload, sizeof(payload));
uint8_t receiveState;

void setup()
{
    Serial.begin(9600);

    payload.A = 0;
    payload.B = 0;
    payload.C = 0;
    payload.D = 0;

    Serial.write("Hello world!");
}

void loop()
{
    receiveState = protocol.receive();

    if (receiveState == ProtocolState::SUCCESS)
    {
        ++payload.A;
        ++payload.B;
        ++payload.C;
        ++payload.D;

        protocol.send();
    }

    delay(1000);
}