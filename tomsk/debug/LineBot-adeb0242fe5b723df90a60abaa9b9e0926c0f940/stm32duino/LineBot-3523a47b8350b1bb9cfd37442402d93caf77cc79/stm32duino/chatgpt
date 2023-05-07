import sensor, image

# инициализируем камеру
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)

# захватываем изображение
img = sensor.snapshot()

# бинаризация изображения для выделения черных линий
threshold = (0, 80)
img.binary([threshold], invert=True)

# скелетонизация изображения
img.skeletonize(thinning=True)

# операция размыкания для удаления маленьких черных объектов
kernel_size = 3
img.morph(opening=(kernel_size, kernel_size))

# преобразование Хафа для обнаружения линий
min_line_length = 50
max_line_gap = 10
lines = img.find_lines(threshold = 1000, theta_margin = 25, rho_margin = 25, roi = (0, 0, img.width(), img.height()), merge_distance = 0, max_theta_diff = 5, robust = True, segment_threshold = 100, line_threshold = 5)

# находим пересечения линий
intersections = []
for i in range(len(lines)):
    for j in range(i + 1, len(lines)):
        intersection = lines[i].intersects(lines[j])
        if intersection:
            intersections.append(intersection)

# выводим координаты пересечений
for i in range(len(intersections)):
    print("Intersection %d: x=%d, y=%d" % (i, intersections[i].x(), intersections[i].y()))

# находим центр перекрестка
if len(intersections) >= 4:
    x_sum = 0
    y_sum = 0
    for intersection in intersections:
        x_sum += intersection.x()
        y_sum += intersection.y()
    center_x = x_sum // len(intersections)
    center_y = y_sum // len(intersections)
    print("Center: x=%d, y=%d" % (center_x, center_y))
else:
    print("Crossroad not detected.")
