# Пример соответствия шаблона NCC-Normalized Cross Correlation (NCC)
#
 # Этот пример показывает, как использовать функцию NCC OpenMV, чтобы сделать небольшую часть изображения и изображения изображения и изображения
 #...Не всегда полезно получить чрезвычайно контролируемую среду NCC.
#
 #: Поддержка NCC должна быть заменена! До сих пор эта функция требует большой работы, чтобы быть полезной.
 # Этот сценарий повторно установит существование функции, но текущее состояние недостаточно.

import time, sensor, image
from image import SEARCH_EX, SEARCH_DS
 #Search_ex и search_ds введены из модуля imgae. Использовать от importПросто представьте search_ex,
 #Search_ds, две части, необходимые, не вводите весь модуль изображения.

 # Сброс датчик
sensor.reset()

 # Установите датчик
sensor.set_contrast(1)
sensor.set_gainceiling(16)
# Max resolution for template matching with SEARCH_EX is QQVGA
 # Максимальное разрешение шаблона и search_ex - qqvga
sensor.set_framesize(sensor.QQVGA)
 # Вы можете установить окно, чтобы уменьшить поисковые картинки.
#sensor.set_windowing(((640-80)//2, (480-60)//2, 80, 60))
sensor.set_pixformat(sensor.GRAYSCALE)#Grayscale изображение.

 # Загрузите шаблон.
 # Шаблон должен быть маленьким(Например.32X32 пиксель)Изображение серого.
template = image.Image("/1.pgm")

clock = time.clock()

 #
while (True):
    clock.tick()
    img = sensor.snapshot()

    # find_template(template, threshold, [roi, step, search])
    # ROI:  Заинтересованная группа (x, y, w, h).
    # Step:Цикл использования(y+= Step, x+= Step)  Используйте большие шаги, чтобы сделать его быстрее.
         # Поиск - это изображение.Search_ex проводит подробный поиск или для изображения.Search_ds выполнять поиск алмаза
    #
    # Note1:  ROI должен быть меньше изображения и шаблона.
    # Note2:В поиске бриллиантов, шаг и рентабельность инвестиций игнорируются.
    r = img.find_template(template, 0.70, step=4, search=SEARCH_EX) #, roi=(10, 0, 60, 60))
    # find_template(template, threshold, [roi, step, search]),
         # Порог0.7Подобный порог,ROI - это соответствующая область (верхняя левая вершина - это (верхняя левая10，0),длинная80ширина60Прямоугольник),
         # Обратите внимание, что размер ROI больше, чем картина шаблона, меньше, чем Frambuffer.
         # Отметьте соответствующее изображение
    if r:
        img.draw_rectangle(r)
        print("соответствие")

    print(clock.fps())