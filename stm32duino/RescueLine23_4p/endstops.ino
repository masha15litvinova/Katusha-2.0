void initEndstopIRQs()
{
  attachInterrupt(digitalPinToInterrupt(ENDSTOP1), endstop1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENDSTOP2), endstop2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENDSTOP3), endstop3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENDSTOP4), endstop4, CHANGE);
}

void endstop1()
{

  motors(0, 0);
  motors(-170, -30);
  long int time_begin = millis();
  while ((millis() - time_begin) < 80) {}
  motors(0, 0);
}
void endstop2()
{
  state = !state;
}
void endstop3()
{
  motors(0, 0);
  motors(-30, -170);
  long int time_begin = millis();
  while ((millis() - time_begin) < 80) {}
  motors(0, 0);
}
void endstop4()
{
  state = !state;
}
