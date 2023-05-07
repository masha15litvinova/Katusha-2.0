#include "config.h"


void uart_cam_read()
{
  __disable_irq();
  digitalWrite(LED_1, HIGH);
  if (UART2_CAMERA.available())
  {
    
    usartData = UART2_CAMERA.read();
    cam_data[count] = usartData;
    count++;
    if (count == 4)
    {
      count = 0;

      if (cam_data[0] == byte_signal)
      {
        direction_move  = cam_data[1];
        line_dev = map(cam_data[2], 0, 250, -255, 255);
        angle = map(cam_data[3], 0, 250, -60, 60);
      }
      else if (cam_data[1] == byte_signal)
      {
        direction_move = cam_data[2];
        line_dev  = map(cam_data[3], 0, 250, -255, 255);
        angle = map(cam_data[0], 0, 250, -60, 60);
      }
      else if (cam_data[2] == byte_signal)
      {
        direction_move = cam_data[3];
        line_dev  = map(cam_data[0], 0, 250, -255, 255);
        angle = map(cam_data[1], 0, 250, -60, 60);
      }
      else if (cam_data[3] == byte_signal)
      {
        direction_move = cam_data[0];
        line_dev  = map(cam_data[1], 0, 250, -255, 255);
        angle = map(cam_data[2], 0, 250, -60, 60);
      }

    }


  }
  __enable_irq();
}

void uart_gyro_read()  //angle_yaw  angle  signal
{
  __disable_irq();
  if (UART3_GYRO.available())
  {
    digitalWrite(LED_2, HIGH);
    usartData_gyro = UART3_GYRO.read();
    gyro_data[count_gyro] = usartData_gyro;
    count_gyro++;
    if (count_gyro == 3)
    {
      count_gyro = 0;

      if (gyro_data[0] == byte_signal_gyro)
      {
        angle_yaw_gyro  = map(gyro_data[1], 0, 254, 0, 360);
        angle_gyro = map(gyro_data[2], 0, 254, 0, 360);

      }
      else if (gyro_data[1] == byte_signal_gyro)
      {
        angle_yaw_gyro  = map(gyro_data[2], 0, 254, 0, 360);
        angle_gyro = map(gyro_data[0], 0, 254, 0, 360);

      }
      else if (gyro_data[2] == byte_signal_gyro)
      {
        angle_yaw_gyro  = map(gyro_data[0], 0, 254, 0, 360);
        angle_gyro = map(gyro_data[1], 0, 254, 0, 360);

      }

    }


  }
  __enable_irq();
}

void send_motors(int m1, int m2, int m3, int m4)
{
  m3 = -m3;
  int conv1 = map(m1, -255, 255, 0, 254);
  int conv2 = map(m2, -255, 255, 0, 254);
  int conv3 = map(m3, -255, 255, 0, 254);
  int conv4 = map(m4, -255, 255, 0, 254);

  UART1_DRIVERBOARD.write(conv1);
  delay(1);
  UART1_DRIVERBOARD.write(conv2);
  delay(1);

  UART1_DRIVERBOARD.write(conv3);
  delay(1);

  UART1_DRIVERBOARD.write(conv4);
  delay(1);

  UART1_DRIVERBOARD.write(byte_motor);


}

int line_deviation()
{
  return line_dev;
}
int direction_turn()
{
  return direction_move;
}
int marker_angle()
{
  return angle;
}

int get_angle_yaw_gyro()
{
  return angle_yaw_gyro;
}
int get_angle_gyro()
{
  return angle_gyro;
}
