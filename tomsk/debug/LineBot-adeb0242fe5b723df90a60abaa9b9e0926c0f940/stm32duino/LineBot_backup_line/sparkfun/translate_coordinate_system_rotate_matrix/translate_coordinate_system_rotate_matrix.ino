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

MPU9250_DMP imu;

void setup() {
  SerialPort.begin(115200);

  // Call imu.begin() to verify communication and initialize
  if (imu.begin() != INV_SUCCESS) {
    while (1) {
      SerialPort.println("Unable to communicate with MPU-9250");
      SerialPort.println("Check connections, and try again.");
      SerialPort.println();
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

float yaw_start = 0;
float roll_start = 0;
float pitch_start = 0;
float roll_0, pitch_0, yaw_0, pitch_corrected;
bool calibrated = false;
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
  yaw_0 = conv_angle(imu.yaw, yaw_start) * PI / 180;
  roll_0 = conv_angle(imu.roll, roll_start) * PI / 180;
  pitch_0 = conv_angle(imu.pitch, pitch_start) * PI / 180;

  float x0 = 1.0;
  float y0 = 0;
  float z0 = 0;

  float x1 = x0;
  float y1 = y0 * cos(roll_0) - z0 * sin(roll_0);
  float z1 = y0 * sin(roll_0) + z0 * cos(roll_0);

  float x2 = x1 * cos(pitch_0) + z1 * sin(pitch_0);
  float y2 = y1;
  float z2 = -x1 * sin(pitch_0) + z1 * cos(pitch_0);

  float x3 = x2 * cos(yaw_0) - y2 * sin(yaw_0);
  float y3 = x2 * sin(yaw_0) + y2 * cos(yaw_0);
  float z3 = z2;

   pitch_corrected = 0;  //asin(z3) * 180 / PI;  //asin(z3) * 180 / PI;  //vectors_angle(1, 0, 0, x3, y3, z3);

  rotate_matrix();
  if (calibrated) {
    SerialPort.println("[x, y, z] = [" + String(x3) + " " + String(y3) + " " + String(z3) + "]  " + "[roll, pitch, yaw] = [" + String(roll_0) + " " + String(pitch_0) + " " + String(yaw_0) + "]" + " pitch_corrected = " + String(pitch_corrected));
    //SerialPort.println("[roll, pitch, yaw] = [" + String(roll_0) + " " + String(pitch_0) + " " + String(yaw_0) + "]");
    //Serial.println("Y_coord_scale: "+String(round(y3)));
    //SerialPort.println(pitch_corrected);
  } else {
    SerialPort.println("[roll, pitch, yaw] = [" + String(roll_0) + " " + String(pitch_0) + " " + String(yaw_0) + "]");
  }
  if ((SerialPort.available()) and !calibrated) {
    roll_start = imu.roll;
    pitch_start = imu.pitch;
    yaw_start = imu.yaw;
    calibrated = true;
  }
}

void printIMUData(void) {
  // After calling dmpUpdateFifo() the ax, gx, mx, etc. values
  // are all updated.
  // Quaternion values are, by default, stored in Q30 long
  // format. calcQuat turns them into a float between -1 and 1
  float q0 = imu.calcQuat(imu.qw);
  float q1 = imu.calcQuat(imu.qx);
  float q2 = imu.calcQuat(imu.qy);
  float q3 = imu.calcQuat(imu.qz);

  /*SerialPort.println("Q: " + String(q0, 4) + ", " +
                    String(q1, 4) + ", " + String(q2, 4) + 
                    ", " + String(q3, 4));
  SerialPort.println("R/P/Y: " + String(imu.roll) + ", "
            + String(imu.pitch) + ", " + String(imu.yaw));
  SerialPort.println("Time: " + String(imu.time) + " ms");
  SerialPort.println();*/
}
int vectors_angle(float x1, float y1, float z1, float x2, float y2, float z2) {
  int a = round(acos((x1 * x2 + y1 * y2 + z1 * z2) / (sqrt(x1 * x1 + y1 * y1 + z1 * z1) * sqrt(x2 * x2 + y2 * y2 + z2 * z2))) * 180 / PI);
  return a;
}
int conv_angle(int angle, int start) {
  int x1 = -angle + start;
  int x2 = -angle + start + 360;
  int x3 = -angle + start - 360;

  int minimum = 10000;
  if (abs(x1) < minimum) minimum = abs(x1);
  if (abs(x2) < minimum) minimum = abs(x2);
  if (abs(x3) < minimum) minimum = abs(x3);

  if (abs(x1) == minimum) return x1;
  if (abs(x2) == minimum) return x2;
  if (abs(x3) == minimum) return x3;
  return 0;
}

void rotate_matrix() {
  float r11 = 0;  //First row
  float r12 = 0;
  float r13 = 0;
  float r21 = 0;  //Second row
  float r22 = 0;
  float r23 = 0;
  float r31 = 0;  //Third row
  float r32 = 0;
  float r33 = 0;

  double su = sin(roll_0);
  double cu = cos(roll_0);
  double sv = sin(pitch_0);
  double cv = cos(pitch_0);
  double sw = sin(yaw_0);
  double cw = cos(yaw_0);

  r11 = cv * cw;
  r12 = su * sv * cw - cu * sw;
  r13 = su * sw + cu * sv * cw;
  r21 = cv * sw;
  r22 = cu * cw + su * sv * sw;
  r23 = cu * sv * sw - su * cw;
  r31 = -sv;
  r32 = su * cv;
  r33 = cu * cv;


  pitch_corrected = -asin(r31);  //Pitch

  //Gymbal lock: pitch = -90
  /*if (A.r31 == 1) {
    angle[0] = 0.0;                         //yaw = 0
    angle[2] = Math.atan2(-A.r12, -A.r13);  //Roll
    System.out.println("Gimbal lock: pitch = -90");
  }

  //Gymbal lock: pitch = 90
  else if (A.r31 == -1) {
    angle[0] = 0.0;                       //yaw = 0
    angle[2] = Math.atan2(A.r12, A.r13);  //Roll
    System.out.println("Gimbal lock: pitch = 90");
  }
  //General solution
  else {
    angle[0] = Math.atan2(A.r21, A.r11);
    angle[2] = Math.atan2(A.r32, A.r33);
    System.out.println("No gimbal lock");
  }
  return angle;  //Euler angles in order yaw, pitch, roll*/
}
