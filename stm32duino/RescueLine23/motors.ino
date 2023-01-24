


void motors(int m1, int m2)
{
  
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

void sliders(int m1, int m2)
{
  if (m1 > slider_pwm_safety_treshold) m1 = slider_pwm_safety_treshold;
  if (m1<-slider_pwm_safety_treshold) m1 = -slider_pwm_safety_treshold;
  if (m2>slider_pwm_safety_treshold) m2 = slider_pwm_safety_treshold;
  if (m2<-slider_pwm_safety_treshold) m2 = -slider_pwm_safety_treshold;

  if(m1>0)
  {
    analogWrite(MOTOR3_1, m1);
    analogWrite(MOTOR3_2, 0);
  }
  else
  {
    analogWrite(MOTOR3_1, 0);
    analogWrite(MOTOR3_2, -m1);
  }

   if(m2>0)
  {
    analogWrite(MOTOR4_1, m2);
    analogWrite(MOTOR4_2, 0);
  }
  else
  {
    analogWrite(MOTOR4_1, 0);
    analogWrite(MOTOR4_2, -m2);
  }
}
