//
// Created by vikto on 2022-03-04.
//

#ifndef ARDUINO_POSITION_H
#define ARDUINO_POSITION_H

#include <cmath>
#include "coord.h"

#define LENGTH_TRAILER 1.16
#define LENGTH_TRUCK 0.22
#define DELTA_MAX 25

struct PositionData {
    float x;
    float y;
    float rz;
    uint16_t crc;
};

class Position {
private:
    uint32_t timeOld = 0;

    uint16_t crc16(unsigned char* data, size_t len) {
        uint16_t crc = 0;
        for (size_t i = 0; i < len; i++) {
            crc ^= data[i];
            for (int k = 0; k < 8; k++) {
                crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
            }
            if(crc < 256) crc |= 1 << 8;
            if (!(crc & 255)) crc |= 1 << 0;
            return crc;
        }
    }

public:
    Position() {
        timeOld = millis();
    }

    bool getPositionTrailer(float angleMagnetic, PositionData& posTruck, PositionData& posTrailer) {
        unsigned char bytesBuffer[15]; 
        int size = SerialUSB.readBytesUntil(byte(0), bytesBuffer, 15);
        if(size != 14) return;

        memcpy(&posTruck, bytesBuffer, 14);  
        uint16_t c = crc16(bytesBuffer, 12);

        posTrailer.x = posTruck.x + cos(angleMagnetic) * LENGTH_TRAILER;
        posTrailer.y = posTruck.y + sin(angleMagnetic) * LENGTH_TRAILER;
        posTrailer.rz = posTruck.rz + PI + angleMagnetic;

        return c == posTruck.crc;
    }

    float oldAngleTrailerWorld, oldAngleTrailerWorldDesired, oldTruckRotation;
    PositionData oldPosTruck;

    float steeringAngle(PositionData posTruck, PositionData posTrailer, Coord posDesired) {
        uint32_t timeNow = millis();
        float dt = (timeNow - timeOld) / 1000.0f; // [s]
        if(dt == 0) {
            timeOld = timeNow;
            return 0;
        }

        float velTruck = sqrt(pow(posTruck.x - oldPosTruck.x, 2) + pow(posTruck.y - oldPosTruck.y, 2)) / dt;

        float angleTrailerWorldDesired = atan2(posDesired.y - posTrailer.y, posDesired.x - posTrailer.x);
        float angleTrailerWorldError = angleTrailerWorldDesired - posTrailer.rz + PI;

        float angleMagneticDesired = angleTrailerWorldDesired + angleTrailerWorldError;   

        float d_angleTrailerWorld = (posTrailer.rz - oldAngleTrailerWorld) / dt;
        float d_angleTrailerWorldDesired = (angleTrailerWorldDesired - oldAngleTrailerWorldDesired) / dt;
        float d_truckRotation = (posTruck.rz - oldTruckRotation) / dt;
        
        oldPosTruck = posTruck;
        oldAngleTrailerWorld = posTrailer.rz;
        oldAngleTrailerWorldDesired = angleTrailerWorldDesired;
        oldTruckRotation = posTruck.rz;

        float deltaTarget = -atan2((d_angleTrailerWorld - d_angleTrailerWorldDesired - d_truckRotation) * LENGTH_TRUCK, velTruck);
        
        timeOld = timeNow;
        return deltaTarget;
    }
};

#endif // ARDUINO_POSITION_H
