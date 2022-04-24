//
// Created by Viktor on 2022-02-19.
//

#include <opencv2/opencv.hpp>
#include "cvwrapper.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

cvwrapper::cvwrapper(int index, int apiPreference) {
    startCapture(index, apiPreference);
    dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
}

std::string
gstreamer_pipeline(int capture_width, int capture_height, int display_width, int display_height, int framerate,
                   int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) +
           ", height=(int)" +
           std::to_string(capture_height) + ", framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" +
           std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) +
           ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

void cvwrapper::startCapture(int index, int apiPreference) {
#ifdef JETSON
    printf("JETSON\n");
    //står att den ska klara 60 fps i 1280x720, men den kör 30 fps då
    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 60 ; //https://forums.developer.nvidia.com/t/120-fps-mode-support-removed-for-imx219-sensor/174327/9
    int flip_method = 0 ;

    std::string pipeline = gstreamer_pipeline(capture_width,
                                              capture_height,
                                              display_width,
                                              display_height,
                                              framerate,
                                              flip_method);

    videoCapture.open(pipeline, cv::CAP_GSTREAMER);
#else
    printf("PC\n");
    videoCapture.open(index, apiPreference);
#endif

    if (!videoCapture.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        exit(0);
    }
}

void cvwrapper::release() {
    videoCapture.release();
    destroyAllWindows();
}

bool cvwrapper::readFrame(InputOutputArray &frame) {
    videoCapture.read(frame);
    if (frame.empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
        return false;
    }
    detect(frame);

    return true;
}

void cvwrapper::detect(InputOutputArray &frame) {
    aruco::detectMarkers(frame, dictionary, markerCorners, markerIds, parameters,
                         rejectedCandidates);
}

void cvwrapper::drawBoundingBoxes(InputOutputArray &frame, Scalar color) {
    aruco::drawDetectedMarkers(frame, markerCorners, noArray(), color);
    for (int i = 0; i < markerIds.size(); i++) {
        aruco::drawAxis(frame, cameraMatrix, distCoeffs, rtVecs.rvecs[i], rtVecs.tvecs[i], 0.05);
    }
}

void cvwrapper::drawIds(InputOutputArray &frame, Scalar color) {
    for (int i = 0; i < markerIds.size(); i++) {
        std::vector<Point2f> corners = markerCorners[i]; // Get current corners of tag with id, id

        // Write id text at top left corner of tag
        putText(frame, std::to_string(markerIds[i]), Point(corners[0].x, corners[0].y - 15),
                FONT_HERSHEY_DUPLEX, 1, color, 2, false);
    }
}

void cvwrapper::drawBox(InputOutputArray &frame, Scalar color, int x, int y, int w, int h, int thickness) {
    Point tl, tr, bl, br; // top-left, top-right, bottom-left, bottom-right
    tl = Point(x, y);
    tr = Point(x + w, y);
    bl = Point(x, y + h);
    br = Point(x + w, y + h);
    line(frame, tl, tr, color, thickness, LINE_8);
    line(frame, tr, br, color, thickness, LINE_8);
    line(frame, br, bl, color, thickness, LINE_8);
    line(frame, bl, tl, color, thickness, LINE_8);
}

void cvwrapper::drawCircle(InputOutputArray &frame, Scalar color, int x, int y, int r) {
    circle(frame, Point(x, y), r, color, FILLED, LINE_8);
}

void cvwrapper::drawText(InputOutputArray &frame, std::string text, int x, int y) {
    putText(frame, text, Point(x, y), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255), 2, false);
}

cvwrapper::rtvecs cvwrapper::getLocation() {
    // Marker is 14 cm
    aruco::estimatePoseSingleMarkers(markerCorners, 0.14, cameraMatrix,
                                     distCoeffs, rtVecs.rvecs, rtVecs.tvecs);
    return cvwrapper::rtVecs;
}

void cvwrapper::show(InputOutputArray &frame) {
    /*Mat temp;
    undistort(frame, temp, cameraMatrix, distCoeffs);
    imwrite("calibresult.png", temp);
    exit(1);*/
    imshow("Live preview", frame);
}

int cvwrapper::numberOfMarkers() {
    return markerIds.size();
}



