int direction_color() {


  int dir1 = 3;
  motors(0, 0);
  display.clearDisplay();
  display.display();


  vyravn();
  //move_forward(10, 50);
  move_forward(5, 30);
  //vyravn();
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
  tcs1.setIntegrationTime(TCS34725_INTEGRATIONTIME_24MS);
  delay(300);  // Delay for one old integ. time period (to finish old reading)
  // Delay for one new integ. time period (to allow new reading)
  tcs1.getRawData(&r1, &g1, &b1, &c1);

  int dist1 = colorDistance(RED_R, GREEN_R, BLUE_R, r1, g1, b1);


  display.setCursor(0, 20);
  display.print("Right: " + String(r1) + " " + String(g1) + " " + String(b1)+" "+String(dist1));
  tcs2.setIntegrationTime(TCS34725_INTEGRATIONTIME_24MS);
  delay(300);  // Delay for one old integ. time period (to finish old reading)
    // Delay for one new integ. time period (to allow new reading)
  tcs2.getRawData(&r2, &g2, &b2, &c2);

  int dist2 = colorDistance(RED_L, GREEN_L, BLUE_L, r2, g2, b2);

  display.setCursor(0, 30);
  display.print("Left: " + String(r2) + " " + String(g2) + " " + String(b2)+" "+String(dist2));
  display.display();
  display.setTextSize(2);
  display.setCursor(0, 50);
  /*if ((g2 > 160) and (g1 > 160)) {
    dir1 = 3;
  } else if ((g1 < GREEN_HIGH_R) and (g1 > GREEN_LOW_R) and (g2 < GREEN_HIGH_L) and (g2 > GREEN_LOW_L) and (r2 < BLUE_RED_HIGH) and (r1 < BLUE_RED_HIGH)) {
    dir1 = 0;
  } else if ((g1 < GREEN_HIGH_R) and (g1 > GREEN_LOW_R) and (r1 < BLUE_RED_HIGH)) {
    dir1 = 1;
  } else if ((g2 < GREEN_HIGH_L) and (g2 > GREEN_LOW_L) and (r2 < BLUE_RED_HIGH)) {
    dir1 = 2;
  } else {
    dir1 = 3;
  }*/

  if ((dist1 < COLOR_DIST_LOW) and (dist2 < COLOR_DIST_LOW)) {
    dir1 = 0;
  } else if ((dist1 < COLOR_DIST_LOW) and (dist2 > COLOR_DIST_HIGH)) {
    dir1 = 1;
  } else if ((dist2 < COLOR_DIST_LOW) and (dist1 > COLOR_DIST_HIGH)) {
    dir1 = 2;
  } else {
    dir1 = 3;
  }

  /*if ((g1 > 60) and (g2 > 60)) {
    dir1 = 0;
  } else if ((g1 > 60)) {
    dir1 = 1;
  } else if ((g2 > 60)) {
    dir1 = 2;
  }*/

  
  display.display();
  motors(0, 0);
  /*move_backward(400, 60);
  delay(500);
  CamUARTClear();
  while (!parsingCam()) {}
  for (int i = 0; i < 5; i++) {
    while (1) {
      if (parsingCam()) {
        break;
      }
    }
  }
  robot.camDir = bufferCam[1];
  //if (robot.camDir != 3) dir1 = robot.camDir;
  robot.camDir = 3;*/

  if (dir1 == 0) display.print("BACK");
  else if (dir1 == 1) display.print("RIGHT");
  else if (dir1 = 2) display.print("LEFT");
  else display.print("NO");
  display.display();
  move_forward(130, 60);
  vyravn();
  delay(500);
  return dir1;
}

