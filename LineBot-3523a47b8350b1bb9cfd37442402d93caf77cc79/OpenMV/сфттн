import sensor, image, time

# Initialize camera and sensor
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)

# Define color thresholds for green squares
green_threshold = (40, 100, -70, -10, -0, 40)  # RGB values for green

# Main loop
while(True):
    # Capture image
    img = sensor.snapshot()

    # Find green blobs
    green_blobs = img.find_blobs([green_threshold])

    # Find black line using Canny and Hough transform
    edges = img.find_edges(image.EDGE_CANNY)
    lines = img.find_lines(threshold = 1000, x_margin = 10, y_margin = 10, theta_margin = 10)

    # Check if any green blobs were found
    if green_blobs:
        for blob in green_blobs:
            # Check if green blob is above or below the black line
            above_line = False
            for line in lines:
                if line.x1() < blob.cx() < line.x2() and line.y1() < blob.cy() < line.y2():
                    above_line = True
                    break
            if above_line:
                print("Green blob above line")
            else:
                print("Green blob below line")

            # Draw green blob
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())

    # Draw black line
    for line in lines:
        img.draw_line(line.x1(), line.y1(), line.x2(), line.y2())

    # Display image
    img.display()
