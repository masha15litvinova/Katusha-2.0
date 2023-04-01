bool is_obstacle() {
  motors(0, 0);
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    status2_read = sensor2.GetDistance(&distance2);
    sum = sum + distance2;
  }
  if((sum<5*DIST_THRESHOLD)and(sum>10)) return true;
  else return false;
}