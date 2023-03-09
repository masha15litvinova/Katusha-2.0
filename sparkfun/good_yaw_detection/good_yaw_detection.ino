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
byte calibrated = 0;

void setup() {
  SerialPort.begin(9600);
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
byte angle_y = 0;
byte angle_p = 0;
void loop() {
  // Check for new data in the FIFO
  if (imu.fifoAvailable()) {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if (imu.dmpUpdateFifo() == INV_SUCCESS) {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      imu.computeEulerAngles();
      printIMUData();
    }
  }
  
  if ((millis() - time_begin) > 30000) calibrated = 1;
  //char buff[8] = { ':', y, '/', p, '/', (char)calibrated, '/', ';' };
  /*if (Serial1.availableForWrite()) {
    //Serial1.println(":" + String(angle_y) + "/" + String(angle_p) + "/" + ";");
    Serial1.print(':');
  }
  if (Serial1.availableForWrite()) { Serial1.print(angle_y); }
  if (Serial1.availableForWrite()) { Serial1.print('/'); }
  if (Serial1.availableForWrite()) { Serial1.print(angle_p); }
  if (Serial1.availableForWrite()) { Serial1.print('/'); }
  if (Serial1.availableForWrite()) { Serial1.print(';'); }*/
  /*if (Serial1.availableForWrite())
  {
    Serial1.write(buff, 6);
  }*/
  if (Serial1.available()) {
    byte rst = Serial1.read();
    if (rst == 1) {
      resetFunc();
    }
  }
  //Serial1.write(buff, 8);
  //String send = ":" + String(angle_y) + "/" + String(angle_p) + "/" + String(calibrated) + "/;";
  

  
  /*for (int i = 0; i < 8; i++) {
    Serial1.print(buff[i]);
    Serial1.flush();
    //delay(5);
  }*/
  //Serial1.flush();
  //delay(10);
  Serial1.print(':');
  delay(10);
  Serial1.print(angle_y);
  delay(10);
  Serial1.print('/');
  delay(12);
  Serial1.print(angle_p);
  delay(10);
  Serial1.print('/');
  delay(10);
  Serial1.print(calibrated);
  delay(10);
  Serial1.print('/');
  delay(10);
  Serial1.print(';');
  delay(10);
  /*Serial1.write(':');
  delay(5);
  Serial1.write((angle_y));
  delay(5);
  Serial1.write('/');
  delay(5);
  Serial1.write((angle_p));
  delay(5);
  Serial1.write('/');
  delay(5);
  Serial1.write((calibrated));
  delay(5);
  Serial1.write('/');
  delay(5);
  Serial1.write(';');
  delay(5);*/
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

  angle_y = map(yaw1, 0, 360, 0, 255);
  angle_p = map(pitch1, 0, 360, 0, 255);
}
