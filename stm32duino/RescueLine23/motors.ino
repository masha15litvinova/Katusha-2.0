#include "pin_config.h"

void motors(int m1, int m2)
{
  //m2 = -m2;
  if(m1>255) m1 = 255;
  if(m1<-255) m1 = -255;
  if(m2>255) m2 = 255;
  if(m2<-255) m2 = -255;

  if(m1>0)
  {
    analogWrite(MOTOR1_2, m1);
    analogWrite(MOTOR1_1, 0);
  }
  else
  {
    analogWrite(MOTOR1_2, 0);
    analogWrite(MOTOR1_1, -m1);
  }

   if(m2>0)
  {
    analogWrite(MOTOR2_1, m2);
    analogWrite(MOTOR2_2, 0);
  }
  else
  {
    analogWrite(MOTOR2_1, 0);
    analogWrite(MOTOR2_2, -m2);
  }
}