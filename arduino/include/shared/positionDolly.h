//
// Created by vikto on 2022-03-04.
//

#ifndef ARDUINO_POSITION_DOLLY_H
#define ARDUINO_POSITION_DOLLY_H

#include <cmath>
#include "coord.h"

#define CAMERA_OFFSET 0.1
#define LENGTH_TRUCK 0.4

class Position
{
private:
    Coord positionTruck = Coord(0, 0);
    Coord positionRotationalCenter = Coord(0, 0);
    float angleTrailer = 0;

    Coord positionTruckOld = Coord(0, 0);
    float angleTrailerOld = 0;
    float phiDesiredOld = 0;
    uint32_t timeOld = 0;

public:
    Position() = default;

    float steeringAngle(Coord positionTrailer, Coord positionDesired, float rz)
    {
        positionRotationalCenter.x = cos(rz + PI) * CAMERA_OFFSET;
        positionRotationalCenter.y = sin(rz + PI) * CAMERA_OFFSET;
        
        Coord lookaheadDelta = positionDesired - positionRotationalCenter;

        float angleTrailerError = atan2(lookaheadDelta.y, lookaheadDelta.x) + rz * PI / 180;

        float phiDesired = -3 * atan2(2 * LENGTH_TRUCK * sin(angleTrailerError),
                                      lookaheadDelta.magnitude());

        Serial.print("dy: ");
        Serial.print(lookaheadDelta.y);
        Serial.print(" theta_err: ");
        Serial.print(angleTrailerError);
        Serial.print(" delta: ");
        Serial.println(phiDesired);

        return phiDesired*180/PI;
    }
};

#endif // ARDUINO_POSITION_DOLLY_H
