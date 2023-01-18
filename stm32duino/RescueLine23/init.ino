#include "pin_config.h"

void resetGyro() {

  digitalWrite(GYRO_RST, LOW);
  delay(500);
  digitalWrite(GYRO_RST, HIGH);
}

void resetCamera() {
  digitalWrite(CAMERA_RST, LOW);
  delay(500);
  digitalWrite(CAMERA_RST, HIGH);
}


void resetLaserDist() {
  digitalWrite(XSHUT1, LOW);
  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT3, LOW);
  digitalWrite(XSHUT4, LOW);
  delay(10);
  digitalWrite(XSHUT1, HIGH);
  digitalWrite(XSHUT2, HIGH);
  digitalWrite(XSHUT3, HIGH);
  digitalWrite(XSHUT4, HIGH);
  delay(10);

  digitalWrite(XSHUT1, LOW);
  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT3, LOW);
  digitalWrite(XSHUT4, LOW);
}
void initPins() {
  pinMode(GYRO_RST, OUTPUT);
  pinMode(CAMERA_RST, OUTPUT);
  pinMode(RX3, INPUT_PULLDOWN);
  pinMode(GYRO_INTERRUPT, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(ENC1, INPUT);
  pinMode(ENC2, INPUT);
  pinMode(ENC3, INPUT);
  pinMode(ENC4, INPUT);

  pinMode(ENC_INT1, INPUT);
  pinMode(ENC_INT2, INPUT);
  pinMode(ENC_INT3, INPUT);
  pinMode(ENC_INT4, INPUT);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);

  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);
  pinMode(SENSOR6, INPUT);

  pinMode(CURRENT1, INPUT);
  pinMode(CURRENT2, INPUT);

  pinMode(POTENTIOMETER1, INPUT);
  pinMode(POTENTIOMETER2, INPUT);

  pinMode(MOTOR1_1, OUTPUT);
  pinMode(MOTOR1_2, OUTPUT);
  pinMode(MOTOR2_1, OUTPUT);
  pinMode(MOTOR2_2, OUTPUT);
  pinMode(MOTOR3_1, OUTPUT);
  pinMode(MOTOR3_2, OUTPUT);
  pinMode(MOTOR4_1, OUTPUT);
  pinMode(MOTOR4_2, OUTPUT);

  pinMode(PWM_LIGHTS, OUTPUT);
  analogWriteFrequency(16000);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  pinMode(XSHUT3, OUTPUT);
  pinMode(XSHUT4, OUTPUT);

  pinMode(SWITCH1, OUTPUT);
  pinMode(SWITCH2, OUTPUT);
  pinMode(SWITCH3, OUTPUT);
  pinMode(SWITCH4, OUTPUT);

  digitalWrite(SWITCH1, LOW);
  digitalWrite(SWITCH2, LOW);
  digitalWrite(SWITCH3, LOW);
  digitalWrite(SWITCH4, LOW);
}


void ledBlinking() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(LED1, HIGH);
    delay(75);
    digitalWrite(LED1, LOW);
    delay(75);
  }
  digitalWrite(LED1, HIGH);
  delay(400);
  digitalWrite(LED1, LOW);
}