int get_distance(VL53L0X* sensor) {
  static int sensor_old_dis = 0;

  if (sensor->timeoutOccurred()){}
  int sensor_dis = sensor->readRangeContinuousMillimeters();

  if (sensor_dis == 0) sensor_dis = -1;
  if (sensor_dis == 8190) sensor_dis = sensor_old_dis;
  else sensor_old_dis = sensor_dis;

  if (sensor_dis == 8191) return -2;
  else return sensor_dis;
}