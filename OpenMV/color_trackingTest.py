import sensor, image, time
from pyb import UART
import pyb
 #red_threshold_01 = (45, 100, -60, 80, 34, 91)
red_threshold_01 = (0, 25, -7, 127, -128, -20)
uart = UART(3, 9600)
blue_led = pyb.LED(3)
red_led = pyb.LED(1)
def map_value( input_data, min_in, max_in, min_out, max_out): #преобразование из одного значения в другой (указывается значение, начальный диапазон и конечный диапазон)
        if(input_data >= max_in):
            input_data = max_in - 1
        if (input_data <= min_in):
            input_data = min_in - 1
        output_data = round(((max_out-min_out)*input_data+max_in*min_out-max_out*min_in)/(max_in-min_in))
        return output_data

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(20)
sensor.set_auto_whitebal(False)
 #Turn off white balance. White balance is turned on by default. In color recognition, you need to turn off white balance
clock = time.clock()
img = sensor.snapshot()
blobs = img.find_blobs([red_threshold_01],
                         area_threshold=600)
last_blobs = blobs

x_size = 320
y_size = 240
while(True):
    clock.tick()
    img = sensor.snapshot()
    if last_blobs:
         for b in blobs:
            x1 = b[0]-7
            y1 = b[1]-7
            w1 = b[2]+12
            h1 = b[3]+12
            #print(b.cx(),b.cy())
         roi2 = (x1,y1,w1,h1)
         print(roi2)
         blobs = img.find_blobs([red_threshold_01],
                                    roi = roi2,
                                    area_threshold=2700)
         last_blobs = blobs
    else:
        blobs = img.find_blobs([red_threshold_01],
                                    area_threshold=2700)
        last_blobs = blobs
    if last_blobs:
        #If the target color is found
        #  print(blobs)
        for b in last_blobs:#Iteratively find the target color area
            img.draw_rectangle(b[0:4])
            img.draw_cross(b[5], b[6])
            cx = b[5]-round(x_size/2)
            data = map_value(cx, -round(x_size/2), round(x_size/2), 0,255)
            uart.writechar(data)
            blue_led .on()
    else:
        blue_led .off()
    #print("Frame rate: ",clock.fps())

