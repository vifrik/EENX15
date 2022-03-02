//
// Created by Viktor on 2022-03-02.
//

#ifndef ARUCOCPP_PUREPC_H
#define ARUCOCPP_PUREPC_H

#include <cmath>
#include <vector>

#define PATH_SIZE 1000

struct Coord {
    double x;
    double y;
};

class purePC {
private:
    std::vector<Coord> *path;

    double calculateDistance(Coord c1, Coord c2) {
        return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
    }

public:
    purePC(std::vector *inputPath) {
        path = inputPath;
    }
    ~purePC() {
        free(path);
        path = NULL;
    }

    Coord getTarget(Coord position) {
        for (int i = 0; i < PATH_SIZE; i++) {
            double d = calculateDistance(position, );
        }
    }
};


#endif //ARUCOCPP_PUREPC_H
