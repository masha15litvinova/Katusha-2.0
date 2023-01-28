
#define SPEED_COEFF_P 0.9  //1.0
#define SPEED_COEFF_D 0    //0.8//0.8//0.8//0.3  //13
#define SPEED_COEFF_I 0.06
#define SPEED_COEFF_CUBE 0

#define CPR 1050
#define RPM1 148
#define RPM2 155

#define ERR_LIM 4
#define U_MAX 2048
long int encoder1 = 0;
long int encoder2 = 0;


float u1 = 0;

float up1 = 0;
float ud1 = 0;
float ui1 = 0;

float err1 = 0;
float err1_old = 0;

float current_speed1 = 0;

long int prev_enc1 = 0;

float u2 = 0;

float up2 = 0;
float ud2 = 0;
float ui2 = 0;

float err2 = 0;
float err2_old = 0;

float current_speed2 = 0;

long int prev_enc2 = 0;

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
  return current_speed1;
}
float Err1() {
  return err1;
}
float U1() {
  return u1;
}
float CurrentSpeed2() {
  return current_speed2;
}
float Err2() {
  return err2;
}
float U2() {
  return u2;
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
  current_speed1 = ((Enc1() - prev_enc1) * 60000 / ((float)CPR * MOTORS_DELAY));
  err1 = -current_speed1 + (float)sp_rpm;

  up1 = SPEED_COEFF_P * err1;
  ui1 = ui1 + SPEED_COEFF_I * err1;
  ud1 = (err1 - err1_old) * SPEED_COEFF_D;

  if (abs(err1) < ERR_LIM) {
    ui1 = 0;
  }
  u1 = up1 + ui1 + ud1;
  if (u1 > U_MAX) u1 = U_MAX;
  else if (u1 < -U_MAX) u1 = -U_MAX;

  int v1 = map(sp_rpm + round(u1), 0, RPM1, 0, 255);
  prev_enc1 = Enc1();
  err1_old = err1;
  return v1;
}

int vel2(float speed) {
  // state = !state;
  int sp_rpm = SpeedRPM2(speed);
  current_speed2 = ((Enc2() - prev_enc2) * 60 / (float)CPR) / ((float)MOTORS_TICK / (float)TIMER_FREQ);
  err2 = -current_speed2 + (float)sp_rpm;

  up2 = SPEED_COEFF_P * err2;
  ui2 = ui2 + SPEED_COEFF_I * err2;
  ud2 = (err2 - err2_old) * SPEED_COEFF_D;

  if (abs(err2) < ERR_LIM) {
    ui2 = 0;
  }
  u2 = up2 + ui2 + ud2;
  if (u2 > U_MAX) u2 = U_MAX;
  else if (u2 < -U_MAX) u2 = -U_MAX;

  int v2 = map(sp_rpm + round(u2), 0, RPM2, 0, 255);
  prev_enc2 = Enc2();
  err2_old = err2;
  return v2;
}
