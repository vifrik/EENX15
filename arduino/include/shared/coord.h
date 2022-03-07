//
// Created by vikto on 2022-03-04.
//

#ifndef ARDUINO_COORD_H
#define ARDUINO_COORD_H

struct Coord {
    float x;
    float y;

    Coord(float x, float y) : x(x), y(y) {};
};

class CoordOperations {
public:
    static Coord subtract(Coord first, Coord second) {
        return Coord(first.x - second.x, first.y - second.y);
    }

    static Coord divide(Coord first, float divisor) {
        return Coord(first.x / divisor, first.y / divisor);
    }

    static float magnitude(Coord coord) {
        return sqrt(pow(coord.x, 2) + pow(coord.y, 2));
    }
};

#endif //ARDUINO_COORD_H
