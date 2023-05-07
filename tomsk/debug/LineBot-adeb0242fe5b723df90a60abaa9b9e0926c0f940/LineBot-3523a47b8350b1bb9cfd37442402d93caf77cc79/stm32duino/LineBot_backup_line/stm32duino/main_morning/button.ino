void init_button()
{
  pinMode(MCU_BTN, INPUT);
}

void wait_button()
{
  while (digitalRead(MCU_BTN) == 1)
  {
    delay(1);
    send_motors(0, 0, 0, 0);
  }

}

void left_correction()
{
int sp = 50;
send_motors(-sp, sp, sp, -sp);
delay(400);
send_motors(sp, sp, sp, sp);
delay(400);
send_motors(0, 0, 0, 0);
  }
  void right_correction()
{
int sp = 50;
send_motors(-sp, sp, sp, -sp);
delay(400);
send_motors(-sp, -sp, -sp, -sp);
delay(400);
send_motors(0, 0, 0, 0);
  }
