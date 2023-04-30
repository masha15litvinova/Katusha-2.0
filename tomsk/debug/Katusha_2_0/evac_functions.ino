#define ANGLE_SENSOR_MIN_THRESHOLD 400


#define NONE 0
#define RIGHT 1  //в какой стороне больше места
#define LEFT 2
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
  int side = NONE;
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
  if (robot.dist_right_front > ANGLE_SENSOR_MIN_THRESHOLD) {
    side = RIGHT;
  } else {
    side = LEFT;
  }

  if (side == RIGHT) {
    turnAngle(-90, 50, 35);
    move_forward_mm(450, 40);
    turnAngle(90, 50, 35);

  } else if (side == LEFT) {
    turnAngle(90, 50, 35);
    move_forward_mm(450, 40);
    turnAngle(-90, 50, 35);
  }
  align_bottom();
  move_forward_mm(500, 45);
  vyravn();
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



  encoder1 = 0;
  while (
    ((encoder1 < 3100)) and (ind < max_ind)) {
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
}