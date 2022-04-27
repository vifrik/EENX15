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
    std::vector<cv::Ptr<aruco::GridBoard>> gridboards;

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
image_width: 1280
image_height: 720
camera_name: camerav2_1280x720
camera_matrix:
  rows: 3
  cols: 3
  data: [1276.704618338571, 0, 634.8876509199106, 0, 1274.342831275509, 379.8318028940378, 0, 0, 1]
distortion_model: plumb_bob
distortion_coefficients:
  rows: 1
  cols: 5
  data: [0.1465167016954302, -0.2847343180128725, 0.00134017721235817, -0.004309553450829512, 0]
rectification_matrix:
  rows: 3
  cols: 3
  data: [1, 0, 0, 0, 1, 0, 0, 0, 1]
projection_matrix:
  rows: 3
  cols: 4
  data: [1300.127197265625, 0, 630.215390285608, 0, 0, 1300.670166015625, 380.1702884455881, 0, 0, 0, 1, 0]

     */
    Mat cameraMatrix = (Mat1f(3, 3) << 1276.704618338571, 0, 634.8876509199106, 0, 1274.342831275509, 379.8318028940378, 0, 0, 1);
    Mat distCoeffs = (Mat1f(5, 1) << 0.1465167016954302, -0.2847343180128725, 0.00134017721235817, -0.004309553450829512, 0);
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

    int numberOfGridboards();

    void drawBoundingBoxes(InputOutputArray &frame, Scalar color);

    void drawIds(InputOutputArray &frame, Scalar color);

    void drawText(InputOutputArray &frame, std::string text, int x, int y);

    void drawBox(InputOutputArray &frame, Scalar color, int x, int y, int w, int h, int thickness = 2);

    void drawCircle(InputOutputArray &frame, Scalar color, int x, int y, int r);

    void show(InputOutputArray &frame);

    rtvecs getLocation();

    std::vector<int> markerIds;

    std::vector<int> gridboardIds;
};


#endif //ARUCOCPP_CVWRAPPER_H
