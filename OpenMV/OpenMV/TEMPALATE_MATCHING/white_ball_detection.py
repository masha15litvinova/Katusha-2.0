# Template Matching Example - Normalized Cross Correlation (NCC)
#
# This example shows off how to use the NCC feature of your OpenMV Cam to match
# image patches to parts of an image... expect for extremely controlled enviorments
# NCC is not all to useful.
#
# WARNING: NCC supports needs to be reworked! As of right now this feature needs
# a lot of work to be made into somethin useful. This script will reamin to show
# that the functionality exists, but, in its current state is inadequate.

import time, sensor, image
from image import SEARCH_EX, SEARCH_DS
# Introduce SEARCH_EX and SEARCH_DS from image module and only SEARCH_EX if you choose to use from import.
# SEARCH_DS There are only two parts needed instead of the whole module of image.

# Reset sensor
sensor.reset()

# Set sensor settings
sensor.set_contrast(1)
sensor.set_gainceiling(16)
# Max resolution for template matching with SEARCH_EX is QQVGA
sensor.set_framesize(sensor.QCIF)
# You can set windowing to reduce the search image.
#sensor.set_windowing(((640-80)//2, (480-60)//2, 80, 60))
sensor.set_pixformat(sensor.GRAYSCALE)

# Load template.
# Template should be a small (eg. 32x32 pixels) grayscale image.


count = 41

clock = time.clock()

# Run template matching
while (True):
    clock.tick()


    # find_template(template, threshold, [roi, step, search])
    # ROI: The region of interest tuple (x, y, w, h).
    # Step: The loop step used (y+=step, x+=step) use a bigger step to make it faster.
    # Search is either image.SEARCH_EX for exhaustive search or image.SEARCH_DS for diamond search
    #
    # Note1: ROI has to be smaller than the image and bigger than the template.
    # Note2: In diamond search, step and ROI are both ignored.
    for i in range(count):
        img = sensor.snapshot()
        print("pgm/"+str(i)+".pgm")
        templ = image.Image("pgm/"+str(i)+".pgm")
        r = img.find_template(templ, 0.30, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
    #find_template(template, threshold, [roi, step, search]) The 0.7 I the threshold is the similarity threshold and  ROI is the region to be matched(its upper left vertex is （10，0）), a rectangular of 60 by 80.
    # Note: The size of the ROI should be larger than the template images and smaller than frame butter.
    # Mark the matched image.

        if r:
            img.draw_rectangle(r)

    print(clock.fps())
