#define SPEED_COEFF_P 3.2//4.5   //1.0
#define SPEED_COEFF_D 0.09//0.5   //0.8//0.8//0.8//0.3  //13
#define SPEED_COEFF_I 0.7//0.08//0.043  //0.06
#define SPEED_COEFF_CUBE 0

#define CPR 1050
#define RPM1 132
#define RPM2 155

#define ERR_LIM 4
#define U_MAX 2048
unsigned long  encoder1 = 0;
unsigned long encoder2 = 0;

void initEncoderIRQs() {
  attachInterrupt(digitalPinToInterrupt(ENC_INT1), enc1, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC_INT2), enc2, FALLING);
}

void enc1() {
  if (digitalRead(ENC1) == HIGH) encoder1--;
  else encoder1++;
}
int Enc1() {
  return encoder1;
}
void enc2() {
  if (digitalRead(ENC2) == HIGH) encoder2++;
  else encoder2--;
}
int Enc2() {
  return encoder2;
}
float CurrentSpeed1() {
  return robot.current_speed1;
}
float Err1() {
  return robot.err1;
}

float CurrentSpeed2() {
  return robot.current_speed2;
}
float Err2() {
  return robot.err2;
}

int SpeedRPM1(float speed) {
  int speed_rpm = map(round(speed), 0, 255, 0, RPM1);
  return speed_rpm;
}
int SpeedRPM2(float speed) {
  int speed_rpm = map(round(speed), 0, 255, 0, RPM2);
  return speed_rpm;
}

int vel1(float speed) {
  // state = !state;
  int sp_rpm = SpeedRPM1(speed);
  int delta_time = millis() - robot.timeMotor1;
  robot.current_speed1 = ((Enc1() - robot.prev_enc1) * 60000 / ((float)CPR * delta_time));
  robot.err1 = -robot.current_speed1 + (float)sp_rpm;

  robot.up1 = SPEED_COEFF_P * robot.err1;
  robot.ui1 = robot.ui1 + SPEED_COEFF_I * robot.err1;
  robot.ud1 = (robot.err1 - robot.err1_old) * SPEED_COEFF_D;

  if (abs(robot.err1) < ERR_LIM) {
    robot.ui1 = 0;
  }
  robot.u1 = robot.up1 + robot.ui1 + robot.ud1;
  if (robot.u1 > U_MAX) robot.u1 = U_MAX;
  else if (robot.u1 < -U_MAX) robot.u1 = -U_MAX;

  int v1 = map(sp_rpm + round(robot.u1), 0, RPM1, 0, 255);
  robot.prev_enc1 = Enc1();
  robot.err1_old = robot.err1;
  robot.timeMotor1 = millis();
  return v1;
}

int vel2(float speed) {
  // state = !state;
  int sp_rpm = SpeedRPM2(speed);
  int delta_time = millis() - robot.timeMotor2;
  robot.current_speed2 = ((Enc2() - robot.prev_enc2) * 60000 / ((float)CPR * delta_time));
  robot.err2 = -robot.current_speed2 + (float)sp_rpm;

  robot.up2 = SPEED_COEFF_P * robot.err2;
  robot.ui2 = robot.ui2 + SPEED_COEFF_I * robot.err2;
  robot.ud2 = (robot.err2 - robot.err2_old) * SPEED_COEFF_D;

  if (abs(robot.err2) < ERR_LIM) {
    robot.ui2 = 0;
  }
  robot.u2 = robot.up2 + robot.ui2 + robot.ud2;
  if (robot.u2 > U_MAX) robot.u2 = U_MAX;
  else if (robot.u2 < -U_MAX) robot.u2 = -U_MAX;

  int v2 = map(sp_rpm + round(robot.u2), 0, RPM2, 0, 255);
  robot.prev_enc2 = Enc2();
  robot.err2_old = robot.err2;
  robot.timeMotor2 = millis();
  return v2;
}
