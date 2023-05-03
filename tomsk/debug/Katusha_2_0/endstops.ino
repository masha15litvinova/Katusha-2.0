void initEndstopIRQs() {
  attachInterrupt(digitalPinToInterrupt(ENDSTOP1), endstop1_irq, RISING);
  //attachInterrupt(digitalPinToInterrupt(ENDSTOP2), endstop2_irq, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENDSTOP3), endstop3_irq, RISING);
  //attachInterrupt(digitalPinToInterrupt(ENDSTOP4), endstop4_irq, CHANGE);
}
bool endstop1 = false;
bool endstop3 = false;
void endstop1_irq() {
  endstop1 = !endstop1;
  if (robot.mode_endstops == 0) {
    motors(0, 0);
    motors(-170, -30);
    long int time_begin = millis();
    while ((millis() - time_begin) < 60) {}
    motors(0, 0);
  }
}
void endstop2_irq() {
  state = !state;
}
void endstop3_irq() {
  endstop3 = !endstop3;
  if (robot.mode_endstops == 0) {
    motors(0, 0);
    motors(-30, -170);
    long int time_begin = millis();
    while ((millis() - time_begin) < 60) {}
    motors(0, 0);
  }
}
void endstop4_irq() {
  state = !state;
}
