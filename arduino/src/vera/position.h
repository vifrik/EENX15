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
    Coord positionTrailerOld = Coord(0, 0);
    Coord positionTrailer = Coord(0, 0);
    Coord dPositionTrailer = Coord(0, 0);

    float thetaTruck = 0;
    float thetaTrailer = 0;

    float phiDesiredOld = 0;

public:
    Position() {}

    Coord updatePosition(float phi) {
        // get pos, read serial here

        Coord difference = CoordOperations::subtract(positionTrailer, positionTrailerOld);
        dPositionTrailer = CoordOperations::divide(difference, 2);
        positionTrailerOld = positionTrailer;

        // update trailer pos
        return positionTrailer;
    }

    float getPhiDesired(Coord positionDesired) {
        Coord difference = CoordOperations::subtract(positionTrailer, positionDesired);
        float thetaError = atan2(difference.y, difference.y);

        return -3 * atan2(2 * LENGTH_TRAILER * sin(thetaError),
                          CoordOperations::magnitude(difference));
    }

    float phiDesiredToDelta(float phiDesired) {
        float dPhiDes = phiDesired - phiDesiredOld;
        phiDesiredOld = phiDesired;
        float dThetaTrailer =
                (dPositionTrailer.x * sin(thetaTrailer) - dPositionTrailer.y * cos(thetaTrailer)) / LENGTH_TRAILER;
        float delta = -atan((dThetaTrailer - dPhiDes) * LENGTH_TRUCK / CoordOperations::magnitude(dPositionTrailer));

        return constrain(delta, -DELTA_MAX * PI / 180, DELTA_MAX * PI / 180);
    }
};

#endif //ARDUINO_POSITION_H