void vyravnLine() {
  motors(0, 0);
  int v_max = 80;
  long int vyravn_start = millis();
  robot.ui_line = 0;
  err_line_sens = 10000;
  while ((abs(err_line_sens) > 0.1) or ((millis() - vyravn_start) < 700)) {


    float ki_line_local = 0.01;  //0.0051;
    float kp_line_local = 10;
    float kd_line_local = 0;  //0.5;
    int v_local = 20;
    int s[6] = { 0, 0, 0, 0, 0, 0 };
    s[0] = analogRead(SENSOR1);
    s[1] = analogRead(SENSOR2);
    s[2] = analogRead(SENSOR3);
    s[3] = analogRead(SENSOR4);
    s[4] = analogRead(SENSOR5);
    s[5] = analogRead(SENSOR6);
    for (int i = 0; i < 6; i++) {
      robot.sensors_analog[i] = (map(s[i], white[i], black[i], 0, 100)) * 0.01;
    }
    int grey_scaled = 24;



    robot.up_line = 0;
    robot.ud_line = 0;
    err_line_sens = 0;
    for (int i = 0; i < 6; i++) {
      // err_line_sens = err_line_sens + robot.sensors[i] * weights[i];

      err_line_sens = err_line_sens + robot.sensors_analog[i] * weights[i] / abs(weights_sum);
    }

    robot.up_line = err_line_sens * kp_line_local;
    robot.ud_line = (err_line_sens - err_old_line_sens) * kd_line_local;
    robot.ui_line = robot.ui_line + err_line_sens * ki_line_local;
    if (abs(err_line_sens) < min_err_i) {
      robot.ui_line = 0;
    }
    robot.u_line = robot.up_line + robot.ud_line + robot.ui_line;
    if (abs(robot.u_line) > v_max) {
      if (robot.u_line > 0) robot.u_line = v_max;
      else if (robot.u_line < 0) robot.u_line = -v_max;
    }
    robot.v1_target = v_local + robot.u_line;
    robot.v2_target = v_local - robot.u_line;
    motorsCorrectedSpeed();
    err_old_line_sens = err_line_sens;
    delay(1);
  }
  motors(0, 0);
  delay(2000);
  motors(0, 0);
}

void vyravn() {
  
  GyroUARTClear();
  for (int i = 0; i < 5; i++) {
    while (1) {
      if (parsingGyro()) {
        robot.angle_yaw = map(bufferGyro[0], 0, 255, 0, 360);
        robot.angle_pitch = map(bufferGyro[1], 0, 255, -90, 90);

        break;
      }
    }
  }
  int yaw_now = conv_angle(robot.angle_yaw, robot.start_angle_y);

  if ((yaw_now >= 0) and (yaw_now <= 45)) turnAngle(-yaw_now, 35, 25);
  else if ((yaw_now >= 0) and (yaw_now <= 90)) turnAngle(90 - yaw_now, 35, 25);
  else if ((yaw_now >= 0) and (yaw_now <= 135)) turnAngle(90 - yaw_now, 35, 25);
  else if ((yaw_now >= 0) and (yaw_now <= 180)) turnAngle(180 - yaw_now, 35, 25);
  else if ((yaw_now < 0) and (yaw_now >= -45)) turnAngle(-yaw_now, 35, 25);
  else if ((yaw_now < 0) and (yaw_now >= -90)) turnAngle(-90 - yaw_now, 35, 25);
  else if ((yaw_now < 0) and (yaw_now >= -135)) turnAngle(-90 - yaw_now, 35, 25);
  else if ((yaw_now < 0) and (yaw_now >= -180)) turnAngle(-180 - yaw_now, 35, 25);
  else turnAngle(360 - yaw_now, 35, 25);
  
}
int colorDistance(int red, int green, int blue, int red_read, int green_read, int blue_read) {
  int dist = sqrt((red - red_read) * (red - red_read) + (green - green_read) * (green - green_read) + (blue - blue_read) * (blue - blue_read));
  return dist;
}
boolean isCross() {
  
  if ((robot.colorDist1 < COLOR_DIST_LOW_FAST) or (robot.colorDist2 < COLOR_DIST_LOW_FAST)) {
    return true;
  }

  return false;
}