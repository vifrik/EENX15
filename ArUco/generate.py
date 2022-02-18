import cv2
import numpy as np
from pathlib import Path

desired_aruco_dictionary = cv2.aruco.DICT_4X4_250

ARUCO_DICT = {
    "DICT_4X4_50": cv2.aruco.DICT_4X4_50,
    "DICT_5X5_50": cv2.aruco.DICT_5X5_50,
    "DICT_6X6_50": cv2.aruco.DICT_6X6_50,
    "DICT_7X7_50": cv2.aruco.DICT_7X7_50,
    "DICT_ARUCO_ORIGINAL": cv2.aruco.DICT_ARUCO_ORIGINAL
}


def main():
    for k, v in ARUCO_DICT.items():
        print(k, v)
        for marker_id in range(50):
            tag = np.zeros((300, 300, 1), dtype="uint8")
            cv2.aruco.drawMarker(cv2.aruco.Dictionary_get(v), marker_id, 300, tag, 1)
            Path(f"out/{k}").mkdir(parents=True, exist_ok=True)
            cv2.imwrite(f"out/{k}/{k}-{marker_id}.png", tag)


if __name__ == '__main__':
    main()
