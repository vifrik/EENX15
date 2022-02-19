//
// Created by Viktor on 2022-02-19.
//

#include "cvwrapper.h"
#include <iostream>

cvwrapper::cvwrapper(int index, int apiPreference) {
    startCapture(index, apiPreference);

    dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
}

void cvwrapper::startCapture(int index, int apiPreference) {
    videoCapture.open(index, apiPreference);

    if (!videoCapture.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        exit(0);
    }
}

void cvwrapper::release() {
    videoCapture.release();
    destroyAllWindows();
}

bool cvwrapper::readFrame() {
    videoCapture.read(frame);
    if (frame.empty()) {
        std::cerr << "ERROR! blank frame grabbed\n";
        return false;
    }
    return true;
}

void cvwrapper::detect() {
    aruco::detectMarkers(frame, dictionary, markerCorners, markerIds, parameters,
                         rejectedCandidates);
}

void cvwrapper::drawBoundingBoxes(Scalar color) {
    for (int i = 0; i < markerIds.size(); i++) {
        std::vector<Point2f> corners = markerCorners[i]; // Get current corners of tag with id, id

        // Draw bounding box around tag
        for (int j = 0; j < 4; j++) {
            line(frame, corners[j], corners[(j + 1) % 4], color, 2, LINE_8);
        }
    }
}

void cvwrapper::drawTexts(Scalar color) {
    for (int i = 0; i < markerIds.size(); i++) {
        std::vector<Point2f> corners = markerCorners[i]; // Get current corners of tag with id, id

        // Write id text at top left corner of tag
        putText(frame, std::to_string(markerIds[i]), Point(corners[0].x,corners[0].y - 15),
                FONT_HERSHEY_DUPLEX,1, color, 2, false);
    }
}

cvwrapper::rtvecs cvwrapper::getLocation() {
    // Camera calibration, use executable calibrate to calibrate
    Mat cameraMatrix = (Mat1f(3, 3) << 971.35220497224020, 0.0, 646.72249200823774, 0.0,
            970.01993978791074, 357.74170661497186, 0.0, 0.0, 1.0);
    Mat distCoeffs = (Mat1f(5, 1) << -0.036721326921445439, 0.60501514643400323,
            0.0027156799005218536, 0.0040208404882518750,
            -1.8414740894267736);

    aruco::estimatePoseSingleMarkers(markerCorners, 0.15, cameraMatrix,
                                     distCoeffs, rtVecs.rvecs, rtVecs.tvecs);
    return cvwrapper::rtvecs();
}

void cvwrapper::show() {
    imshow("Live preview", frame);
}



