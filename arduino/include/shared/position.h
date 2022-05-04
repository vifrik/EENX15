//
// Created by vikto on 2022-03-04.
//

#ifndef ARDUINO_POSITION_H
#define ARDUINO_POSITION_H

#include <cmath>
#include "coord.h"

#define LENGTH_TRAILER 1.16
#define LENGTH_TRUCK 0.22
#define CAMERA_OFFSET 0.04
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
    }
    if(crc < 256) crc |= 1 << 8;
    if (!(crc & 255)) crc |= 1 << 0;
    return crc;
}

public:
    Position() {
        timeOld = millis();
    }

    bool getPositionTrailer(float anglePotentiometer, PositionData& posTruck, PositionData& posTrailer) {
        unsigned char bytesBuffer[15]; 
        int size = Serial.readBytesUntil(byte(0), bytesBuffer, 15);
        if(size != 14) return false;

        memcpy(&posTruck, bytesBuffer, 14);  
        uint16_t c = crc16(bytesBuffer, 12);

        posTruck.x -= cos(posTruck.rz) * CAMERA_OFFSET;
        posTruck.y -= sin(posTruck.rz) * CAMERA_OFFSET;

        posTrailer.x = posTruck.x - cos(posTruck.rz + anglePotentiometer) * LENGTH_TRAILER;
        posTrailer.y = posTruck.y - sin(posTruck.rz + anglePotentiometer) * LENGTH_TRAILER;
        posTrailer.rz = posTruck.rz + PI + anglePotentiometer;

        return c == posTruck.crc;
    }

    float oldAngleDesired = 0;
    float oldTheta = 0;
    float oldError = 0;
    float errorInteg = 0;
    float oldRZ = 0;

    float k_i = 1.0f;
    float k_p = 2.0f;
    float k_d = 1.0f;

    Coord old_pos = Coord(0,0);
    float steeringAngle(PositionData posTruck, PositionData posTrailer, Coord posDesired) {
        uint32_t timeNow = millis();
        float dt = (timeNow - timeOld) / 1000.0f; // [s]

        float angleTrailerWorldDesired = atan2(posDesired.y - posTrailer.y, posDesired.x - posTrailer.x);
        float error = angleTrailerWorldDesired - posTrailer.rz + PI;

        errorInteg += k_i * error * dt;
        float d_error = (error - oldError) / dt;
        oldError = error;

        float angleDesired = k_p * error + k_i * errorInteg + k_d * d_error;
        float d_angleDesired = (angleDesired - oldAngleDesired) / dt;
        oldAngleDesired = angleDesired;

        float d_theta = (posTrailer.rz - oldTheta) / dt;
        oldTheta = posTrailer.rz;

        float dRZ = (posTruck.rz - oldRZ) / dt;
        oldRZ = posTruck.rz;

        float vel = -1 * sqrt(pow(posTrailer.x - old_pos.x, 2) + pow(posTrailer.y - old_pos.y, 2)) / dt;
        old_pos = Coord(posTrailer.x, posTrailer.y);

        float deltaTarget = atan2((d_theta - d_angleDesired - dRZ) * LENGTH_TRUCK, vel);

#ifdef PRINT
#pragma region Print
        Serial.print("posTruck.x: ");
        Serial.println(posTruck.x);

        Serial.print("posTruck.y: ");
        Serial.println(posTruck.y);

        Serial.print("posTruck.rz: ");
        Serial.println(posTruck.rz);

        Serial.print("posTrailer.x: ");
        Serial.println(posTrailer.x);

        Serial.print("posTrailer.y: ");
        Serial.println(posTrailer.y);

        Serial.print("posTrailer.rz: ");
        Serial.println(posTrailer.rz);

        Serial.print("angleTrailerWorldDesired: ");
        Serial.println(angleTrailerWorldDesired);

        Serial.print("deltaTarget: ");
        Serial.println(deltaTarget);

        Serial.print("dt: ");
        Serial.println(dt);
#pragma endregion Region
#endif

        timeOld = timeNow;
        return deltaTarget;
    }
};

#endif // ARDUINO_POSITION_H
