#include <Adafruit_TCS34725.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <Servo.h>
#include <SmartButton.h>
#include <Robot.h>
#include <EEPROM.h>
#include <GParser.h>

#define LONG_RANGE
#define sensor_lf_newAddress 0xC0
#define sensor_f_newAddress 0x31
#define sensor_rf_newAddress 0x10
#define sensor_r_newAddress 0x3C

#define CAMERA_BYTE_SIGNAL 255
#define CAM_X_SIZE 320
#define CAM_LINE_COUNT 25


#define GYRO_BYTE_SIGNAL 255
#define GYRO_TIME_SIGNAL 254
#define UART_BAUDRATE 115200

#define DEFAULT_START_POSITION 10000

#define GREY_THRESHOLD 90

#define GREEN_HIGH_R 147  //порог зеленого (может, другие цвета тоже нужны)
#define GREEN_LOW_R 130

#define GREEN_HIGH_L 147  //порог зеленого (может, другие цвета тоже нужны)
#define GREEN_LOW_L 130

#define BLUE_RED_HIGH 116





#define RED_R 327
#define GREEN_R 733
#define BLUE_R 700

#define RED_L 349
#define GREEN_L 609
#define BLUE_L 562

#define COLOR_DIST_HIGH 23
#define COLOR_DIST_LOW 15

#define V_CALIBR 0  //140
#define V_MAIN 26
#define V_GORKA_UP 42
#define V_GORKA_DOWN 50  //22
#define V_OBSTACLE 90
#define V_GRAB_BALL 80
#define V_MIN 20
#define V_OBST 40
#define V_WALL 45
#define V_ALIGN 40
#define V_ROTATE_BALLS 45
#define V_EVAC 50

#define MAX_V 98

#define ANGLE_GORKA 16

#define DELAY_CALIBR_4 4000
#define PWM_LEDS 40

#define HIGH_SPEED  //tcs

#define BTN_DELAY 400

#define DIST_THRESHOLD 60
#define DIST_NO_OBJECT 160

#define TIMER_FREQ 500000  //в тиках таймера
#define MOTORS_TICK 6000
#define LED_TICK 100000
#define CAMERA_UART_TICK 4000
#define GYRO_UART_TICK 4000
#define LINE_SENS_TICK 4000

#define DIST2_DELAY 650  //в миллисекундах
#define MOTORS_DELAY 14
#define CAMERA_DELAY 60
#define GYRO_DELAY 8
#define GYRO_DELAY_LINE 12
#define LINE_SENS_DELAY 6
#define CURRENT_SENS_DELAY 600
#define COLORS_DELAY 1500

/**********STATES************************/
#define LINE 0
#define MOTORS_PWM_COMPUTE 1
#define STOP_SCREEN1 2  //line sensors, tcs sensors
#define STOP_SCREEN2 3  //camera and sparkfun data
#define STOP_SCREEN3 4  //laser dist sensors data
#define STOP_SCREEN4 5  //current sensors and sliders position, ball sensor
#define STOP_SCREEN5 6  //encoders
#define STOP_SCREEN0 7  //STOP
#define AFTER_STOP_SCREEN 8

#define CAMERA_READ_DATA 9
#define GYRO_READ_DATA 10
#define LINE_READ_DATA 11
#define DIST2_READ_DATA 12
#define CALIBRATION 13
#define ROTATING_GREEN 14
#define COLOR_READ_DATA 15
#define MOVE_SLIDERS 16
#define OBSTACLE 17
#define TAKE_CUBE 18
#define EVAC_ZONE 19
/*****************************************/

int slider_pwm_safety_treshold = 245;

int min_pwm = 10;
int current_max = 950;

int state_robot = STOP_SCREEN0;
int last_state_robot = LINE;

int dir = 3;
