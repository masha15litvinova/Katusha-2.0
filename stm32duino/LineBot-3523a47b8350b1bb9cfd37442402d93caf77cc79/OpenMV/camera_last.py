import sensor
import image
import time
import math
from pyb import UART

x_size = 352
y_size = 288


class greenSquares():
    DRAWING_SQR = True
    #detecting the green squares

    global x_size
    global y_size
    green_thresholds = [21, 91, -128, -7, -128, 127] #порог зеленого цвета
    black_thresholds = [0, 25, -128, 34, -128, 41] #порог черного цвета
    min_area = 1200    #минимальная площадь квадратов
    min_pixels = 1200
    height = 20
    height_k = 1.0


    def __init__(self, in_image):
        k = 0.9


        #roi_gs = (0, int(y_size//2- y_size*k//2), x_size, int(y_size//2+ y_size*k//2))
        roi_gs = (0,self.height, x_size, round(self.height_k*y_size))
        self.green_markers = in_image.find_blobs([self.green_thresholds], area_threshold = self.min_area, pixels_threshold = self.min_pixels, roi=roi_gs)
        self.count = len(self.green_markers)
        self.m_array = []  #array of coordinates of the markers (2 coordinates for each of 4 corners (x,y))
        self.m_array_converted = [] #array of coordinates of the markers in converted order
        self.angles_array = [] #массив углов наклона для всех квадратиков
        self.perimeter_array = [] #массив цветов с 4 сторон от каждого зеленого квадрата
        self.centres_array = [] #массив центров квадратов
        self.cam_image = in_image

        if True:
            self.cam_image.draw_rectangle(0,self.height, x_size, round(self.height_k*y_size),(0,0,255), 2, False)



    def markers_array(self):

        for i in range(self.count):
            self.m_array.append(list(self.green_markers[i].min_corners()))


    def convert_vertices_order(self):  #изменение порядка вершин квадрата (по возрастанию y-координаты)

        self.m_array_converted = []
        if len(self.m_array) != 0:
            len_in_arr = len(self.m_array)
            for j in range(len_in_arr):

                arr = []
                for i in range(4):

                    arr.append(list(self.m_array[j][i]))


                arr.sort(key = lambda x: x[1])

                self.m_array_converted.append(arr) #vertices in converted order

    def perimeter_colors(self):    #array with elements like [0, 1, 1, 0] 0-white 1-black  [left, right, down, up]
        #in_arr - array [[[x1, y1],[x2, y2],[x3, y3],[x4, y4]],[],[],[]] -- VERTICES OF MARKERS MUST BE IN CONVERTED ORDER!!!
        self.perimeter_array = []
        self.colors_array = []


        for i in range(len(self.centres_array)):
            sq_perimeter_array = [0,0,0,0,0,0]
            sq_perimeter_array[0] = self.centres_array[i][0]
            sq_perimeter_array[1] = self.centres_array[i][1]
            dist_coeff = 1.8 #for distance between green square and near pixels (во сколько раз расстояние до центра квадрата больше расстояния от центра стороны до соседних пикселей)
            c_x = self.centres_array[i][0]
            c_y = self.centres_array[i][1]
            v0 = self.m_array_converted[i][0]
            v1 = self.m_array_converted[i][1]
            v2 = self.m_array_converted[i][2]
            v3 = self.m_array_converted[i][3]
            if self.DRAWING_SQR:
                self.cam_image.draw_circle(v0[0], v0[1], 2, (255,255,0), 3, True)
                self.cam_image.draw_circle(v1[0], v1[1], 2, (255,255,0), 3, True)
                self.cam_image.draw_circle(v2[0], v2[1], 2, (255,255,0), 3, True)
                self.cam_image.draw_circle(v3[0], v3[1], 2, (255,255,0), 3, True)
            angle = self.angles_array[i]
            if angle > 5:
                up = [min(max(int(((dist_coeff/2 + 0.5)*(v0[0]+v1[0])-c_x)/dist_coeff),1),x_size-1), min(max(int(((dist_coeff/2 + 0.5)*(v0[1]+v1[1])-c_y)/dist_coeff),1),y_size-1)]
                left = [min(max(int(((dist_coeff/2 + 0.5)*(v0[0]+v2[0])-c_x)/dist_coeff),1),x_size-1), min(max(int(((dist_coeff/2 + 0.5)*(v0[1]+v2[1])-c_y)/dist_coeff), 1), y_size-1)]
                down = [min(max(int(((dist_coeff/2 + 0.5)*(v3[0]+v2[0])-c_x)/dist_coeff), 1), x_size-1), min(max(int(((dist_coeff/2 + 0.5)*(v3[1]+v2[1])-c_y)/dist_coeff), 1),y_size - 1)]
                right = [min(max(int(((dist_coeff/2 + 0.5)*(v3[0]+v1[0])-c_x)/dist_coeff),1),x_size -1), min(max(int(((dist_coeff/2 + 0.5)*(v3[1]+v1[1])-c_y)/dist_coeff),1), y_size-1)]
            elif angle < -5:
                up = [min(max(int(((dist_coeff/2 + 0.5)*(v0[0]+v1[0])-c_x)/dist_coeff),1), x_size-1), min(max(int(((dist_coeff/2 + 0.5)*(v0[1]+v1[1])-c_y)/dist_coeff),1), y_size-1)]
                left = [min(max(int(((dist_coeff/2 + 0.5)*(v1[0]+v3[0])-c_x)/dist_coeff),1),x_size-1), min(max(int(((dist_coeff/2 + 0.5)*(v1[1]+v3[1])-c_y)/dist_coeff),1), y_size-1)]
                down = [min(max(int(((dist_coeff/2 + 0.5)*(v3[0]+v2[0])-c_x)/dist_coeff),1), x_size-1), min(max(int(((dist_coeff/2 + 0.5)*(v3[1]+v2[1])-c_y)/dist_coeff), 1),y_size - 1)]
                right = [min(max(int(((dist_coeff/2 + 0.5)*(v2[0]+v0[0])-c_x)/dist_coeff),1),x_size-1), min(max(int(((dist_coeff/2 + 0.5)*(v2[1]+v0[1])-c_y)/dist_coeff),1),y_size-1)]
            else:
                if self.m_array_converted[i][0][0] > self.m_array_converted[i][1][0]:
                    self.m_array_converted[i][0][0],self.m_array_converted[i][1][0] = self.m_array_converted[i][1][0],self.m_array_converted[i][0][0]
                if self.m_array_converted[i][2][0] > self.m_array_converted[i][3][0]:
                    self.m_array_converted[i][2][0],self.m_array_converted[i][3][0] = self.m_array_converted[i][3][0],self.m_array_converted[i][2][0]
                length = abs(self.m_array_converted[i][2][1]-self.m_array_converted[i][0][1])

                up = [min(max(self.centres_array[i][0], 1), x_size - 1), min(max(int(self.m_array_converted[i][0][1]-(length/2)/dist_coeff), 1), y_size-1)]
                left = [min(max(int(self.m_array_converted[i][0][0]-(length/2)/dist_coeff), 1), x_size-1), min(max(self.centres_array[i][1], 1), y_size - 1)]
                down = [min(max(self.centres_array[i][0], 1), x_size - 1), min(max(int(self.m_array_converted[i][3][1]+(length/2)/dist_coeff), 1), y_size-1)]
                right = [min(max(int(self.m_array_converted[i][3][0]+(length/2)/dist_coeff), 1), x_size-1), min(max(self.centres_array[i][1], 1), y_size - 1)]

            colors = []
            length = 10

            l_roi = self.dot_roi(min(max(left[0]-length//2, 1), x_size-1),min(max(left[1]-length//2,1), y_size-1),length,length)
            r_roi = self.dot_roi(min(max(right[0]-length//2,1),x_size-1),min(max(right[1]-length//2,1), y_size-1),length,length)
            d_roi = self.dot_roi(min(max(down[0]-length//2, 1), x_size-1),min(max(down[1]-length//2,1), y_size-1),length,length)
            u_roi = self.dot_roi(min(max(up[0]-length//2,1), x_size-1),min(max(up[1]-length//2,1), y_size-1),length,length)

            #print("l_roi =", l_roi)
            #print("r_roi =", r_roi)
            #print("d_roi =", d_roi)
            #print("u_roi =", u_roi)

            if l_roi[2]*l_roi[3] == 0: #checking if roi is rectangle
                break
            elif r_roi[2]*r_roi[3] == 0:
                break
            elif d_roi[2]*d_roi[3] == 0:
                break
            elif u_roi[2]*u_roi[3] == 0:
                break
            else:
                if len(self.cam_image.find_blobs([self.black_thresholds],False,roi=l_roi, pixels_threshold=(length**2)//4))>0:
                    if self.DRAWING_SQR:
                        self.cam_image.draw_rectangle(self.dot_roi(left[0],left[1],20,20),(255,255,255),2,False)
                        self.cam_image.draw_string(left[0], left[1], "B", (255,255,255), 1)
                    sq_perimeter_array[2] = 1
                else:
                    if self.DRAWING_SQR:
                        self.cam_image.draw_rectangle(self.dot_roi(left[0],left[1],20,20),(0,0,0),2,False)
                        self.cam_image.draw_string(left[0], left[1], "W", (0,0,0), 1)
                    sq_perimeter_array[2] = 0
                if len(self.cam_image.find_blobs([self.black_thresholds],False,roi=r_roi, pixels_threshold=(length**2)//4))>0:
                    if self.DRAWING_SQR:
                        self.cam_image.draw_rectangle(self.dot_roi(right[0],right[1],20,20),(255,255,255),2,False)
                        self.cam_image.draw_string(right[0], right[1], "B", (255,255,255), 1)
                    sq_perimeter_array[3] = 1
                else:
                    if self.DRAWING_SQR:
                        self.cam_image.draw_rectangle(self.dot_roi(right[0],right[1],20,20),(0,0,0),2,False)
                        self.cam_image.draw_string(right[0], right[1], "W", (0,0,0), 1)
                    sq_perimeter_array[3] = 0
                if len(self.cam_image.find_blobs([self.black_thresholds],False,roi=d_roi, pixels_threshold=(length**2)//4))>0:
                    if self.DRAWING_SQR:
                        self.cam_image.draw_rectangle(self.dot_roi(down[0],down[1],20,20),(255,255,255),2,False)
                        self.cam_image.draw_string(down[0], down[1], "B", (255,255,255), 1)
                    sq_perimeter_array[4] = 1
                else:
                    if self.DRAWING_SQR:
                        self.cam_image.draw_rectangle(self.dot_roi(down[0],down[1],20,20),(0,0,0),2,False)
                        self.cam_image.draw_string(down[0], down[1], "W", (0,0,0), 1)
                    sq_perimeter_array[4] = 0
                if len(self.cam_image.find_blobs([self.black_thresholds],False,roi=u_roi, pixels_threshold=(length**2)//4))>0:
                    if self.DRAWING_SQR:
                        self.cam_image.draw_rectangle(self.dot_roi(up[0],up[1],20,20),(255,255,255),2,False)
                        self.cam_image.draw_string(up[0], up[1], "B", (255,255,255), 1)
                    sq_perimeter_array[5] = 1
                else:
                    if self.DRAWING_SQR:
                        self.cam_image.draw_rectangle(self.dot_roi(up[0],up[1],20,20),(0,0,0),2,False)
                        self.cam_image.draw_string(up[0], up[1], "W", (0,0,0), 1)
                    sq_perimeter_array[5] = 0
                self.perimeter_array.append(sq_perimeter_array)   #[left, right, down, up]





    def turn_direction(self):
        '''0 - don't know
        1 - right
        2 - left
        3 - straight forward
        4 - backward'''
        direct = 0
        #print("self.perimeter_array =", self.perimeter_array)
        #print("self.perimeter_array =", self.perimeter_array)
        if(len(self.perimeter_array)==0):
            direct = 0
        elif (len(self.perimeter_array)==1):
            if(self.perimeter_array[0][5]*self.perimeter_array[0][2] == 1):
                direct = 1
            elif (self.perimeter_array[0][5]*self.perimeter_array[0][3] == 1):
                direct = 2
            else:
                direct = 3
        elif (len(self.perimeter_array)==2):
            if((self.perimeter_array[0][5]*self.perimeter_array[1][5])):
                direct = 4
            elif(self.perimeter_array[0][5]*self.perimeter_array[0][2] + self.perimeter_array[1][5]*self.perimeter_array[1][2] == 1):
                direct = 1
            elif (self.perimeter_array[0][5]*self.perimeter_array[0][3] + self.perimeter_array[1][5]*self.perimeter_array[1][3] == 1):
                direct = 2
            else:
                direct = 3
        elif (len(self.perimeter_array)==3): #тут какая-то дичь происходит сейчас [...,...,l, r, d, u] 0-white, 1-black
            if self.perimeter_array[0][5]+self.perimeter_array[1][5]+self.perimeter_array[2][5] == 2:
                direct = 4
            else:
                if (self.perimeter_array[0][5]*self.perimeter_array[0][3] == 1) or (self.perimeter_array[1][5]*self.perimeter_array[1][3] == 1) or (self.perimeter_array[2][5]*self.perimeter_array[2][3] == 1):
                    direct = 2
                elif (self.perimeter_array[0][5]*self.perimeter_array[0][2] == 1) or (self.perimeter_array[1][5]*self.perimeter_array[1][2] == 1) or (self.perimeter_array[2][5]*self.perimeter_array[2][2] == 1):
                    direct = 1
        elif (len(self.perimeter_array)==4):
            direct = 4

        if self.DRAWING_SQR:

            if direct == 0:
                self.cam_image.draw_string(10, 10, "Don't know", color=(255,255,0), scale=2)
            elif direct == 1:
                self.cam_image.draw_string(10, 10, "Right", color=(255,255,0), scale=2)
            elif direct == 2:
                self.cam_image.draw_string(10, 10, "Left", color=(255,255,0), scale=2)
            elif direct == 3:
                self.cam_image.draw_string(10, 10, "Straight forward", color=(255,255,0), scale=2)
            elif direct == 4:
                self.cam_image.draw_string(10, 10, "Backward", color=(255,255,0), scale=2)

        return direct


    def count_angles(self):

        self.angles_array = []

        for i in range(self.count):
            c_x = self.centres_array[i][0]
            c_y = self.centres_array[i][1]
            if self.DRAWING_SQR:
                self.cam_image.draw_circle(c_x, c_y, 2, (255,0,0), 3, True)
            med_x = int((self.m_array_converted[i][0][0] + self.m_array_converted[i][1][0])/2)
            med_y = int((self.m_array_converted[i][0][1] + self.m_array_converted[i][1][1])/2)
            angle = math.degrees(-math.atan((med_x - c_x)/(med_y-c_y)))
            self.angles_array.append(angle)
            if self.DRAWING_SQR:
                self.cam_image.draw_arrow(c_x, c_y, med_x, med_y, (0,0,255), 2)
                self.cam_image.draw_circle(med_x, med_y, 3,(0,0,255), 2, True)
                self.cam_image.draw_string(c_x - 10, c_y, str(angle),(255, 0, 0), 1)

    def markers_centres(self):
        self.centres_array = []


        for i in range(self.count):
            c_x = int((self.m_array[i][0][0] +self.m_array[i][1][0] + self.m_array[i][2][0] +self.m_array[i][3][0])/4)
            c_y = int((self.m_array[i][0][1] + self.m_array[i][1][1] + self.m_array[i][2][1] +self.m_array[i][3][1])/4)
            c = [c_x, c_y]
            self.centres_array.append(c)

    def dot_roi(self, x, y, lx, ly):   #rectangle around the dot (it is the center of it)  lx - x-side  ly - y-side
        x_coord1 = min(max(x - lx//2, 1), x_size-1)
        y_coord1 = min(max(y - ly//2, 1), y_size-1)
        x_coord2 = min(max(x + lx//2, 1), x_size-1)
        y_coord2 = min(max(y + ly//2, 1), y_size-1)
        l_rect_x = abs(x_coord2 - x_coord1)
        l_rect_y = abs(y_coord2 - y_coord1)
        roi_rect = (x_coord1, y_coord1, l_rect_x, l_rect_y)
        return roi_rect

    def draw_info(self):
        for i in range(len(self.green_markers)):
            corners = self.green_markers[i].min_corners()
            for j in range(4):
                img.draw_circle(corners[j][0], corners[j][1], 2, (255,0,0), 3, True)

class Line():
    #detecting the line
    DRAWING_L = True
    black_thresholds = [0, 42, -128, 127, -128, 127] #thresholds of black
    min_area = 200
    min_pixels = 200
    height_k = 0.7
    height = 0
    def __init__(self, in_image):

        self.roi_k = 0
        roi_line = (round(x_size*self.roi_k),self.height,round((1-2*self.roi_k)*x_size), round(y_size*self.height_k)+self.height)
        self.lines = []
        self.line_array = [] #array of approximated vertices of line
        self.sorted_line_fr = []
        self.cam_image = in_image
        self.sqr_avg_deviation = 0;
        if self.DRAWING_L:
            self.cam_image.draw_rectangle(round(x_size*self.roi_k),round((1-self.height_k)*y_size)+self.height,round((1-2*self.roi_k)*x_size), round(y_size*self.height_k),(0,255,0), 2, False)

    def approx_line(self): #ВООБЩЕ НАДО ПОТОМ ДОПИСАТЬ, ЕСЛИ В КАДРЕ НЕ ОДНА ЛИНИЯ
        self.sorted_line_fr = []
        self.appr_deg =  15#на сколько (горизонтальных) полосок делим экран
        self.appr_width = round(y_size*self.height_k) // self.appr_deg #ширина одной полоски, где ищем фрагмент линии
        center = [y_size, x_size//2]
        if self.DRAWING_L:
            for i in range(self.appr_deg):
                self.cam_image.draw_line(0, i*self.appr_width+round((1-self.height_k)*y_size)+self.height, x_size, i*self.appr_width+round((1-self.height_k)*y_size)+self.height, (255, 255, 255), 1)
        self.lfr_centers = [] #centers of line fragments
        self.line_angles = [] #angles of lines
        self.line_deviations = [] #отклонения от центра каждого фрагмента линии
        self.line_widths = []
        for i in range(freq-1):
            self.line_angles.append(0)
            self.line_deviations.append(0)

        for i in range(self.appr_deg):
            roi_line_fr = (round(x_size*self.roi_k), self.appr_width*(self.appr_deg-i-1)+round((1-self.height_k)*y_size)+self.height, round((1-2*self.roi_k)*x_size), self.appr_width) #область поиска черных фрагментов линии
            self.line_fr = self.cam_image.find_blobs([self.black_thresholds], area_threshold = self.min_area, pixels_threshold = self.min_pixels, roi=roi_line_fr)
            if self.line_fr:
                dist_min = 1000000000000
                for x in self.line_fr:
                    d = (x.cx()-center[0])**2+(x.cy()-center[1])**2
                    if d < dist_min:
                        s = x
                        dist_min = d



                self.sorted_line_fr.append(s)
                center[0] = s.cx()
                center[1] = s.cy()
                self.cam_image.draw_circle(s.cx(), s.cy(), 5, (0, 255, 0), 1, True)


            for slfr in self.sorted_line_fr:

                self.lfr_centers.append([slfr.cx(), slfr.cy()])
                self.line_widths.append(slfr.w())



            for slfr in self.sorted_line_fr:
                if self.DRAWING_L:
                    self.cam_image.draw_circle(slfr.cx(), slfr.cy(), 5, (255, 0, 0), 1, True)


        if self.DRAWING_L:
            self.cam_image.draw_line(x_size//2, 0, x_size//2, y_size, (0, 255, 0), 1)
            #линия по углам отклонения от центра
        if len(self.lfr_centers) != 0:
            for i in range(len(self.lfr_centers)):
                #self.cam_image.draw_line(x_size//2, self.lfr_centers[i][1], self.lfr_centers[i][0], self.lfr_centers[i][1], (0, 255, 255), 1)
                deviation = (self.lfr_centers[i][0] - x_size//2)*self.line_widths[i]
                self.line_deviations.append(deviation)

                #линия по углам между соседними точками
                #print("lfr_centers = ", self.lfr_centers)
            '''if len(self.lfr_centers) != 0:
                for i in range(len(self.lfr_centers)-1):
                    self.cam_image.draw_line(self.lfr_centers[i][0], self.lfr_centers[i][1], self.lfr_centers[i+1][0], self.lfr_centers[i+1][1], (255,0,0), 1)
                    angle = int(math.degrees(math.atan((self.lfr_centers[i][0]-self.lfr_centers[i+1][0])/(max(-self.lfr_centers[i][1]+self.lfr_centers[i+1][1],0.001)))))
                    self.line_angles[i] = angle


        self.avg_angle = 0
        sum_angles = 0
        for i in range(len(self.line_angles)):
            sum_angles += self.line_angles[i]
        self.avg_angle = int(sum_angles / len(self.line_angles))

        print("line_angles =", self.line_angles)
        print("avg_angle =", self.avg_angle)
        return avg_angle'''
        self.avg_deviation = 0 #отклонение от линии в целом
        sum_deviations = 0
        for i in range(len(self.line_deviations)):
            #sum_deviations += self.line_deviations[i]*abs(self.line_deviations[i])
            sum_deviations += self.line_deviations[i]
        self.sqr_avg_deviation = int(sum_deviations / len(self.line_deviations))

        #print("line_deviations =", self.line_deviations)
        #print("avg_deviation =", self.sqr_avg_deviation)


class Info_send():
    #отправка данных на МК (STM32F103)

    def __init__(self):
        self.uart = UART(3, 115200) #создаем UART 3: используем пины PB10, PB11 (они названы P4 и P5 на корпусе камеры)
        self.send_data = [] #направление поворота (цифра (0, 1, 2, 3, 4))
        self.arr_line_info = []
        for i in range(freq):
            self.arr_line_info.append(0)
        for i in range(freq):
            self.send_data.append(0)

    def map_value(self, input_data, min_in, max_in, min_out, max_out): #преобразование из одного значения в другой (указывается значение, начальный диапазон и конечный диапазон)
        if(input_data >= max_in):
            input_data = max_in - 1
        if (input_data <= min_in):
            input_data = min_in - 1
        output_data = round(((max_out-min_out)*input_data+max_in*min_out-max_out*min_in)/(max_in-min_in))
        return output_data
    def remember_line_data(self, value):

        for i in range(freq-1,0, -1):
            #print("send_data["+str(i)+"] =", self.send_data[i])
            self.arr_line_info[i] = self.arr_line_info[i-1]
            #print("send_data now =", self.send_data)
        self.arr_line_info[0] = value
    def remember_dir_data(self, value):

        for i in range(freq-1,0, -1):
            #print("send_data["+str(i)+"] =", self.send_data[i])
            self.send_data[i] = self.send_data[i-1]
            #print("send_data now =", self.send_data)
        self.send_data[0] = value


    def transmit_line_data(self, line_info, dir_info, angle_info):  #ball_info - координаты центров [белый, белый, черный]
        #print("send_data =", self.send_data)
        sum_data = 0
        for j in range(freq):
            sum_data += self.send_data[j]
        avg_data = round(sum_data / freq)
        directions = ["no", "right", "left", "straight", "backward"]
        #print("avg_data =", avg_data)
        #print("send to mcu:", directions[avg_data], line_info)

        #преобразуем в диапазон (0, 250), чтобы передать по UART'у (251-255 используются как сигнальные, поэтому максимум 250)




        sum_line_info = 0

        for i in range(freq):
            sum_line_info += self.arr_line_info[i]#*abs(self.arr_line_info[i])

        avg_line_info = round((sum_line_info / freq))

        converted_line_info = self.map_value(avg_line_info, -8000, 8000, 0, 250)  #UART очевидно еще не тестировался
        converted_angle_info = self.map_value(angle_info, -60, 60, 0, 250)
        '''converted_ball_info = []
        for i in range(3):

            converted_ball_info[i] = self.map_value(ball_info[i], -x_size//2, x_size//2, 0, 250)'''

        #сигнальные байты, чтобы удобнее читать на STM

        byte_signal = 253
        byte_line = 254
        byte_dir = 255

        '''print("-----------------")
        print(round(avg_data))
        print(converted_line_info)
        print(angle_info)
        print(byte_signal)
        print("-------------------")

        print("line_info: ", line_info)'''
        #print("converted_line_info: ", converted_line_info)
        #print("----------")
        #print("arr_line_info =", self.arr_line_info)
        #print("----------")

        self.uart.writechar(dir_info) #avg_data
        time.sleep_ms(1)
        self.uart.writechar(converted_line_info) #converted_line_info
        time.sleep_ms(1)
        self.uart.writechar(converted_angle_info)
        time.sleep_ms(1)
        self.uart.writechar(byte_signal)
        time.sleep_ms(1)
        '''uart.write(converted_ball_info[0])
        uart.write(byte_balls)
        uart.write(converted_ball_info[1])
        uart.write(byte_balls)
        uart.write(converted_ball_info[2])
        uart.write(byte_balls)'''
        '''if (uart.any()):
            mode = uart.read()''' #stm должна мониторить перекресток на входе в зону эвакуации  1-линия 0-эвакуация и надо на камере тоже его проверять


class Balls():

    black_balls_threshold = [0, 0, 0, 0, 0, 0] #черные жертвы --надо подобрать
    white_balls_threshold = [0, 0, 0, 0, 0, 0] #серебряные жертвы --надо подобрать

    balls_info = []

    def __init__(self, in_image):
        #значения надо подобрать
        self.cam_image = in_image
        self.black_ft = img
        self.black_ft.binary([black_balls_threshold])

        self.white_ft = img
        self.white_ft.binary([white_balls_threshold])

        self.black_balls_array = []
        self.white_balls_array = []

        for c in self.black_ft.find_circles(threshold = 2000, x_margin = 10, y_margin = 10, r_margin = 10, r_min = 100, r_max = 200, r_step = 2):
            self.cam_images.draw_circle(c.x(), c.y(), c.r(), color = (255, 0, 0))
            self.black_balls_array.append([c.x()-x_size//2, c.y(), c.r()])
        for c in self.white_ft.find_circles(threshold = 2000, x_margin = 10, y_margin = 10, r_margin = 10, r_min = 100, r_max = 200, r_step = 2):
            self.cam_images.draw_circle(c.x()-x_size//2, c.y(), c.r(), color = (0, 0, 255))
            self.white_balls_array.append([c.x(), c.y(), c.r()])

    def count_balls_info(self):  #подсчет координат всех шаров
        for i in range(3):
            self.balls_info.append(self.black_balls_array[i][0])



sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.CIF)   # Set frame size to CIF (352x288)

mode = 1

sensor.skip_frames(time = 2000)     # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.
freq = 15
#uart = UART(3, 9600)

info = Info_send()
while(True):
    clock.tick()                    # Update the FPS clock.
    img = sensor.snapshot().gamma_corr(gamma = 0.85, contrast = 1.3, brightness = 0.06)        # Take a picture and return the image.



    if mode:  #так, это режим линии
        gS = greenSquares(img)

        gS.markers_array()
        #print("m_array =", gS.m_array)
        gS.markers_centres()
        #print(gS.centres_array)
        gS.convert_vertices_order()
        #print("m_array_converted =", gS.m_array_converted)
        gS.count_angles()
        if(len(gS.angles_array)!=0):
            if((gS.angles_array[0])>=0):
                new_img = img.rotation_corr(z_rotation=gS.angles_array[0])

        gS.perimeter_colors()
        #print("perimeter_colors =", gS.perimeter_array)
        #print("perimeter_array =", gS.perimeter_array)
        direction = gS.turn_direction()
        sensor.flush()

        #print("direction =", direction)
        '''lines = Line(img)
        lines.approx_line()



        info.remember_line_data(lines.sqr_avg_deviation)
        info.remember_dir_data(direction)
        print("send_data: ", info.send_data)
        if len(gS.angles_array) != 0:
            sum_angles = 0
            for aa in gS.angles_array:
                sum_angles += aa
            avg_aa = round(sum_angles/len(gS.angles_array))
            info.transmit_line_data(lines.sqr_avg_deviation, direction, avg_aa)
        else:
            info.transmit_line_data(lines.sqr_avg_deviation, direction, 0)
        if len(gS.angles_array) != 0:
            print("angle: ", gS.angles_array)

        time.sleep_ms(5)


    else:
        balls = Balls(img)'''




