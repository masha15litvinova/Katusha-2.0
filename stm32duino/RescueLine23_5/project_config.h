#include <Adafruit_TCS34725.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <vl53l0x_class.h>
#include <Servo.h>
#include <SmartButton.h>
#include <Robot.h>

#define GYRO_BYTE_SIGNAL 255
#define GYRO_TIME_SIGNAL 254
#define UART_BAUDRATE 115200

#define DEFAULT_START_POSITION 10000

#define GREY_THRESHOLD 90

#define GREEN_HIGH_R 138 //порог зеленого (может, другие цвета тоже нужны)
#define GREEN_LOW_R 105

#define GREEN_HIGH_L 135 //порог зеленого (может, другие цвета тоже нужны)
#define GREEN_LOW_L 105

#define BLUE_RED_HIGH 116

#define V_CALIBR 0//140
#define V_MAIN 45
#define V_GORKA_UP 95
#define V_GORKA_DOWN 35
#define V_OBSTACLE 90
#define V_GRAB_BALL 80
#define DELAY_CALIBR_4 4000
#define PWM_LEDS 100

#define HIGH_SPEED  //tcs

#define BTN_DELAY 400

#define TIMER_FREQ 500000  //в тиках таймера
#define MOTORS_TICK 6000
#define LED_TICK 100000
#define CAMERA_UART_TICK 4000
#define GYRO_UART_TICK 4000
#define LINE_SENS_TICK 4000

#define DIST2_DELAY 650  //в миллисекундах
#define MOTORS_DELAY 16
#define CAMERA_DELAY 50
#define GYRO_DELAY 8
#define GYRO_DELAY_LINE 7
#define LINE_SENS_DELAY 14
#define CURRENT_SENS_DELAY 600

/**********STATES************************/
#define LINE 0
#define MOTORS_PWM_COMPUTE 1
#define STOP_SCREEN1 2       //line sensors, tcs sensors
#define STOP_SCREEN2 3       //camera and sparkfun data
#define STOP_SCREEN3 4       //laser dist sensors data
#define STOP_SCREEN4 5       //current sensors and sliders position, ball sensor
#define STOP_SCREEN5 6       //encoders
#define STOP_SCREEN0 7       //STOP
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
/*****************************************/

int slider_pwm_safety_treshold = 255;

int min_pwm = 0;
int current_max = 1023;

int state_robot = STOP_SCREEN0;
int last_state_robot = LINE;

int dir = 0;
