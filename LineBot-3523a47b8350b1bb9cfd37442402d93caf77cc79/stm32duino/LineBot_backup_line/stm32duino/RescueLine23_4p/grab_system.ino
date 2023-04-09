


float kp_sliders = 8;
float ki_sliders = 35.0;  //0.05;
float kd_sliders = 0.01;  //0.01;
float prevErr1 = 0;
float prevErr2 = 0;
float dt = 0.005;
float h1 = 0;
float h2 = 0;

int max_target_position = 60;
int max_target_position2 = 60;
int max_err_h1 = 3;



void sliders_movement(int target_position) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Movement start...");
  display.display();


  display.setCursor(0, 10);
  display.println("target: " + String(target_position));
  display.display();

  //target_position2 = max_target_position - target_position;
  h1 = map(analogRead(POTENTIOMETER1), 0, 1023, 0, max_target_position);
  h2 = map(analogRead(POTENTIOMETER2), 0, 1023, 0, max_target_position);

  if (target_position < 0) target_position = 0;
  if (target_position > max_target_position) target_position = max_target_position;
  float err_h1 = (h1 - target_position);
  float err_h2 = (h2 - target_position);
  float integral1 = 0;
  float integral2 = 0;
  while ((abs(err_h1) > max_err_h1) or (abs(err_h2) > max_err_h1)) {
    display.clearDisplay();
    h1 = map(analogRead(POTENTIOMETER1), 0, 1023, 0, max_target_position);
    h2 = map(analogRead(POTENTIOMETER2), 0, 1023, 0, max_target_position);
    int c1 = analogRead(CURRENT1);
    int c2 = analogRead(CURRENT2);
    display.setCursor(0, 20);
    display.println("h1: " + String(h1) + " c1: " + String(c1));
    display.setCursor(0, 30);
    display.println("h2: " + String(h2) + " c2:" + String(c2));
    display.display();
    err_h1 = (h1 - target_position);

    integral1 = integral1 + (float)err_h1 * dt * ki_sliders;
    err_h2 = (h2 - target_position);
    integral2 = integral2 + (float)err_h2 * dt * ki_sliders;
    float D1 = (err_h1 - prevErr1) / dt;
    float D2 = (err_h2 - prevErr2) / dt;
    prevErr1 = err_h1;
    prevErr2 = err_h2;
    float result1 = err_h1 * kp_sliders + integral1 + D1 * kd_sliders;
    float result2 = err_h2 * kp_sliders + integral2 + D2 * kd_sliders;
    //if (abs(result) < 60) result = 0;
    //if(abs(err_h1)<max_err_h1) result1 = 0;
    //if(abs(err_h2)<max_err_h1) result2 = 0;
    sliders(-result2, result1);
  }
  sliders(0, 0);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Movement completed");
  display.display();
  delay(300);
}
void calibration_grab()
{
  int v_grab = 255;
  move_servos_180_up();
  sliders_movement(0);
  sliders(0, 0);
  move_servos_180_down();
}
void move_servos_180_up()
{
  servo2.write(0);
  servo3.write(180);
  for (int i = 0; i < 180; i++)
  {
    servo2.write(i);
    servo3.write(180 - i);
    delay(5);
  }
}
void move_servos_180_down()
{
  servo2.write(180);
  servo3.write(0);
  for (int i = 0; i < 180; i++)
  {
    servo3.write(i);
    servo2.write(180 - i);
    delay(10);
  }
}
void grab()
{
  servo1.write(90);
  delay(3500);
  sliders_movement(10);
  sliders_movement(60);


  delay(2000);
  while (digitalRead(BALL) == 0) {}
  servo1.write(50);
  delay(3000);
  sliders_movement(0);
}
