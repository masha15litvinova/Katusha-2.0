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

#define SerialPort SerialUSB
#define LEDPIN 13


void (*resetFunc)(void) = 0;

MPU9250_DMP imu;

long int time_begin = millis();
long int blink_time = millis();
byte calibrated = 0;
bool send = true;
void setup() {
  //SerialPort.begin(9600);
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(LEDPIN, OUTPUT);

  if (imu.begin() != INV_SUCCESS) {
    while (1) {

      delay(5000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL,
               10);
}
byte angle_y = 0;
byte angle_p = 0;
byte angle_r = 0;
float pitch_robot = 0.0;
float old_pitch_robot = 0.0;
float k_filter = 0.4;
long int time_gorka_start = 0;
void loop() {

  if (imu.fifoAvailable()) {

    if (imu.dmpUpdateFifo() == INV_SUCCESS) {

      imu.computeEulerAngles();
      imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
      printIMUData();
    }
  }
  float accelX = imu.calcAccel(imu.ax);
  float accelY = imu.calcAccel(imu.ay);
  float accelZ = imu.calcAccel(imu.az);
  float gyroX = imu.calcGyro(imu.gx);
  float gyroY = imu.calcGyro(imu.gy);
  float gyroZ = imu.calcGyro(imu.gz);
  float magX = imu.calcMag(imu.mx);
  float magY = imu.calcMag(imu.my);
  float magZ = imu.calcMag(imu.mz);
  pitch_robot = (atan((accelY) / (accelZ)) * 180 / PI);
  float p = 0;
  p = pitch_robot * k_filter + old_pitch_robot * (1 - k_filter);
  Serial.println(String(imu.roll) + " " + String(imu.pitch) + " " + String(imu.yaw));
  if (abs(p) > 7) {
    if ((p < 20) and (p > 0)) p = 20;
    else if ((p < -20) and (p < 0)) p = -20;
  }

  if (abs(gyroZ) > 60) p = 0;

  byte send_pitch = map(round(p), -180, 180, 0, 255);

  if ((millis() - time_begin) > 30000) calibrated = 1;
  if (Serial1.available()) {
    byte signal = Serial1.read();
    if (signal == 1) {
      resetFunc();
    }
    if (signal == 2) {
      send = false;
    }
    if (signal == 3) {
      send = true;
    }
  }
  if (send) {
    if ((millis() - blink_time) > 30) {
      digitalWrite(LEDPIN, HIGH);
      blink_time = millis();
    }
    Serial1.print(':');
    delay(8);
    Serial1.print(angle_y);
    delay(8);
    Serial1.print('/');
    delay(8);
    Serial1.print(send_pitch);
    delay(8);
    Serial1.print('/');
    delay(8);
    Serial1.print(calibrated);
    delay(8);
    Serial1.print('/');
    delay(8);
    Serial1.print(';');
    delay(8);
    if ((millis() - blink_time) > 30) {
      digitalWrite(LEDPIN, LOW);
      blink_time = millis();
    }
  }

  old_pitch_robot = pitch_robot;
}

void printIMUData(void) {

  float q0 = imu.calcQuat(imu.qw);
  float q1 = imu.calcQuat(imu.qx);
  float q2 = imu.calcQuat(imu.qy);
  float q3 = imu.calcQuat(imu.qz);

  int yaw1 = imu.yaw;
  int pitch1 = imu.pitch;
  int roll1 = imu.roll;

  angle_y = map(yaw1, 0, 360, 0, 255);
  angle_p = map(pitch1, 0, 360, 0, 255);
  angle_r = map(roll1, 0, 360, 0, 255);
}
