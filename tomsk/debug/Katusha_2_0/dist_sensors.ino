int get_distance(VL53L0X* sensor) {
  static int sensor_old_dis = 0;

  if (sensor->timeoutOccurred()) {}
  int sensor_dis = sensor->readRangeContinuousMillimeters();

  if (sensor_dis == 0) sensor_dis = -1;
  /* if (sensor_dis > 65000) sensor_dis = sensor_old_dis;
  else sensor_old_dis = sensor_dis;*/

  if (sensor_dis > 65000) return -2;
  else return sensor_dis;
}

void read_all_dist_meters() {
  robot.dist_right_front = get_distance(&sensor_rf);
  robot.dist_right = get_distance(&sensor_r);
  robot.dist_left_front = get_distance(&sensor_lf);
  robot.dist_front = get_distance(&sensor_f);
  
  if ((robot.dist_front > 65000) or (robot.dist_front < 0)) robot.dist_front = robot.dist_front_last;
  else robot.dist_front_last = robot.dist_front;

  if ((robot.dist_right_front > 65000) or (robot.dist_right_front < 0)) robot.dist_right_front = robot.dist_right_front_last;
  else robot.dist_right_front_last = robot.dist_right_front;

  if ((robot.dist_right > 65000) or (robot.dist_right < 0)) robot.dist_right = robot.dist_right_last;
  else robot.dist_right_last = robot.dist_right;

  if ((robot.dist_left_front > 65000) or (robot.dist_left_front < 0)) robot.dist_left_front = robot.dist_left_front_last;
  else robot.dist_left_front_last = robot.dist_left_front;
}