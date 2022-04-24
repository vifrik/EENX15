#!/usr/bin/env python

from __future__ import print_function
import cv2
import numpy as np

desired_aruco_dictionary = cv2.aruco.DICT_4X4_50
camera_calibration_parameters_filename = 'calibration_chessboard.yaml'

def main():
    # load ArUco dictionary
    aruco_dictionary = cv2.aruco.Dictionary_get(desired_aruco_dictionary)
    aruco_parameters = cv2.aruco.DetectorParameters_create()

    cv_file = cv2.FileStorage(camera_calibration_parameters_filename, cv2.FILE_STORAGE_READ)
    camera_matrix = cv_file.getNode('K').mat()
    dist_coeffs = cv_file.getNode('D').mat()
    cv_file.release()

    cap = cv2.VideoCapture(0) # begin video capture

    while (True):
        ret, frame = cap.read() # read frame

        # detect ArUco markers in frame
        (corners, ids, rejected) = cv2.aruco.detectMarkers(frame, aruco_dictionary, parameters=aruco_parameters)

        if len(corners) == 0:
            continue

        rvec, tvec, _ = cv2.aruco.estimatePoseSingleMarkers(corners, 0.14, camera_matrix, dist_coeffs)

        R = cv2.Rodrigues(rvec)[0]
        R_T = R.T
        T = tvec[0].T

        print(T)

        xyz = np.dot(R_T, - T)
        #print(xyz)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release() # close stream

if __name__ == '__main__':
    main()