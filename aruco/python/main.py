#!/usr/bin/env python

from __future__ import print_function  # Python 2/3 compatibility
#import cv2  # Import the OpenCV library
from cv2.cv2 import *
import numpy as np  # Import Numpy library
from cv2.aruco import estimatePoseSingleMarkers
import socket
import random

desired_aruco_dictionary = cv2.aruco.DICT_4X4_50
camera_calibration_parameters_filename = 'calibration_chessboard.yaml'

HOST = '127.0.0.1'                     # Symbolic name meaning all available interfaces
PORT = 12000              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

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

        if ids is not None:
            ids = ids.flatten() # flatten the ArUco IDs list

            # Loop over the detected ArUco corners
            for (marker_corner, marker_id) in zip(corners, ids):
                corners = marker_corner.reshape((4, 2)) # extract marker corners
                (tl, tr, br, bl) = corners # turn corners into coordinate tuples

                # convert from float tuples to int tuples
                tr = tuple(map(int, tr))
                br = tuple(map(int, br))
                bl = tuple(map(int, bl))
                tl = tuple(map(int, tl))

                # Draw the bounding box of the ArUco detection
                cv2.line(frame, tl, tr, (0, 255, 0), 2)
                cv2.line(frame, tr, br, (0, 255, 0), 2)
                cv2.line(frame, br, bl, (0, 255, 0), 2)
                cv2.line(frame, bl, tl, (0, 255, 0), 2)

                # draw id text
                cv2.putText(frame, str(marker_id), (tl[0], tl[1] - 15),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

                rvecs, tvecs, obj_points = estimatePoseSingleMarkers(marker_corner, 0.3, camera_matrix, dist_coeffs)
                cv2.aruco.drawAxis(frame, camera_matrix, dist_coeffs, rvecs[marker_id], tvecs[marker_id], 0.05)

                data = (str(tvecs[marker_id][0][0]) + ' '
                        + str(tvecs[marker_id][0][1]) + ' '
                        + str(tvecs[marker_id][0][2]) + ' '
                        + str(rvecs[marker_id][0][0]) + ' '
                        + str(rvecs[marker_id][0][1]) + ' '
                        + str(rvecs[marker_id][0][2]) + ' '
                        + '\n').encode()
                s.send(data)

        cv2.imshow('frame', frame) # display frame

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release() # close stream
    cv2.destroyAllWindows()
    s.close()


if __name__ == '__main__':
    main()