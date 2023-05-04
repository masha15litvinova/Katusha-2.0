# Untitled - By: masha - Вт май 2 2023

import sensor, image, time
from pyb import UART
from pyb import Pin
import pyb

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)

def map_value( input_data, min_in, max_in, min_out, max_out): #преобразование из одного значения в другой (указывается значение, начальный диапазон и конечный диапазон)
        if(input_data >= max_in):
            input_data = max_in - 1
        if (input_data <= min_in):
            input_data = min_in - 1
        output_data = round(((max_out-min_out)*input_data+max_in*min_out-max_out*min_in)/(max_in-min_in))
        return output_data

x_size = 320
y_size = 240

k_x = 0.7
k_y = 0.5
delta_h = 20

red_thr = (0, 44, 29, 127, -128, 127)
green_thr = (12, 30, -128, -9, -128, 127)
uart = UART(3, 9600)
pin1 = Pin('P1', Pin.IN, Pin.PULL_DOWN)

blue_led = pyb.LED(3)
green_led = pyb.LED(2)
red_led = pyb.LED(1)

roi_zone = (round(x_size*(1-k_x)/2), delta_h, round(x_size*k_x), round(y_size*k_y))

clock = time.clock()

while(True):
    clock.tick()
    img = sensor.snapshot()

    red_zone = img.find_blobs([red_thr], roi= roi_zone, area_threshold=400, pixels_threshold=400)
    green_zone = img.find_blobs([green_thr], roi= roi_zone, area_threshold=400, pixels_threshold=400)
    x_zone_red = -1
    y_zone_red = -1
    x_zone_green = -1
    y_zone_green = -1
    max_pixels_red = -1
    max_pixels_green = -1
    uart_access = pin1.value()
    #print(uart_access)


    for rz in red_zone:
        if(rz.pixels()>max_pixels_red):
            max_pixels_red = rz.pixels()
            x_zone_red = rz.cx()
            y_zone_red = rz.cy()
    if((x_zone_red>0)and(y_zone_red>0)):
         img.draw_cross(x_zone_red, y_zone_red, (0,255,0),3,2)
         red_led.on()
    else:
        red_led.off()


    for gz in green_zone:
        if(gz.pixels()>max_pixels_green):
            max_pixels_green = gz.pixels()
            x_zone_green = gz.cx()
            y_zone_green = gz.cy()
    if((x_zone_green>0)and(y_zone_green>0)):
         img.draw_cross(x_zone_green, y_zone_green, (255,0,0),3,2)
         green_led.on()
    else:
         green_led.off()
    if(green_zone):
        print(max_pixels_green)
    if(True):
        delta = (x_zone_green-round(x_size/2))

        coord_send = map_value(delta,-round(x_size/2), round(x_size/2),0,255)
        uart.writechar(coord_send)
        time.sleep_ms(40)

    #uart.writechar(1)

