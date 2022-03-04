//
// Created by vikto on 2022-03-04.
//

#ifndef ARDUINO_MAGNETIC_H
#define ARDUINO_MAGNETIC_H

#include "math.h"
#include "Wire.h"
#include "AS5600.h"

class Magnetic {
private:
    static AMS_5600 ams5600;
    static float phi0;
    
    static float convertRawAngleToRad(word angle) {
        return angle * 2 * PI / 4095;
    }

    static float angleToPhi(float angle, float phiStart) {
        return angle - phiStart;
    }

    static void convertAngle(float &angle) {
        if (angle > PI) {
            angle -= -2 * PI;
        } else if (angle < -PI) {
            angle += 2 * PI;
        }
    }

public:
    static void calibrate() {
        phi0 = convertRawAngleToRad(ams5600.getRawAngle());
    }

    static float readAngle() {
        float measuredAngle = convertRawAngleToRad(ams5600.getRawAngle());
        measuredAngle = angleToPhi(measuredAngle, phi0);
        convertAngle(measuredAngle);

        return measuredAngle;
    }
};


#endif //ARDUINO_MAGNETIC_H
