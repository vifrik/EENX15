//
// Created by Viktor on 2022-02-19.
//

#ifndef ARUCOCPP_CVWRAPPER_H
#define ARUCOCPP_CVWRAPPER_H

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
    Mat frame;

    void startCapture(int index, int apiPreference = CAP_ANY);
protected:
public:
    struct rtvecs {
        std::vector<Vec3d> rvecs;
        std::vector<Vec3d> tvecs;
    } rtVecs;

    cvwrapper(int index, int apiPreference);
    void release();
    bool readFrame();
    void detect();
    int numberOfMarkers();
    void drawBoundingBoxes(Scalar color);
    void drawTexts(Scalar color);
    void drawText(std::string text, int x, int y);
    void show();
    rtvecs getLocation();

    std::vector<int> markerIds;
};


#endif //ARUCOCPP_CVWRAPPER_H
