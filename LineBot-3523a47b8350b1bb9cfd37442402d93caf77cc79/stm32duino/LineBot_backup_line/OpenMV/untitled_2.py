# Untitled - By: masha - Сб мар 11 2023

import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)

clock = time.clock()
thresholds = [47, 55, -128, 7, -128, 123]
while(True):
    clock.tick()
    img = sensor.snapshot().cont
    new_img = img
    new_img.binary([thresholds])
    print(clock.fps())
