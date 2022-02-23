//
// Created by Viktor on 2022-02-23.
//

#ifndef ARUCOCPP_MATH_H
#define ARUCOCPP_MATH_H

#include <opencv2/opencv.hpp>

using namespace cv;

namespace oMath {
    static Mat eulerAnglesToRotationMatrix(Vec3d &theta) {
        // rotation x
        Mat rotationalX = (Mat1d(3, 3) << 1, 0, 0,
                0, cos(theta[0]), -sin(theta[0]),
                0, sin(theta[0]), cos(theta[0])
        );

        // rotation y
        Mat rotationalY = (Mat1d(3, 3) << cos(theta[1]), 0, sin(theta[1]),
                0, 1, 0,
                -sin(theta[1]), 0, cos(theta[1])
        );

        // rotatio z
        Mat rotationalZ = (Mat1d(3, 3) << cos(theta[2]), -sin(theta[2]), 0,
                sin(theta[2]), cos(theta[2]), 0,
                0, 0, 1);

        // final rotational matrix
        Mat rotationalMatrix = rotationalZ * rotationalY * rotationalX;
        return rotationalMatrix;
    }

    Vec3d rotationMatrixToEulerAngles(const Mat3d & R) {
        float sy = sqrt(R.at<double>(0,0) * R.at<double>(0,0) + R.at<double>(1,0) * R.at<double>(1,0));
        bool singular = sy < 1e-6;

        float x, y, z;
        if (!singular) {
            x = atan2(R.at<double>(2,1) , R.at<double>(2,2));
            y = atan2(-R.at<double>(2,0), sy);
            z = atan2(R.at<double>(1,0), R.at<double>(0,0));
        } else {
            x = atan2(-R.at<double>(1,2), R.at<double>(1,1));
            y = atan2(-R.at<double>(2,0), sy);
            z = 0;
        }
        return Vec3d(x, y, z);
    }
}

#endif //ARUCOCPP_MATH_H
