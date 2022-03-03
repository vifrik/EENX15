#include <cmath>
#include <iostream>
#include "purePursuitController.h"


int main(int argc, char **argv) {
    std::vector<Coord> path;

    // generate path
    for (int i = 0; i < 1000; i++) {
        Coord coord = Coord(i/100.0, sin((i/100.0)*500.0*M_PI/180.0));
        path.push_back(coord);
    }

    purePursuitController ppc = purePursuitController(path, 1.0);
    Coord target1 = ppc.getTarget(Coord(0,0));
    Coord target2 = ppc.getTarget(Coord(target1.x,target1.y));

    std::cout << target1.x << " " << target1.y << std::endl;
    std::cout << target2.x << " " << target2.y << std::endl;
}