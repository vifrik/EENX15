void setup(){};

int baudrates[] = {57600, 76800, 115200, 230400, 250000, 500000, 1000000};

void loop()
{
    for (int baudrate : baudrates) {
        Serial.begin(baudrate);
        Serial.printf("Baudrate = %s", baudrate);
        delay(500);
    }
};