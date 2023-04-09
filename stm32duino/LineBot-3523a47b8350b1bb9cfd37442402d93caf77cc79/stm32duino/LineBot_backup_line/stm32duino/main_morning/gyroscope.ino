#include "config.h"

void turn(int angle_yaw) //angle_yaw - относительный поворот
{
  int err_yaw = 0;
  int kp = 1;
  int ki = 0.001;
  int up = 0;
  int ui = 0;
  int u = 0;
  int sp = 30;

  int angle_start = get_angle_yaw_gyro();

  if (((angle_start + angle_yaw) > 0) and ((angle_start + angle_yaw) < 360))

  {
    err_yaw = angle_yaw;
    while (abs(get_angle_yaw_gyro() - (angle_start + angle_yaw)) > 5)
    {
      if (angle_yaw > 0)
      {
        send_motors(sp, sp, sp, sp);
        delay(1);
      }
      else
      {
        send_motors(-sp, -sp, -sp, -sp);
        delay(1);
      }
    }
  }
  else if ((angle_start + angle_yaw) < 0)
  {
    while ((angle_start + get_angle_yaw_gyro()) > 0)
    {
      send_motors(sp, sp, sp, sp);
      delay(1);
    }
    while (((angle_start+angle_yaw+360) - get_angle_yaw_gyro()) > 0)
    {
      send_motors(sp, sp, sp, sp);
      delay(1);
    }
    send_motors(0, 0, 0, 0);
    
  }



}
