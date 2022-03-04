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

class Position {
private:
    Coord positionTruck = Coord(0, 0);
    float angleTrailer = 0;

    Coord positionTruckOld = Coord(0, 0);
    float angleTrailerOld = 0;
    float phiDesiredOld = 0;

public:
    Position() {}

    Coord getPositionTrailer(float phi) {
        positionTruck = Coord(0, 0); // get these two from serial comms
        float angleTruck = 0;

        Coord positionTrailer = Coord(positionTruck.x + cos(phi) * LENGTH_TRAILER,
                                      positionTruck.y + sin(phi) * LENGTH_TRAILER);
        angleTrailer = angleTruck + PI + phi;

        return positionTrailer;
    }

    float steeringAngle(Coord positionTrailer, Coord positionDesired) {
        Coord lookaheadDelta = CoordOperations::subtract(positionDesired, positionTrailer);

        float angleTrailerError = atan2(lookaheadDelta.x, lookaheadDelta.y) - angleTrailer;

        float phiDesired = -3 * atan2(2 * LENGTH_TRAILER * sin(angleTrailerError),
                                      CoordOperations::magnitude(lookaheadDelta));

        float d_angleTrailer = (angleTrailer - angleTrailerOld) / 2.0f;
        float d_phiDesired = (phiDesired - phiDesiredOld) / 2.0f;
        Coord positionTruckDelta = CoordOperations::subtract(positionTruck, positionTruckOld);
        Coord d_positionTruck = CoordOperations::divide(positionTruckDelta, 2.0f);
        float velocityTruck = CoordOperations::magnitude(d_positionTruck);

        angleTrailerOld = angleTrailer;
        phiDesiredOld = phiDesired;

        float delta = -atan2((d_angleTrailer - d_phiDesired) * LENGTH_TRUCK, velocityTruck);
        return constrain(delta, -DELTA_MAX * PI / 180, DELTA_MAX * PI / 180);
    }
};

#endif // ARDUINO_POSITION_H
