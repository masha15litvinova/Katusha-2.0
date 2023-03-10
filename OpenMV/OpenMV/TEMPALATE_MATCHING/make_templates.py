# Untitled - By: masha - Ср мар 1 2023

import sensor, image, time, machine

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QCIF)
sensor.skip_frames(time = 2000)

clock = time.clock()

x_size = 176
y_size = 144
coeff_x = 0.2
coeff_y = 0.1

count = 50

roi_image = (round(x_size*coeff_x), round(y_size*coeff_y), round(x_size*(1-2*coeff_x)), round(y_size*(1-2*coeff_y)))

while(True):
    clock.tick()


    for i in range(count):

        img = sensor.snapshot()
        img.draw_rectangle(roi_image[0], roi_image[1], roi_image[2], roi_image[3], (255,0,0), 2)
        img.save('pictures/'+str(i),roi_image,quality=100)
        time.sleep_ms(100)
    machine.deepsleep()
