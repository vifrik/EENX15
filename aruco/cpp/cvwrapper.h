//
// Created by Viktor on 2022-02-19.
//

#ifndef ARUCOCPP_CVWRAPPER_H
#define ARUCOCPP_CVWRAPPER_H

#include "defines.h"
#include <vector>
#include <iostream>
#include <opencv2/aruco.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

class cvwrapper {
private:
    VideoCapture videoCapture;
    Ptr<aruco::Dictionary> dictionary;
    Ptr<aruco::DetectorParameters> parameters = aruco::DetectorParameters::create();
    std::vector<std::vector<Point2f>> markerCorners, rejectedCandidates;

    // Raspberry pi camera
    // cameraMatrix : [554.8647168500464, 0, 304.1026427291827;
    // 0, 539.8427898383699, 285.1912733996161;
    // 0, 0, 1]
    //distCoeffs : [0.2255440298514118, -0.7484333071771456, -0.009964906964899217, -0.009827800017520698, -0.9339961719359797]

#ifdef MACBOOK
    Mat cameraMatrix = (Mat1f(3, 3) << 971.35220497224020, 0.0, 646.72249200823774, 0.0,
            970.01993978791074, 357.74170661497186, 0.0, 0.0, 1.0);
    Mat distCoeffs = (Mat1f(5, 1) << -0.036721326921445439, 0.60501514643400323,
            0.0027156799005218536, 0.0040208404882518750,
            -1.8414740894267736);
#endif

#ifdef JETSON
    /*
Calibrations Matrix:
[[1.22188438e+03 0.00000000e+00 6.37897696e+02]
 [0.00000000e+00 1.21825310e+03 2.85477181e+02]
 [0.00000000e+00 0.00000000e+00 1.00000000e+00]]
Distortion:  [[ 0.14843644 -0.09067028 -0.01228633 -0.00743626 -1.11185888]]
0.42998493880855576
     */
    Mat cameraMatrix = (Mat1f(3, 3) << 1222, 0, 638, 0.0,
            1218, 285, 0.0, 0.0, 1.0);
    Mat distCoeffs = (Mat1f(5, 1)
            << 0.14843644, -0.09067028, -0.01228633, -0.00743626, -1.11185888);
#endif

    void startCapture(int index, int apiPreference = CAP_ANY);

    void detect(InputOutputArray &frame);

protected:
public:
    struct rtvecs {
        std::vector<Vec3d> rvecs;
        std::vector<Vec3d> tvecs;
    } rtVecs;

    cvwrapper(int index, int apiPreference);

    void release();

    bool readFrame(InputOutputArray &frame);

    int numberOfMarkers();

    void drawBoundingBoxes(InputOutputArray &frame, Scalar color);

    void drawIds(InputOutputArray &frame, Scalar color);

    void drawText(InputOutputArray &frame, std::string text, int x, int y);

    void drawBox(InputOutputArray &frame, Scalar color, int x, int y, int w, int h, int thickness = 2);

    void drawCircle(InputOutputArray &frame, Scalar color, int x, int y, int r);

    void show(InputOutputArray &frame);

    rtvecs getLocation();

    std::vector<int> markerIds;
};


#endif //ARUCOCPP_CVWRAPPER_H
