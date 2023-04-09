/************************************************************
MPU9250_DMP_Quaternion
 Quaternion example for MPU-9250 DMP Arduino Library 
Jim Lindblom @ SparkFun Electronics
original creation date: November 23, 2016
https://github.com/sparkfun/SparkFun_MPU9250_DMP_Arduino_Library

The MPU-9250's digital motion processor (DMP) can calculate
four unit quaternions, which can be used to represent the
rotation of an object.

This exmaple demonstrates how to configure the DMP to 
calculate quaternions, and prints them out to the serial
monitor. It also calculates pitch, roll, and yaw from those
values.

Development environment specifics:
Arduino IDE 1.6.12
SparkFun 9DoF Razor IMU M0

Supported Platforms:
- ATSAMD21 (Arduino Zero, SparkFun SAMD21 Breakouts)
*************************************************************/
#include <SparkFunMPU9250-DMP.h>
#include <SoftwareSerial.h>



#define SerialPort SerialUSB
#define LEDPIN 13

#define TIME_DELAY 10

void (*resetFunc)(void) = 0;

MPU9250_DMP imu;

bool state = 1;

//SoftwareSerial mySerial(13, 12);  // RX, TX

unsigned long time_begin = millis();
unsigned long time_led = millis();
byte calibrated = 0;
bool send = true;
void setup() {
  //SerialPort.begin(9600);
  Serial1.begin(9600);
  //mySerial.begin(9600);
}
void loop() {
  //mySerial.write(1);
  delay(500);
}
