#include <Adafruit_TCS34725.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <vl53l0x_class.h>



#define HIGH_SPEED
#define TIMER_FREQ 500000
#define MOTORS_TICK 30000
#define LED_TICK 100000

#define MOTORS_PWM_COMPUTE 3
#define LINE 0