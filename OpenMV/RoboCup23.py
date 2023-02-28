# Gamma Correction
#
# This example shows off gamma correction to make the image brighter. The gamma
# correction method can also fix contrast and brightness too.

import sensor, image, time, math
from pyb import UART

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()

uart = UART(3, 115200)

green_thresholds = [23, 81, -128, -7, -128, 127] #порог зеленого цвета
black_thresholds = [0, 25, -128, 34, -128, 41] #порог черного цвета

line_count = 30



x_size = 320
y_size = 240

y_k = 0.6
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

line_rois = []
weight_sum = 0
for i in range(line_count-1,-1,-1):
    w =  1
    line_rois.append((0, round(i*y_size/line_count), x_size, round(y_size/line_count),w))
    weight_sum+=w
while(True):
    clock.tick()

    # Gamma, contrast, and brightness correction are applied to each color channel. The
    # values are scaled to the range per color channel per image type...
    img = sensor.snapshot().gamma_corr(gamma = 0.85, contrast = 1.3, brightness = 0.06)

    left_roi = (0, round(y_size*y_k), round(x_size/2-delta_x), round(y_size*(1-y_k)))
    right_roi = (round(x_size/2+delta_x), round(y_size*y_k), round(x_size/2-delta_x), round(y_size*(1-y_k)))
    #img.draw_rectangle(0, round(y_size*y_k), round(x_size/2-delta_x), round(y_size*(1-y_k)), (255,255,0),2,False)
    #img.draw_rectangle(round(x_size/2+delta_x), round(y_size*y_k), round(x_size/2-delta_x), round(y_size*(1-y_k)), (0,255,255),2,False)

    green_marker_left = img.find_blobs([green_thresholds], area_threshold = min_area, pixels_threshold = min_pixels, roi=left_roi)
    green_marker_right = img.find_blobs([green_thresholds], area_threshold = min_area, pixels_threshold = min_pixels, roi=right_roi)





    '''if(len(green_marker_left)>0):
        for gl in green_marker_left:
            img.draw_edges(gl.min_corners(), color=(255,0,0))
            img.draw_circle(gl.cx(), gl.cy(), 3, (255,0,0),2,True)
    if(len(green_marker_right)>0):
         for gr in green_marker_right:
             img.draw_edges(gr.min_corners(), color=(255,0,0))
             img.draw_circle(gr.cx(), gl.cy(), 3, (255,0,0),2,True)'''
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

    centroid_sum = 0

    line_blobs = []

    for r in line_rois:
        blobs = img.find_blobs([black_thresholds], roi=r[0:4], merge=True) # r[0:4] is roi tuple.

        if blobs:
            # Find the blob with the most pixels.
            largest_blob = max(blobs, key=lambda b: b.pixels())

            # Draw a rect around the blob.
            img.draw_rectangle(largest_blob.rect())
            img.draw_cross(largest_blob.cx(),
                           largest_blob.cy())

            line_blobs.append([largest_blob.cx(), largest_blob.cy()])


            centroid_sum += largest_blob.cx() * r[4] # r[4] is the roi weight.

    center_pos = (centroid_sum / weight_sum) # Determine center of line.


    deflection_angle = 0


    deflection_angle = -math.atan((center_pos-round(x_size/2))/round(y_size/2))

    # Convert angle in radians to degrees.
    deflection_angle = round(math.degrees(deflection_angle))

    angle = 0
    if(len(line_blobs)>2):
        i = 1

        while(     (((line_blobs[i][0]-line_blobs[0][0])*(line_blobs[i+1][0]-line_blobs[i][0]))>0)   and    (i<len(line_blobs)-2)):
             i += 1
        i -= 1
        img.draw_line(line_blobs[0][0], line_blobs[0][1],line_blobs[i][0],line_blobs[i][1],(255,0,0),2)
        try:
            angle = round(math.degrees(math.atan((line_blobs[i][0]-line_blobs[0][0])/(line_blobs[i][1]-line_blobs[0][1]))))
        except:
            angle = 0


    # Now you have an angle telling you how much to turn the robot by which
    # incorporates the part of the line nearest to the robot and parts of
    # the line farther away from the robot for a better prediction.
    print(angle)

    if(center_pos==0):
        deflection_angle=0
    img.draw_string(10, 30, str(angle),(255,0,0),2)
    uart.writechar(255)
    time.sleep_ms(1)

                                 # to the IDE. The FPS should increase once disconnected.
    uart.writechar(angle+60)
    time.sleep_ms(1)

    uart.writechar(dir_turn)
    time.sleep_ms(1)







