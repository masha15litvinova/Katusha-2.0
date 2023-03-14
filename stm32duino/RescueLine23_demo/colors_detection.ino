int direction_color() {
  int dir1 = 3;
  motors(0, 0);
  int e1 = Enc1();
  while ((Enc1() - e1) > -40) {
    motors(-60, -60);
  }
  motors(0, 0);
  
  //0 - разворот на 180
  //1 - направо
  //2 - налево
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Green markers...");
  display.display();
  uint16_t r1, g1, b1, c1;
  uint16_t r2, g2, b2, c2;
  tcs1.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
  delay(300);  // Delay for one old integ. time period (to finish old reading)
  delay(154);  // Delay for one new integ. time period (to allow new reading)
  tcs1.getRawData(&r1, &g1, &b1, &c1);
  digitalWrite(LED2, LOW);
  display.setCursor(0, 20);
  display.print("Right: " + String(g1));
  tcs2.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
  delay(300);  // Delay for one old integ. time period (to finish old reading)
  delay(154);  // Delay for one new integ. time period (to allow new reading)
  tcs2.getRawData(&r2, &g2, &b2, &c2);
  digitalWrite(LED2, HIGH);
  display.setCursor(0, 30);
  display.print("Left: " + String(g2));
  display.display();
  display.setTextSize(2);
  display.setCursor(0, 50);
  if ((g1 < GREEN_HIGH_R) and (g1 > GREEN_LOW_R) and (g2 < GREEN_HIGH_L) and (g2 > GREEN_LOW_L) and (r2 < BLUE_RED_HIGH) and (r1 < BLUE_RED_HIGH) and (b2 < BLUE_RED_HIGH) and (b1 < BLUE_RED_HIGH)) {
    dir1 = 0;



  } else if ((g1 < GREEN_HIGH_R) and (g1 > GREEN_LOW_R) and (r1 < BLUE_RED_HIGH) and (b1 < BLUE_RED_HIGH)) {
    dir1 = 1;

  } else if ((g2 < GREEN_HIGH_L) and (g2 > GREEN_LOW_L) and (r2 < BLUE_RED_HIGH) and (b2 < BLUE_RED_HIGH)) {
    dir1 = 2;

  } else {
    dir1 = 3;
  }
  display.display();


  
  motors(0, 0);

  e1 = Enc1();
  while ((Enc1() - e1) > -400) {
    robot.v1_target = -45;
    robot.v2_target = -45;
    motorsCorrectedSpeed();
    delay(5);
  }
  motors(0, 0);
  delay(500);
  CamUARTClear();
  while (!parsingCam()) {}
  robot.camDir = bufferCam[1];
  //if (robot.camDir != 3) dir = robot.camDir;
  robot.camDir = 3;
  e1 = Enc1();
  while ((Enc1() - e1) < 400) {
    robot.v1_target = 45;
    robot.v2_target = 45;
    motorsCorrectedSpeed();
    delay(5);
  }
  motors(0, 0);
  if (dir1 == 0) display.print("BACK");
  else if (dir1 == 1) display.print("RIGHT");
  else if (dir1 = 2) display.print("LEFT");
  else display.print("NO");
  display.display();

  delay(500);
  return dir1;
}
