//
// Created by vikto on 2022-03-04.
//
#include <cmath>

#ifndef ARDUINO_COORD_H
#define ARDUINO_COORD_H

struct Coord {
    float x;
    float y;

    Coord(float x, float y) : x(x), y(y) {};

    float magnitude() {
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    Coord operator-(const Coord& o) {
        return Coord(x - o.x, y - o.y);
    }

    Coord operator/(const float d) {
        return Coord(x / d, y / d);
    }
};

#endif //ARDUINO_COORD_H
