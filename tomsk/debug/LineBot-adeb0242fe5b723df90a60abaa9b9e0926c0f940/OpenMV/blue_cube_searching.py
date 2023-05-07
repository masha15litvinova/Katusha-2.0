# Untitled - By: masha - Вт апр 4 2023

import sensor, image, time
import pyb

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)

clock = time.clock()

x_size = 320
y_size = 240

blue = (0, 64, -24, 127, -128, -5)
white = (55, 100, -12, 127, -128, 127)
roi_cube = (round(x_size*0.28),  round(y_size*0.3), round(x_size*0.44),round(y_size*0.5))
uart = pyb.UART(3, 9600)
blue_led = pyb.LED(3)
red_led = pyb.LED(1)

while(True):
    clock.tick()
    img = sensor.snapshot()
    img.gaussian(3)
    img.draw_rectangle(roi_cube[0], roi_cube[1], roi_cube[2], roi_cube[3], (255,255,0), 2)

    cubes = img.find_blobs([blue],invert=False, roi = roi_cube, area_threshold = 1200, pixels_threshold = 1200)
    white_area = img.find_blobs([white],invert=False, roi = roi_cube, pixels_threshold = 5000)
    for cube in cubes:
        if(len(white_area)!=0):
            img.draw_rectangle(cube.rect()[0], cube.rect()[1],cube.rect()[2],cube.rect()[3],(255,0,0),2)


    if(len(cubes)*len(white_area)!=0):
        if(uart.any()):
            uart.writechar(1)
            #red_led .on()
        blue_led .on()
    else:
        if(uart.any()):
            uart.writechar(0)
            #red_led .on()
        blue_led .off()
