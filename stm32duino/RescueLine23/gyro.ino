void turnTargetAngle(int targetAngle, int max_v) {
  int startPosition = 0;
  while (!GyroUART.available()) {}
  while (startPosition == 0) {
    if (GyroUART.available()) {
      int uart_read = GyroUART.read();
      if (uart_read != GYRO_BYTE_SIGNAL) startPosition = uart_read;
    }
  }
  int angle = startPosition;
  int err = targetAngle - startPosition;
  float u = 0;
  float k_gyro = 1.0;
  while (abs(err) > MAX_ERR_ANGLE) {
    if (millis() - timeMotors > MOTORS_DELAY) state_robot = MOTORS_PWM_COMPUTE;
    if (GyroUART.available()) {
      int uart_read = GyroUART.read();
      if (uart_read != GYRO_BYTE_SIGNAL) angle = uart_read;
    }
err = angle - targetAngle;
u = err*k_gyro;
if(u>max_v) u = max_v;
if(u<-max_v) u = -max_v;
v1_target = round(u);
v2_target = -round(u);
motors(motor1, motor2);
  }
  motors(0, 0);
}