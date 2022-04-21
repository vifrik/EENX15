#include "Wire.h"

class Potentiometer {
    public:
        float read() {
            uint32_t sensorReading = analogRead(A0);
            float angle = map(sensorReading,250,780,-90,90)*PI/180;
            return angle;
        }
};