# Gamma Correction
#
# This example shows off gamma correction to make the image brighter. The gamma
# correction method can also fix contrast and brightness too.

import sensor, image, time, math, utime
from pyb import UART

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()

uart = UART(3, 115200)
last_clock = 0

green_thresholds = [23, 81, -128, -7, -128, 127] #порог зеленого цвета
black_thresholds = [0, 22, -13, 7, -128, 127] #порог черного цвета
yellow_thresholds = [0, 100, -128, 127, -128, 67]
line_count = 12


x_size = 160
y_size = 120

y_k = 0.4
delta_x = 20
ky_regression = 0.5
kx_regression =0.1

min_area = 100
min_pixels = 100

dir_turn = 0
'''
0 - backward
1 - right
2 - left
3 - no
'''

line_rois = []
center_blob_pos = [round(x_size/2), y_size]
angle = 0
perp_length = 0
delta = 10

transmitted_val = 0
perp_coeff=0.01
max_transmitted_val = round(x_size/2)*perp_coeff+90+1


weight_sum = 0

def map( x,  in_min,  in_max,  out_min,  out_max):

    return (round((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)-1)




for i in range(line_count-1,-1,-1):
    w =  1
    line_rois.append((0, round(i*y_size/line_count), x_size, round(y_size/line_count),w))
    weight_sum+=w
while(True):
    clock.tick()

    # Gamma, contrast, and brightness correction are applied to each color channel. The
    # values are scaled to the range per color channel per image type...
    img = sensor.snapshot()#.gamma_corr(gamma = 0.85, contrast = 1.3, brightness = 0.06)
    img.mean(4)
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
    count_line_blobs = 0

    for r in line_rois:
        blobs = img.find_blobs([black_thresholds], roi=r[0:4], merge=True, area_threshold=min_area, pixels_threshold=min_pixels) # r[0:4] is roi tuple.

        if blobs:
            # Find the blob with the most pixels.
            largest_blob = min(blobs, key=lambda b: (b.cx()-center_blob_pos[0])**2+(b.cy()-center_blob_pos[1])**2)
            center_blob_pos [0]= largest_blob.cx()
            center_blob_pos [1]= largest_blob.cy()

            # Draw a rect around the blob.
            img.draw_rectangle(largest_blob.rect())
            img.draw_circle(largest_blob.cx(),
                           largest_blob.cy(),5,(255, 255, 0),3,True)

            #img.draw_string(largest_blob.cx()+7, largest_blob.cy(), str(count_line_blobs),(255,0,0), 2)

            line_blobs.append([largest_blob.cx(), largest_blob.cy()])


            centroid_sum += largest_blob.cx() * r[4] # r[4] is the roi weight.
            count_line_blobs+=1

    regression_roi = (round(x_size*kx_regression), round(y_size*ky_regression), round(x_size*(1-2*kx_regression)), round(y_size*(1-ky_regression)))

    new_img = img
    new_img.binary([yellow_thresholds])
    line_get = new_img.get_regression([(0,0)], robust=False,roi=regression_roi, x_stride=5, y_stride = 5)

    if (line_get):
        img.draw_line(line_get[0],line_get[1],line_get[2],line_get[3],255,1)
    img.draw_rectangle(regression_roi[0],regression_roi[1],regression_roi[2], regression_roi[3],127,1)

    # The 80 is from half the X res, the 60 is from half the Y res. The
    # equation below is just computing the angle of a triangle where the
    # opposite side of the triangle is the deviation of the center position
    # from the center and the adjacent side is half the Y res. This limits
    # the angle output to around -45 to 45. (It's not quite -45 and 45).

    if(line_get):
        angle = line_get.theta()
        if(angle>90):
            angle = angle-180
        angle = -angle
    else:
        angle = 0
    '''if(len(line_blobs)>2):
        i = 1

        while(     ((((line_blobs[i][0]-line_blobs[0][0])*(line_blobs[i+1][0]-line_blobs[i][0]))>0)or((line_blobs[i+1][0]==line_blobs[i][0])))   and    (i<len(line_blobs)-2)):
             i += 1
        i -= 1
        img.draw_line(line_blobs[0][0], line_blobs[0][1],line_blobs[i][0],line_blobs[i][1],(255,0,0),2)
        if(i>=1):
            perp_length=round(abs( (line_blobs[0][1]* line_blobs[i][0]-line_blobs[i][1]* line_blobs[0][0])/(math.sqrt((line_blobs[i][1]-line_blobs[0][1])**2+(line_blobs[i][0]-line_blobs[0][0])**2))))
        else:
            perp_length = 0
        try:
            angle = round(math.degrees(math.atan((line_blobs[i][0]-line_blobs[0][0])/(line_blobs[i][1]-line_blobs[0][1]))))
        except:
            angle = 0'''


    # Now you have an angle telling you how much to turn the robot by which
    # incorporates the part of the line nearest to the robot and parts of
    # the line farther away from the robot for a better prediction.



    transmitted_val = map(angle, -91, 91, 0, 255)#map(angle, -max_transmitted_val, max_transmitted_val, 0, 254) #+math.copysign(abs(round(perp_coeff*(-perp_length+round(x_size/2)))),angle)'''
    #print(transmitted_val)

    img.draw_string(10, 30, str(angle),(255,0,0),2)

    #print(angle)


    #uart.writechar(255)

    #uart.writechar(transmitted_val)


    #uart.writechar(3)

    uart.write(":%f/%f/;" % (transmitted_val, 3))

    print(utime.ticks_diff(utime.ticks_ms(), last_clock))
    last_clock = utime.ticks_ms()









