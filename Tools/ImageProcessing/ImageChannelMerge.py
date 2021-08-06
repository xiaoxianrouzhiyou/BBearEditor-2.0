import cv2

# Three RGB images take one channel respectively to synthesize one RGB image
def Merge0():
    b = cv2.imread("D:/workspace/BBearEditor/Tools/ImageProcessing/1.png")
    g = cv2.imread("D:/workspace/BBearEditor/Tools/ImageProcessing/2.png")
    r = cv2.imread("D:/workspace/BBearEditor/Tools/ImageProcessing/3.png")

    b = cv2.cvtColor(b, cv2.COLOR_RGB2GRAY)
    g = cv2.cvtColor(g, cv2.COLOR_RGB2GRAY)
    r = cv2.cvtColor(r, cv2.COLOR_RGB2GRAY)

    merged = cv2.merge([b, g, r])
    cv2.imwrite("D:/workspace/BBearEditor/Tools/ImageProcessing/merged.png", merged)
    return

Merge0()