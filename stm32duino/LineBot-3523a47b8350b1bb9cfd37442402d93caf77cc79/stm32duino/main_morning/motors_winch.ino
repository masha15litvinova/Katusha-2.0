#include "config.h"

void init_motors_grab()
{
  pinMode(MOTOR1_IN1, OUTPUT);
  pinMode(MOTOR1_IN2, OUTPUT);
  pinMode(MOTOR2_IN1, OUTPUT);
  pinMode(MOTOR2_IN2, OUTPUT);
}
void motors_grab(int m1, int m2)
{
  if (m1 > 255)
  {
    m1 = 255;
  }
  if (m1 < -255)
  {
    m1 = -255;
  }
  if (m2 > 255)
  {
    m2 = 255;
  }
  if (m2 < -255)
  {
    m2 = -255;
  }
  if (m1 > 0)
  {
    analogWrite(MOTOR1_IN1, 255 - m1);
    analogWrite(MOTOR1_IN2, 255);
  }
  else
  {
    analogWrite(MOTOR1_IN1, 255);
    analogWrite(MOTOR1_IN2, 255 + m1);
  }
  if (m2 > 0)
  {
    analogWrite(MOTOR2_IN1, 255 - m2);
    analogWrite(MOTOR2_IN2, 255);
  }
  else
  {
    analogWrite(MOTOR2_IN1, 255);
    analogWrite(MOTOR2_IN2, 255 + m2);
  }
}

void servo_test()
{
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    winchServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    winchServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
