//
// Created by vikto on 2022-03-04.
//

#ifndef ARDUINO_POSITION_DOLLY_H
#define ARDUINO_POSITION_DOLLY_H

#include <cmath>
#include "coord.h"

#define LENGTH_TRAILER 1.16
#define LENGTH_TRUCK 0.22
#define DELTA_MAX 25

class Position
{
private:
    Coord positionTruck = Coord(0, 0);
    float angleTrailer = 0;

    Coord positionTruckOld = Coord(0, 0);
    float angleTrailerOld = 0;
    float phiDesiredOld = 0;
    uint32_t timeOld = 0;

public:
    Position() = default;

    float steeringAngle(Coord positionTrailer, Coord positionDesired)
    {
        Coord lookaheadDelta = positionDesired - positionTrailer;

        float angleTrailerError = atan2(lookaheadDelta.x, lookaheadDelta.y) - angleTrailer;

        float phiDesired = -3 * atan2(2 * LENGTH_TRAILER * sin(angleTrailerError),
                                      lookaheadDelta.magnitude());
        return constrain(phiDesired, -DELTA_MAX * PI / 180, DELTA_MAX * PI / 180);
    }
};

#endif // ARDUINO_POSITION_DOLLY_H
