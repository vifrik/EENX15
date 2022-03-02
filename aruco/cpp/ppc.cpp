#include <cmath>


int main(int argc, char **argv) {
    for (int i = 0; i < 1000; i++) {
        coords[i].x = i;
        coords[i].y = 5;
    }

    Coord pos = {15, 15};

    for (int i = 0; i < 1000; i++) {
        double d = calculateDistance(pos, coords[i]);
    }
}