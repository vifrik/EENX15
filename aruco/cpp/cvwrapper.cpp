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
    for (int i = 0; i < markerIds.size(); i++) {
        std::vector<Point2f> corners = markerCorners[i]; // Get current corners of tag with id, id

        // Draw bounding box around tag
        for (int j = 0; j < 4; j++) {
            line(frame, corners[j], corners[(j + 1) % 4], color, 2, LINE_8);
        }

        aruco::drawAxis(frame, cameraMatrix, distCoeffs, rtVecs.rvecs[i], rtVecs.tvecs[i], 0.05);
    }
}

void cvwrapper::drawIds(InputOutputArray &frame, Scalar color) {
    for (int i = 0; i < markerIds.size(); i++) {
        std::vector<Point2f> corners = markerCorners[i]; // Get current corners of tag with id, id

        // Write id text at top left corner of tag
        putText(frame, std::to_string(markerIds[i]), Point(corners[0].x,corners[0].y - 15),
                FONT_HERSHEY_DUPLEX,1, color, 2, false);
    }
}

void cvwrapper::drawBox(InputOutputArray &frame, Scalar color, int x, int y, int w, int h, int thickness) {
    Point tl, tr, bl, br; // top-left, top-right, bottom-left, bottom-right
    tl = Point(x, y);
    tr = Point(x+w, y);
    bl = Point(x, y+h);
    br = Point(x+w, y+h);
    line(frame, tl, tr, color, thickness, LINE_8);
    line(frame, tr, br, color, thickness, LINE_8);
    line(frame, br, bl, color, thickness, LINE_8);
    line(frame, bl, tl, color, thickness, LINE_8);
}

void cvwrapper::drawCircle(InputOutputArray &frame, Scalar color, int x, int y, int r) {
    circle( frame, Point(x, y),r, color, FILLED, LINE_8 );
}

void cvwrapper::drawText(InputOutputArray &frame, std::string text, int x, int y) {
    putText(frame, text, Point(x, y), FONT_HERSHEY_DUPLEX,1, Scalar (0,0,255), 2, false);
}

cvwrapper::rtvecs cvwrapper::getLocation() {
    // Marker is 14 cm
    aruco::estimatePoseSingleMarkers(markerCorners, 0.14, cameraMatrix,
                                     distCoeffs, rtVecs.rvecs, rtVecs.tvecs);
    return cvwrapper::rtVecs;
}

void cvwrapper::show(InputOutputArray &frame) {
    imshow("Live preview", frame);
}

int cvwrapper::numberOfMarkers() {
    return markerIds.size();
}



