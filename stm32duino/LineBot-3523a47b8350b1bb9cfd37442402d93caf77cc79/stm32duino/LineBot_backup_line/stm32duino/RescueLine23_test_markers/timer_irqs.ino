/*void gyro() {
  __disable_irq();
  if (GyroUART.available()) {

    int u = GyroUART.read();
    if (u != 255) {
      usartData = u;
    }
  }
  __enable_irq();
}

void camera() {
  __disable_irq();
  if (CamUART.available()) {

    usartData1 = CamUART.read();
  }
  __enable_irq();
}*/

/*void time() {
  timeCamera++;
  timeGyro++;
  timeMotors++;
  timeLED++;
  timeLineSens++;
  if (timeMotors == MOTORS_TICK) {
    state_robot = MOTORS_PWM_COMPUTE;
    timeMotors = 0;
  }
  if (timeLED == LED_TICK) {
    timeLED = 0;
  }
  if (timeCamera == CAMERA_UART_TICK) {
    state_robot = CAMERA_READ_DATA;
    timeCamera = 0;
  }
  if (timeGyro == GYRO_UART_TICK) {
    state_robot = GYRO_READ_DATA;
    timeGyro = 0;
  }
   if (timeLineSens == LINE_SENS_TICK) {
    state_robot = LINE_SENS_READ;
    timeLineSens = 0;
  }
}*/
