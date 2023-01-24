#include <Adafruit_TCS34725.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <vl53l0x_class.h>
#include <Servo.h>
#include <SmartButton.h>



#define V_CALIBR 80
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

#define DIST2_DELAY 500  //в миллисекундах
#define MOTORS_DELAY 20
#define CAMERA_DELAY 50
#define GYRO_DELAY 50
#define LINE_SENS_DELAY 40

/**********STATES************************/
#define LINE 0
#define MOTORS_PWM_COMPUTE 1
#define STOP_SCREEN1 2       //line sensors, tcs sensors
#define STOP_SCREEN2 3       //camera and sparkfun data
#define STOP_SCREEN3 4       //laser dist sensors data
#define STOP_SCREEN4 5       //current sensors and sliders position, ball sensor
#define STOP_SCREEN5 6       //encoders
#define STOP_SCREEN0 7       //STOP
#define AFTER_STOP_SCREEN 8  //clear display
#define CAMERA_READ_DATA 9
#define GYRO_READ_DATA 10
#define LINE_SENS_READ 11
#define DIST2_SENS_READ 12
#define CALIBRATION 13
/*****************************************/

int slider_pwm_safety_treshold = 200;

int state_robot = CALIBRATION;
int last_state_robot = LINE;



