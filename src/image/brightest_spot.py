from PIL import Image, ImageGrab, ImageEnhance
import cv2
import numpy as np

# read image as grayscale


while True:
    image = np.array(ImageGrab.grab(bbox=(60,250,1500+60,700+250)))
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    (minVal, maxVal, minLoc, maxLoc) = cv2.minMaxLoc(blurred)
    cv2.circle(image, maxLoc, 5, (255, 0, 0), 2)
    cv2.imshow("parsed", image)
    cv2.waitKey(1)