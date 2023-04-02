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

#define TIME_DELAY 12

void (*resetFunc)(void) = 0;

MPU9250_DMP imu;

bool state = 1;
float drift = 0;
unsigned long time_begin = millis();
unsigned long time_led = millis();
unsigned long time_integrated = millis();
byte calibrated = 0;
bool send = true;
void setup() {
  //SerialPort.begin(9600);
  Serial1.begin(9600);

  pinMode(LEDPIN, OUTPUT);
  // Call imu.begin() to verify communication and initialize
  if (imu.begin() != INV_SUCCESS) {
    while (1) {

      delay(5000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT |  // Enable 6-axis quat
                 DMP_FEATURE_GYRO_CAL,   // Use gyro calibration
               10);                      // Set DMP FIFO rate to 10 Hz
  // DMP_FEATURE_LP_QUAT can also be used. It uses the
  // accelerometer in low-power mode to estimate quat's.
  // DMP_FEATURE_LP_QUAT and 6X_LP_QUAT are mutually exclusive
}
int count = 0;

float p = 0;
float p1 = 0.0;
byte send_pitch = 0;
byte angle_y = 0;
byte angle_p = 0;
byte angle_r = 0;
float pitch_robot = 0.0;
float last_p = 0.0;
float last_last_p = 0.0;
float old_pitch_robot = 0.0;
float k_filter = 0.55;
long int time_gorka_start = 0;
void loop() {
  // Check for new data in the FIFO
  if (imu.fifoAvailable()) {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if (imu.dmpUpdateFifo() == INV_SUCCESS) {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      imu.computeEulerAngles();
      imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
      printIMUData();
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
      accelY = constrain(accelY, -1.0, 1.0);
      //p = pitch_robot * k_filter + old_pitch_robot * (1 - k_filter);
      /*if (count > 199) {
        p = p + (gyroX - drift) * (millis() - time_integrated) * 0.001;
      } else {
        p = 0;
      }*/
      time_integrated = millis();


      /*if (abs(p) > 7) {
        if ((p < 20) and (p > 0)) p = 20;
        else if ((p < -20) and (p < 0)) p = -20;
      }*/
      if (p > 80) p = 80;
      else if (p < -80) p = -80;
      //if (abs(gyroZ) > 60) p = 0;

      send_pitch = map(round(p1), -180, 180, 0, 255);
      if (count < 200) {
        drift = drift + gyroX * 0.005;
        count++;
      }
      p = ((acos(accelY) * 180) / PI) - 90.0;

      //float middle = (p < last_p) ? ((last_p < last_last_p) ? last_p : ((last_last_p < p) ? p : last_last_p)) : ((p < last_last_p) ? p : ((last_last_p < last_p) ? last_p : last_last_p));
      p1 = (p)*k_filter + p1 * (1.0 - k_filter);
      /*SerialPort.print(p);
      SerialPort.print(",");
      SerialPort.println(p1);*/
      // SerialPort.println(accelY);
      last_last_p = last_p;
      last_p = p1;
    }
  }



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
    if ((millis() - time_led) > 90) {
      digitalWrite(LEDPIN, !state);
      time_led = millis();
    }
    Serial1.print(':');
    delay(TIME_DELAY);
    Serial1.print(angle_y);
    delay(TIME_DELAY);
    Serial1.print('/');
    delay(TIME_DELAY);
    Serial1.print(send_pitch);
    delay(TIME_DELAY);
    Serial1.print('/');
    delay(TIME_DELAY);
    Serial1.print(calibrated);
    delay(TIME_DELAY);
    Serial1.print('/');
    delay(TIME_DELAY);
    Serial1.print(';');
    delay(TIME_DELAY);
    digitalWrite(LEDPIN, LOW);
  }  //*/
  //SerialPort.println("p = " + String(p) + " drift = " + String(drift));
  old_pitch_robot = pitch_robot;
}

void printIMUData(void) {
  digitalWrite(LEDPIN, OUTPUT);
  // After calling dmpUpdateFifo() the ax, gx, mx, etc. values
  // are all updated.
  // Quaternion values are, by default, stored in Q30 long
  // format. calcQuat turns them into a float between -1 and 1
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
