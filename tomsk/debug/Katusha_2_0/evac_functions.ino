#define NONE 0
#define RIGHT 1  //в какой стороне больше места
#define LEFT 2

#define ANGLE_SENSOR_MIN_THRESHOLD 400
int side = NONE;


void align_bottom() {
  bool end_1 = false;
  bool end_2 = false;
  while ((digitalRead(ENDSTOP2) == 0) or (digitalRead(ENDSTOP4) == 0)) {
    if (digitalRead(ENDSTOP2) == 1) end_1 = true;
    if (digitalRead(ENDSTOP4) == 1) end_2 = true;
    robot.v1_target = (!end_1) * (-V_ALIGN);
    robot.v2_target = (!end_2) * (-V_ALIGN);
    motorsCorrectedSpeed();
    delay(5);
  }
  motors(0, 0);
  delay(300);
}
void move_center_zone() {

  robot.dist_right_front = 0;
  robot.dist_right_front_last = 0;
  robot.dist_right = 0;
  robot.dist_right_last = 0;
  robot.dist_front = 0;
  robot.dist_front_last = 0;
  robot.dist_left_front = 0;
  robot.dist_left_front_last = 0;
  long int time_start_dist_read = millis();
  motors(0, 0);
  while ((millis() - time_start_dist_read) < 2000) {
    read_all_dist_meters();
  }
  if (DEBUG) {
    ST_Link.println("right_front: " + String(robot.dist_right_front));
    ST_Link.println("left_front: " + String(robot.dist_left_front));
  }
  if ((robot.dist_right_front > ANGLE_SENSOR_MIN_THRESHOLD) and (robot.dist_left_front > ANGLE_SENSOR_MIN_THRESHOLD)) {

    if (robot.dist_right_front > robot.dist_left_front) side = RIGHT;
    else side = LEFT;
  } else {
    if (robot.dist_right_front > ANGLE_SENSOR_MIN_THRESHOLD) side = RIGHT;
    else side = LEFT;
  }

  if (side == RIGHT) {
    turnAngle(-90, 50, 35);
    move_forward_mm(530, 40);
    turnAngle(90, 50, 35);

  } else if (side == LEFT) {
    turnAngle(90, 50, 35);
    move_forward_mm(530, 40);
    turnAngle(-90, 50, 35);
  }
  align_bottom();
  move_forward_mm(500, 45);
  vyravn();
  if (side == LEFT) {
    turnAngle(-140, 50, 33);
  } else if (side == RIGHT) {
    turnAngle(140, 50, 33);
  }
  motors(0, 0);
  delay(100);
}
void rotate_find_balls() {
  int max_ind = 252;
  int step = 63;
  int ind = 0;
  int dist_data[max_ind]{};
  int dist_data_1[max_ind]{};
  int ball_data[max_ind]{};
  long int time_start_rotate = millis();

  float k_filter = 0.1;


  robot.dist_right_front = 0;
  robot.dist_right_front_last = 0;
  robot.dist_right = 0;
  robot.dist_right_last = 0;
  robot.dist_front = 0;
  robot.dist_front_last = 0;
  robot.dist_left_front = 0;
  robot.dist_left_front_last = 0;
  robot.v1_target = V_ROTATE_BALLS;
  robot.v2_target = -V_ROTATE_BALLS;

  long int time_start_dist_read = millis();
  while ((millis() - time_start_dist_read) < 1000) {
    read_all_dist_meters();
  }
  encoder1 = 0;

  digitalWrite(CAM_SIGNAL, HIGH);
  int delta = -1;
  int i_zone = -1;
  int x_size = 320;
  int y_size = 240;

  while (
    ((encoder1 < 3170)) and (ind < max_ind)) {
    if (CamUART.available()) {
      int x = CamUART.read();
      delta = map(x, 0, 255, -(x_size) / 2, (x_size / 2));
      if ((abs(delta) < 10)and(ind>max_ind/7))and(ind<max_ind*6/7) i_zone = ind;
    }

    robot.dist_right = get_distance(&sensor_r);
    robot.dist_front = get_distance(&sensor_f);
    //if ((robot.dist_front > 65000) or (robot.dist_front < 0)) robot.dist_front = robot.dist_front_last;
    //else robot.dist_front_last = robot.dist_front;
    dist_data[ind] = robot.dist_right;
    ball_data[ind] = robot.dist_front;

    ind++;
    motorsCorrectedSpeed();
  }
  motors(0, 0);
  for (int i = 0; i < max_ind; i++) {
    dist_data_1[i] = dist_data[(i - step + max_ind) % max_ind];
  }


  for (int i = 1; i < max_ind - 1; i++) {
    dist_data_1[i] = (dist_data_1[i - 1] + dist_data_1[i] + dist_data_1[i + 1]) / 3;
  }
  for (int i = 1; i < max_ind - 1; i++) {
    ball_data[i] = (ball_data[i - 1] + ball_data[i] + ball_data[i + 1]) / 3;
  }



  for (int i = 1; i < max_ind - 1; i++) {
    ball_data[i] = 0.5 * (0.5 * (ball_data[i - 1] + ball_data[i + 1]) + ball_data[i]);
  }

  ball_data[max_ind - 1] = ball_data[max_ind - 2];
  ball_data[0] = ball_data[1];
  for (int i = 1; i < max_ind; i++) {
    ball_data[i] = ball_data[i - 1] * k_filter + ball_data[i] * (1 - k_filter);
  }


  if (DEBUG) {
    ST_Link.print("dist_data_1[]= { ");
    for (int i = 0; i < max_ind; i++) {
      ST_Link.print(String(dist_data_1[i]) + ", ");
    }
    ST_Link.println("}");
    ST_Link.print("ball_data[]= { ");
    for (int i = 0; i < max_ind; i++) {
      ST_Link.print(String(ball_data[i]) + ", ");
    }
    ST_Link.println("}");
  }
  motors(0, 0);


  int dist = 0;
  int dist_last = 0;
  int count_exits = 0;
  int exits[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
  int out_of_range = 2900;
  int count_out_of_range = 0;


  for (int i = 0; i < max_ind; i++) {
    if ((ball_data[i] > out_of_range) and (ball_data[(i - 1 + max_ind) % max_ind] < out_of_range)) count_out_of_range = 1;
    else if ((ball_data[i] > out_of_range) and (ball_data[(i - 1 + max_ind) % max_ind] > out_of_range)) count_out_of_range++;
    else if ((ball_data[i] < out_of_range) and (ball_data[(i - 1 + max_ind) % max_ind] > out_of_range)) {
      exits[count_exits] = i - (count_out_of_range) / 2;
      count_exits++;
    } else if ((ball_data[i] < out_of_range) and (ball_data[(i - 1 + max_ind) % max_ind] < out_of_range)) count_out_of_range = 0;
  }
  if (DEBUG) ST_Link.println("exits[] = " + String(exits[0]) + " " + String(exits[1]) + " " + String(exits[2]) + " " + String(exits[3]));

  int exit = -1;
  for (int i = 0; i < 10; i++) {
    if (((exits[i]) > max_ind / 5) and ((exits[i]) < (max_ind * 4) / 5)) {
      exit = exits[i];
    }
  }

  int angle = (exit * 360 / max_ind);
  if (angle > 180) angle = angle - 360;


  int alpha_zone = (i_zone * 360 / max_ind);
  if (alpha_zone > 180) alpha_zone = alpha_zone - 360;

  if (i_zone > 0) {
    turnAngle(-alpha_zone, 50, 33);
    move_object_and_return();
  }

  turnAngle(-angle, 50, 33);




  while ((robot.sensors[0] + robot.sensors[1] + robot.sensors[2] + robot.sensors[3] + robot.sensors[4] + robot.sensors[5]) < 3) {
    display.clearDisplay();
    analogWrite(PWM_LIGHTS, PWM_LEDS);
    int s[6] = { 0, 0, 0, 0, 0, 0 };
    s[0] = analogRead(SENSOR1);
    s[1] = analogRead(SENSOR2);
    s[2] = analogRead(SENSOR3);
    s[3] = analogRead(SENSOR4);
    s[4] = analogRead(SENSOR5);
    s[5] = analogRead(SENSOR6);
    for (int i = 0; i < 6; i++) {
      robot.sensors_analog[i] = map(s[i], white[i], black[i], 0, 100) * 0.01;
    }
    int grey_scaled = 24;

    for (int i = 0; i < 6; i++) {
      if (map(s[i], white[i], black[i], 0, 100) > grey_scaled) {
        robot.sensors[i] = 1;
        //display.fillRect(9 + i * 18, 9, 9, 9, SH110X_WHITE);
      } else {
        robot.sensors[i] = 0;
        // display.drawRect(9 + i * 18, 9, 9, 9, SH110X_WHITE);
      }
    }


    robot.v1_target = V_EVAC;
    robot.v2_target = V_EVAC;
    motorsCorrectedSpeed();

    delay(5);
  }
  motors(0, 0);
  delay(300);
  move_forward(200, V_MAIN);
  robot.dist_front = -1;
  robot.dist_right_front = -1;
  robot.dist_right = -1;
  robot.timeWhite = millis();
  robot.sensors[0] = 0;
  robot.sensors[1] = 0;
  robot.sensors[2] = 0;
  robot.sensors[3] = 0;
  robot.sensors[4] = 0;
  robot.sensors[5] = 0;
}

void align_forward() {
  robot.v1_target = V_EVAC;
  robot.v2_target = V_EVAC;
  bool estp1 = endstop1;
  bool estp3 = endstop3;
  int end_1 = true;
  int end_3 = true;
  robot.mode_endstops = 1;
  while ((!estp1) or (!estp3)) {
    if (endstop1 != estp1) end_1 = false;
    if (endstop3 != estp3) end_3 = false;
    robot.v2_target = (end_1) * (V_ALIGN);
    robot.v1_target = (end_3) * (V_ALIGN);
    motorsCorrectedSpeed();
    delay(5);
  }
  robot.mode_endstops = 0;
  motors(0, 0);

  delay(300);
}
void move_object_and_return() {
  encoder1 = 0;
  move_servos_angle();
  align_forward();
  open_iris();
  delay(2000);
  close_iris();
  int enc_proezd = encoder1;
  move_backward(enc_proezd, V_EVAC);
}