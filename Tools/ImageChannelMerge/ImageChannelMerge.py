import cv2

b = cv2.imread("D:/workspace/BBearEditor/Tools/ImageChannelMerge/AO.png")
g = cv2.imread("D:/workspace/BBearEditor/Tools/ImageChannelMerge/Roughness.png")
r = cv2.imread("D:/workspace/BBearEditor/Tools/ImageChannelMerge/Metallic.png")

b = cv2.cvtColor(b, cv2.COLOR_RGB2GRAY)
g = cv2.cvtColor(g, cv2.COLOR_RGB2GRAY)
r = cv2.cvtColor(r, cv2.COLOR_RGB2GRAY)

merged = cv2.merge([b, g, r])
cv2.imwrite("D:/workspace/BBearEditor/Tools/ImageChannelMerge/merged.png", merged)