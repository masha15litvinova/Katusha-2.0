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

uart = UART(3, 9600)
last_clock = 0

green_thresholds = [40, 92, -128, -15, -128, 127] #порог зеленого цвета
black_thresholds = [0, 28, -119, 127, -128, 127] #порог черного цвета
yellow_thresholds = [0, 100, -128, 127, -128, 67]
line_count = 25
segments_count = 30

x_size = 160
y_size = 120

y_k = 0.7
x_k = 0.2
delta_x = 2
delta_y = 5
ky_regression = 0.4
kx_regression =0.15

min_area = 160
min_pixels = 160
ky_green = 0.5
kx_green = 0.4


dir_turn = 0
'''
0 - backward
1 - right
2 - left
3 - no
'''

line_rois = []
segments_rois = []
center_blob_pos = [round(x_size/2), y_size]
angle = 0
linear_dev = 0
perp_length = 0
delta = 10

transmitted_val = 0
transmitted_sum = 0
perp_coeff=0.01
max_transmitted_val = round(x_size/2)*perp_coeff+90+1


weight_sum = 0
regression_roi = (round(x_size*kx_regression), round(y_size*ky_regression), round(x_size*(1-2*kx_regression)), round(y_size*(1-ky_regression)))
k_line = 0.2

def map( x,  in_min,  in_max,  out_min,  out_max):

    return (round((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)-1)




for i in range(line_count-1,-1,-1):
    w =  1
    line_rois.append((round(x_size*k_line), round(i*y_size/line_count), round(x_size*(1-2*k_line)), round(y_size/line_count),w))
    weight_sum+=w
for i in range(segments_count):

    segments_rois.append((0, round(i*y_size/line_count), x_size, round(y_size/line_count)))

while(True):
    clock.tick()

    # Gamma, contrast, and brightness correction are applied to each color channel. The
    # values are scaled to the range per color channel per image type...
    img = sensor.snapshot().gamma_corr(gamma = 0.7, contrast = 3.0, brightness = 0.0)#.gamma_corr(gamma = 0.85, contrast = 1.3, brightness = 0.06)
    #img.mean(4)
    left_roi = (round(x_size*x_k), round(y_size*y_k)-delta_y, round(x_size/2-x_size*x_k-delta_x), round(y_size*(1-y_k)))
    right_roi = (round(x_size/2+delta_x), round(y_size*y_k)-delta_y, round(x_size/2-x_size*x_k-delta_x), round(y_size*(1-y_k)))
    #img.draw_rectangle(left_roi[0], left_roi[1], left_roi[2], left_roi[3], (0,0,0),2,False)
    #img.draw_rectangle(right_roi[0], right_roi[1], right_roi[2], right_roi[3], (0,0,0),2,False)

    green_marker_left = img.find_blobs([green_thresholds], area_threshold = min_area, pixels_threshold = min_pixels, roi=left_roi)
    green_marker_right = img.find_blobs([green_thresholds], area_threshold = min_area, pixels_threshold = min_pixels, roi=right_roi)

    #if(len(green_markers_coords)!=0):
        #green_markers_coords_converted = list(green_markers[i].corners()))


    left = 0
    right = 0

    for gl in green_marker_left:
        pix = (gl.cx(), gl.cy()-8)
        if(sum(img.get_pixel(pix[0], pix[1]))>250):
            left += 1
    for gr in green_marker_right:
        pix = (gr.cx(), gr.cy()-8)
        if(sum(img.get_pixel(pix[0], pix[1]))>250):
            right += 1
    '''for gml in green_marker_left:
        width = 30
        height = 15
        roi_black = (max(min(round(gml.cx()-gml.w()/2),round(x_size)),0), max(min(round(gml.cy()-gml.h()/2-height),round(y_size/2)), 0), width, height)
        img.draw_rectangle(roi_black[0], roi_black[1], roi_black[2], roi_black[3],(255,0,0), 2)'''


    '''if(len(green_marker_left)>0):
        for gl in green_marker_left:
            img.draw_edges(gl.min_corners(), color=(255,0,0))
            img.draw_circle(gl.cx(), gl.cy(), 3, (255,0,0),2,True)
    if(len(green_marker_right)>0):
         for gr in green_marker_right:
             img.draw_edges(gr.min_corners(), color=(255,0,0))
             img.draw_circle(gr.cx(), gl.cy(), 3, (255,0,0),2,True)'''
    '''if(left>0)and(right>0):
        dir_turn = 0
        img.draw_string(10, 10, "backward",(255,0,0),2)
    elif(left==0)and(right>0):
        dir_turn = 1
        img.draw_string(10, 10, "right",(255,0,0),2)
    elif(left>0)and(right==0):
        dir_turn = 2
        img.draw_string(10, 10, "left",(255,0,0),2)
    elif(left==0)and(right==0):
        dir_turn = 3
        img.draw_string(10, 10, "no",(255,0,0),2)'''

    centroid_sum = 0

    line_blobs = []
    count_line_blobs = 0

    global_max = -1
    y_max = -1

    for r in line_rois:
        blobs = img.find_blobs([black_thresholds], roi=r[0:4], merge=True, area_threshold=min_area, pixels_threshold=min_pixels) # r[0:4] is roi tuple.

        if blobs:
            # Find the blob with the most pixels.
            largest_blob = min(blobs, key=lambda b: (b.cx()-center_blob_pos[0])**2+(b.cy()-center_blob_pos[1])**2)
            center_blob_pos [0]= largest_blob.cx()
            center_blob_pos [1]= largest_blob.cy()
            if(largest_blob.pixels()>global_max):
                global_max = largest_blob.pixels()
                y_max = center_blob_pos[1]
            # Draw a rect around the blob.
            #img.draw_rectangle(largest_blob.rect())
            img.draw_circle(largest_blob.cx(),
                           largest_blob.cy(),3,(255, 255, 0),3,True)

            #img.draw_string(largest_blob.cx()+7, largest_blob.cy(), str(count_line_blobs),(255,0,0), 2)

            line_blobs.append([largest_blob.cx(), largest_blob.cy()])


            centroid_sum += (largest_blob.cx()-round(x_size/2)) * r[4] # r[4] is the roi weight.
            count_line_blobs+=1

    if(global_max>-1):
        green_roi = (0, y_max, x_size, y_size-y_max)
    else:
        green_roi = (0,0,x_size, y_size)
    delta_x_roi = 15
    delta_y_up_roi = 30
    delta_y_down_roi = 20
    green_roi = (delta_x_roi,delta_y_up_roi,x_size-2*delta_x_roi, y_size-delta_y_up_roi-delta_y_down_roi)
    green_markers = img.find_blobs([green_thresholds], area_threshold = min_area, pixels_threshold = min_pixels, roi = green_roi)


    green_markers_centers = []
    if(len(green_markers)!=0):
        for grM in green_markers:
            green_markers_centers.append(grM.cx())
            #img.draw_circle(grM.cx(),grM.cy(),2,(255,0,0),2)
    if(len(green_markers_centers)==0):
        dir_turn = 3
        #img.draw_string(10, 10, "no",(255,0,0),2)
    elif (len(green_markers_centers)==2):
        dir_turn = 0
        #img.draw_string(10, 10, "back",(255,0,0),2)
    elif(len(green_markers_centers)==1):
        if(green_markers_centers[0]>round(x_size/2)+10):
            dir_turn = 1
            #img.draw_string(10, 10, "right",(255,0,0),2)
        elif(green_markers_centers[0]<round(x_size/2)-10):
            dir_turn = 1
            #img.draw_string(10, 10, "right",(255,0,0),2)
        else:
            dir_turn = 3
            #img.draw_string(10, 10, "no",(255,0,0),2)
    else:
        dir_turn = 3
        #img.draw_string(10, 10, "no",(255,0,0),2)



    new_img = img
    new_img.binary([yellow_thresholds])
    line_get = new_img.get_regression([(0,0)], robust=True,roi=regression_roi, x_stride=1, y_stride = 1)

    if (line_get):
        img.draw_line(line_get[0],line_get[1],line_get[2],line_get[3],255,1)
        #img.draw_string(10, 10, str(linear_dev), 127, 2)
    #img.draw_rectangle(regression_roi[0],regression_roi[1],regression_roi[2], regression_roi[3],127,1)

    # The 80 is from half the X res, the 60 is from half the Y res. The
    # equation below is just computing the angle of a triangle where the
    # opposite side of the triangle is the deviation of the center position
    # from the center and the adjacent side is half the Y res. This limits
    # the angle output to around -45 to 45. (It's not quite -45 and 45).

    if(line_get):
        angle = line_get.theta()
        linear_dev = round(-(line_get.x1()+line_get.x2())/2+x_size/2)
        if(angle>90):
            angle = angle-180
        angle = -angle
    else:
        angle = 0
        linear_dev = 0
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



    transmitted_val = (map(angle, -91, 91, 0, 255))#map(angle, -max_transmitted_val, max_transmitted_val, 0, 254) #+math.copysign(abs(round(perp_coeff*(-perp_length+round(x_size/2)))),angle)'''
    #print(transmitted_val)
    #linear_dev = 0
    transmitted_line_dev = map(linear_dev, -round(x_size/2), round(x_size/2), 0, 255)
    transmitted_sum = map(centroid_sum, -line_count*round(x_size/2), line_count*round(x_size/2), 0, 255)
    img.draw_string(10, 30, str(angle),(255,0,0),2)
    img.draw_string(10, 15, str(dir_turn),(255,0,0),2)
    if(len(green_markers)>0):
        dir_turn = 0
    #print(angle)


    #uart.writechar(255)

    #uart.writechar(transmitted_val)


    #uart.writechar(3)
    #img.draw_rectangle(green_roi[0], green_roi[1], green_roi[2], green_roi[3], (0,0,255),2,False)
    uart.write(":%d/%d/%d/;" %(transmitted_val, dir_turn,transmitted_line_dev))
    time.sleep_ms(10)
    #uart.write(":"+str(transmitted_val)+"/"+str(3)+"/"+str(transmitted_line_dev)+"/;")
    '''uart.write(":")
    time.sleep_ms(5)
    uart.write(str(transmitted_val))
    time.sleep_ms(5)
    uart.write("/")
    time.sleep_ms(5)
    uart.write(str(dir_turn))
    time.sleep_ms(5)
    uart.write("/")
    time.sleep_ms(5)
    uart.write(str(transmitted_sum))
    time.sleep_ms(5)
    uart.write("/")
    time.sleep_ms(5)
    uart.write(";")
    time.sleep_ms(5)'''
    #print(dir_turn)
    #time.sleep_ms(5)
    #uart.sendbreak()
    #uart.write(":"+str(transmitted_val)+"/"+str(3)+"/"+str(126)+"/;")
    #uart.print()
    #uart.sendbreak()

    #print(utime.ticks_diff(utime.ticks_ms(), last_clock))
    #print(transmitted_line_dev)
    last_clock = utime.ticks_ms()









