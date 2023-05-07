

void motors(int m1, int m2) {

  if (m1 > 255) m1 = 255;
  if (m1 < -255) m1 = -255;
  if (m2 > 255) m2 = 255;
  if (m2 < -255) m2 = -255;

  if (m1 > 0) {
    analogWrite(MOTOR1_2, m1);
    analogWrite(MOTOR1_1, 0);
  } else if (m1 < 0) {
    analogWrite(MOTOR1_2, 0);
    analogWrite(MOTOR1_1, -m1);
  } else {
    analogWrite(MOTOR1_2, 255);
    analogWrite(MOTOR1_1, 255);
  }

  if (m2 > 0) {
    analogWrite(MOTOR2_1, m2);
    analogWrite(MOTOR2_2, 0);
  } else if (m2 < 0) {
    analogWrite(MOTOR2_1, 0);
    analogWrite(MOTOR2_2, -m2);
  } else {
    analogWrite(MOTOR2_2, 255);
    analogWrite(MOTOR2_1, 255);
  }
}

void sliders(int m1, int m2) {
  if (m1 > slider_pwm_safety_treshold) m1 = slider_pwm_safety_treshold;
  if (m1 < -slider_pwm_safety_treshold) m1 = -slider_pwm_safety_treshold;
  if (m2 > slider_pwm_safety_treshold) m2 = slider_pwm_safety_treshold;
  if (m2 < -slider_pwm_safety_treshold) m2 = -slider_pwm_safety_treshold;

  if (millis() - robot.timeCurrSens > CURRENT_SENS_DELAY) {
    if (analogRead(CURRENT1) > current_max) {
      m2 = 0;
    }
    if (analogRead(CURRENT2) > current_max) {
      m1 = 0;
    }
  }

  if (m1 > 0) {
    analogWrite(MOTOR3_2, m1);
    analogWrite(MOTOR3_1, 0);
    // pwm_start(MOTOR3_1, 16000, m1, RESOLUTION_8B_COMPARE_FORMAT);
    //pwm_start(MOTOR3_2, 16000, 0, RESOLUTION_8B_COMPARE_FORMAT);
    //analogWrite(MOTOR3_1, m1);
    //analogWrite(MOTOR3_2, 0);
  } else {
    analogWrite(MOTOR3_2, 0);
    analogWrite(MOTOR3_1, -m1);
    //pwm_start(MOTOR3_1, 16000, 0, RESOLUTION_8B_COMPARE_FORMAT);
    //pwm_start(MOTOR3_2, 16000, -m1, RESOLUTION_8B_COMPARE_FORMAT);
    //analogWrite(MOTOR3_1, 0);
    //analogWrite(MOTOR3_2, -m1);
  }

  if (m2 > 0) {
    analogWrite(MOTOR4_1, m2);
    analogWrite(MOTOR4_2, 0);
    // pwm_start(MOTOR4_1, 16000, m2, RESOLUTION_8B_COMPARE_FORMAT);
    // pwm_start(MOTOR4_2, 16000, 0, RESOLUTION_8B_COMPARE_FORMAT);

  } else {
    analogWrite(MOTOR4_1, 0);
    analogWrite(MOTOR4_2, -m2);
    // pwm_start(MOTOR4_1, 16000, 0, RESOLUTION_8B_COMPARE_FORMAT);
    //pwm_start(MOTOR4_2, 16000, -m2, RESOLUTION_8B_COMPARE_FORMAT);
  }
}
