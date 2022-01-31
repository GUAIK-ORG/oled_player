import cv2 as cv
from PIL import Image
import numpy as np
import serial
import time


def convert2bitmap(img):
    # 将图像转换成灰度图
    black_img = img.convert("L")
    bdata_list = list(black_img.getdata())
    threshold = 128
    # 将灰度图转换为单色位图，值小于128的转换为0，其他的转换为1
    bvalue_list = [0 if i < threshold else 1 for i in bdata_list]
    arr = np.array(bvalue_list)
    # 将数组数据转换成64行128列的矩阵
    arr = arr.reshape((64, 128))
    data = []
    # 转换成OLED0561可显示的数据（左上到右下，高位在下边）
    for m in range(0, 8):
        for i in range(0, 128):
            col = arr[:, i]
            a = 0
            for j in range(0, 8):
                a |= col[m*8+j] << j
            data.append(a)
    return data


def worker():
    # 初始化串口（串口号需要根据实际值进行修改）
    s = serial.Serial("COM10", 115200)

    # 打开默认摄像头设备
    capture = cv.VideoCapture(0)

    while True:
        # 读取摄像头一帧数据
        ret, frame = capture.read()
        # 将图像设置为128 * 64大小（OLED屏幕为128 * 64）
        frame = cv.resize(frame, (128, 64))
        # 将帧数据转换成图像数据
        img = Image.fromarray(frame)
        # 将图像数据转换成OLED
        bitmap = convert2bitmap(img)
        # 将数据写入串口
        s.write(bitmap)
        # 这里加一些延迟，不然OLED屏幕闪的厉害
        time.sleep(0.3)
        cv.imshow("frame", frame)
        # 延迟1ms后切换到下一帧图像
        cv.waitKey(1)


if __name__ == "__main__":
    worker()
