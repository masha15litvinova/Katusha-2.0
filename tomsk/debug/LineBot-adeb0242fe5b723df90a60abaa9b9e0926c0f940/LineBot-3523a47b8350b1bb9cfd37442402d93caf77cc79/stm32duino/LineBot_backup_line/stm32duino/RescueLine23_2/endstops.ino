void initEndstopIRQs()
{
  attachInterrupt(digitalPinToInterrupt(ENDSTOP1), endstop1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENDSTOP2), endstop2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENDSTOP3), endstop3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENDSTOP4), endstop4, CHANGE);
}

void endstop1()
{
  state = !state;
}
void endstop2()
{
  state = !state;
}
void endstop3()
{
  state = !state;
}
void endstop4()
{
  state = !state;
}
