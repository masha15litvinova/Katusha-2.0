#
 #Elert Соответствует простая процедура камеры
#

import time, sensor, image
from image import SEARCH_EX, SEARCH_DS
 #Search_ex и search_ds введены из модуля imgae. Использовать от importПросто представьте search_ex,
 #Search_ds, две части, необходимые, не вводите весь модуль изображения.

sensor.reset()  #Initialized датчик (камера)
 # Установите датчик
sensor.set_contrast(1)
sensor.set_gainceiling(16)
sensor.set_framesize(sensor.LCD)  #Resolution, если вы используете ЖК -экран, вам нужно установить его в LCD. Как программа соответствия шаблонов.
sensor.set_pixformat(sensor.GRAYSCALE)  #Photo mode, метод изображения серого
sensor.skip_frames(time = 200)  #Стабильный.
sensor.set_auto_gain(False)  # Отслеживание цвета должно закрыть автоматическое усиление
sensor.set_auto_whitebal(False)  #Отключите баланс белого.
sensor.set_auto_exposure(False,16777)  #Settings воздействие,Нужно изменить

clock = time.clock()  # FPS -частота кадров

while(True):
    clock.tick()    #    snapshots()Миллисекунд проходят между.
    img = sensor.snapshot().lens_corr(strength = 1.8, zoom = 1.0)      #()
    print("FPS %f" % clock.fps())  # Уведомление:  При подключении компьютера OpenMV повернутся на полпути. Когда компьютер не подключен, частота кадров увеличится.
