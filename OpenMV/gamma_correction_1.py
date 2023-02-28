# Gamma Correction
#
# This example shows off gamma correction to make the image brighter. The gamma
# correction method can also fix contrast and brightness too.

import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()

green_thresholds = [23, 81, -128, -7, -128, 127] #порог зеленого цвета
black_thresholds = [0, 25, -128, 34, -128, 41] #порог черного цвета

x_size = 320
y_size = 240

y_k = 0.3
delta_x = 20

min_area = 800
min_pixels = 800

dir_turn = 0
'''
0 - backward
1 - right
2 - left
3 - no
'''
while(True):
    clock.tick()

    # Gamma, contrast, and brightness correction are applied to each color channel. The
    # values are scaled to the range per color channel per image type...
    img = sensor.snapshot().gamma_corr(gamma = 0.85, contrast = 1.3, brightness = 0.06)

    left_roi = (0, round(y_size*y_k), round(x_size/2-delta_x), round(y_size*(1-y_k)))
    right_roi = (round(x_size/2+delta_x), round(y_size*y_k), round(x_size/2-delta_x), round(y_size*(1-y_k)))

    img.draw_rectangle(0, round(y_size*y_k), round(x_size/2-delta_x), round(y_size*(1-y_k)), (255,255,0),2,False)
    img.draw_rectangle(round(x_size/2+delta_x), round(y_size*y_k), round(x_size/2-delta_x), round(y_size*(1-y_k)), (0,255,255),2,False)

    green_marker_left = img.find_blobs([green_thresholds], area_threshold = min_area, pixels_threshold = min_pixels, roi=left_roi)
    green_marker_right = img.find_blobs([green_thresholds], area_threshold = min_area, pixels_threshold = min_pixels, roi=right_roi)

    if(len(green_marker_left)>0):
        for gl in green_marker_left:
            img.draw_edges(gl.min_corners(), color=(255,0,0))
            img.draw_circle(gl.cx(), gl.cy(), 3, (255,0,0),2,True)
    if(len(green_marker_right)>0):
         for gr in green_marker_right:
             img.draw_edges(gr.min_corners(), color=(255,0,0))
             img.draw_circle(gr.cx(), gl.cy(), 3, (255,0,0),2,True)
    if(len(green_marker_left)>0)and(len(green_marker_right)>0):
        dir_turn = 0
        img.draw_string(10, 10, "backward",(255,0,0),2)
    elif(len(green_marker_left)==0)and(len(green_marker_right)>0):
        dir_turn = 1
        img.draw_string(10, 10, "right",(255,0,0),2)
    elif(len(green_marker_left)>0)and(len(green_marker_right)==0):
        dir_turn = 2
        img.draw_string(10, 10, "left",(255,0,0),2)
    elif(len(green_marker_left)==0)and(len(green_marker_right)==0):
        dir_turn = 3
        img.draw_string(10, 10, "no",(255,0,0),2)


