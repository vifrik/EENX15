//
// Created by vikto on 2022-03-04.
//

#ifndef ARDUINO_POSITION_DOLLY_H
#define ARDUINO_POSITION_DOLLY_H

#include <cmath>
#include "coord.h"

#define CAMERA_OFFSET 0.15
#define LENGTH_TRUCK 0.15

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
        Coord lookaheadDelta = positionDesired - positionTrailer;
        Serial.print(positionDesired.x - positionTrailer.x);
        Serial.print(" ");
        Serial.println(positionDesired.y - positionTrailer.y);
        return atan2(positionDesired.y - positionTrailer.y, positionDesired.x - positionTrailer.x) * 180 / PI;


        float angleTrailerError = atan2(lookaheadDelta.y, lookaheadDelta.x) + (rz + PI);

        float phiDesired = -atan2(2 * LENGTH_TRUCK * sin(-angleTrailerError),
                                      lookaheadDelta.magnitude());

        /*Serial.print("dy: ");
        Serial.print(lookaheadDelta.y);
        Serial.print(" theta_err: ");
        Serial.print(angleTrailerError);
        Serial.print(" delta: ");
        Serial.println(phiDesired);*/

        return phiDesired*180/PI;
    }
};

#endif // ARDUINO_POSITION_DOLLY_H
