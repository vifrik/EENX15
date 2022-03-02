//
// Created by Viktor on 2022-03-02.
//

#ifndef ARUCOCPP_PUREPURSUITCONTROLLER_H
#define ARUCOCPP_PUREPURSUITCONTROLLER_H

#include <cmath>
#include <utility>
#include <vector>
#include <queue>

#define MAX_STEPS 100

struct Coord {
    double x;
    double y;

    Coord(double x, double y) : x(x), y(y) {}
};

class purePursuitController {
private:
    std::vector<Coord> &path;
    std::vector<int> targetCandidates;
    double lookahead;
    int index = 0;
    bool fromNegative = false;

public:
    purePursuitController(std::vector<Coord> &path, double lookahead) : path(path), lookahead(lookahead) {}

    static double calculateDistance(Coord c1, Coord c2) {
        return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
    }

    Coord getTarget(Coord position) {
        for (int i = index; i < path.size(); i++) {
            Coord current = path.at(i);
            double distance = calculateDistance(position, current);

            if (fromNegative && distance - lookahead >= 0) {
                fromNegative = false;
                targetCandidates.push_back(i);
            } else if (distance - lookahead < 0) {
                fromNegative = true;
            }

            // To save time, break after 100 iterations if we found a candidate
            if (i > index + MAX_STEPS && !targetCandidates.empty()) {
                break;
            }

            if (i == path.size() - 1) {
                return path.at(path.size() - 1);
            }
        }

        if (targetCandidates.empty()) {
            return path.at(index);
        }

        index = targetCandidates.at(targetCandidates.size() - 1);
        return path.at(index);
    }
};


#endif //ARUCOCPP_PUREPURSUITCONTROLLER_H
