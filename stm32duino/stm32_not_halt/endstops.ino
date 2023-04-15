void initEndstopIRQs()
{
  attachInterrupt(digitalPinToInterrupt(ENDSTOP1), endstop_1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENDSTOP2), endstop_2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENDSTOP3), endstop_3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENDSTOP4), endstop_4, CHANGE);
}

void endstop_1()
{

  motors(0, 0);
  motors(-170, -30);
  long int time_begin = millis();
  while ((millis() - time_begin) < 60) {}
  motors(0, 0);
}
void endstop_2()
{
  state = !state;
}
void endstop_3()
{
  motors(0, 0);
  motors(-30, -170);
  long int time_begin = millis();
  while ((millis() - time_begin) < 60) {}
  motors(0, 0);
}
void endstop_4()
{
  state = !state;
}
