import cv2
import numpy as np

image = cv2.imread("D:/workspace/BBearEditor/Tools/ImageProcessing/4.png")
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
result = np.zeros_like(image)
result[:, :, 0] = gray
result[:, :, 1] = gray
result[:, :, 2] = gray

cv2.imwrite("D:/workspace/BBearEditor/Tools/ImageProcessing/5.png", result)