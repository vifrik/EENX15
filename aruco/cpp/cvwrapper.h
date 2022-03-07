//
// Created by Viktor on 2022-02-19.
//

#ifndef ARUCOCPP_CVWRAPPER_H
#define ARUCOCPP_CVWRAPPER_H

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

    Mat cameraMatrix = (Mat1f(3, 3) << 971.35220497224020, 0.0, 646.72249200823774, 0.0,
            970.01993978791074, 357.74170661497186, 0.0, 0.0, 1.0);
    Mat distCoeffs = (Mat1f(5, 1) << -0.036721326921445439, 0.60501514643400323,
            0.0027156799005218536, 0.0040208404882518750,
            -1.8414740894267736);

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
