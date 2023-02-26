#define MAX_ERR_ANGLE 5

float kp_gyro = 0.5;
float ki_gyro = 0.01;

/*void targetAngle(int targetAngle, int max_v, int min_v) {  //поворот на абсолютный угол (зависит исключительно от того, в каком положении откалиброван гироскоп)

  if (robot.turnCompleted) {
    robot.startPosition = DEFAULT_START_POSITION;

    long int time_uart_available = millis();
    while (!GyroUART.available()) {
      if ((millis() - time_uart_available) > 8000) {
        robot.v1_target = 0;
        robot.v2_target = 0;
        resetGyro();
        time_uart_available = millis();
      }
    }

    while (robot.startPosition == DEFAULT_START_POSITION) {
      if (GyroUART.available()) {
        int uart_read_data = map(GyroUART.read(), 0, BYTE)_SIGNAL_GYRO-1, 0, 360);
        if (uart_read_data != GYRO_BYTE_SIGNAL) robot.startPosition = uart_read_data;
      }
    }

    if (!robot.turnCompleted) {
      robot.angle = robot.startPosition;
      robot.errAngle = robot.targetAngle - robot.startPosition;

      while (abs(robot.errAngle) > MAX_ERR_ANGLE) {
        display.clearDisplay();

        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("angle: " + String(robot.angle));
        display.setCursor(0, 10);
        display.println("startPos: " + String(robot.startPosition));
        display.setCursor(0, 20);
        display.println("robot.targetAngle: " + String(robot.targetAngle));
        display.setCursor(0, 30);
        display.println("robot.errAngle: " + String(robot.errAngle));
        display.display();
        if (millis() - robot.timeMotors > MOTORS_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = MOTORS_PWM_COMPUTE;
          break;
        }
        if (GyroUART.available()) {
          int uart_read = GyroUART.read();
          if (uart_read != GYRO_BYTE_SIGNAL) robot.angle = map(uart_read, 0, BYTE)_SIGNAL_GYRO-1, 0, 360);
        }
        robot.errAngle = robot.angle - robot.targetAngle;
        robot.up_gyro = robot.errAngle * kp_gyro;
        robot.ui_gyro = robot.errAngle * ki_gyro + robot.ui_gyro;
        robot.u_gyro = robot.ui_gyro + robot.up_gyro;
        if (robot.u_gyro > max_v) robot.u_gyro = max_v;
        if (robot.u_gyro < -max_v) robot.u_gyro = -max_v;
        if (abs(robot.u_gyro) < min_v) {
          if (robot.u_gyro > 0) robot.u_gyro = min_v;
          else robot.u_gyro = -min_v;
        }
        robot.v1_target = round(robot.u_gyro);
        robot.v2_target = -round(robot.u_gyro);
        motors(robot.motor1, robot.motor2);
      }
    }
    if (abs(robot.errAngle) < MAX_ERR_ANGLE) {
      robot.ui_gyro = 0;
      robot.up_gyro = 0;
      robot.u_gyro = 0;
      motors(0, 0);
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Rotation completed");
      display.setCursor(0, 10);
      display.display();
      robot.turnCompleted = true;
      robot.startPosition = DEFAULT_START_POSITION;
      state_robot = LINE;
    }
  }
  }*/




void turnAngle90Right(int max_v, int min_v) {  //поворот на данный угол относительно текущего


  if (robot.turnCompleted) {
    robot.startPosition = DEFAULT_START_POSITION;

    long int time_uart_available = millis();
    while (!GyroUART.available()) {
      if ((millis() - time_uart_available) > 8000) {
        robot.v1_target = 0;
        robot.v2_target = 0;
        resetGyro();
        time_uart_available = millis();
      }
    }
    digitalWrite(LED1, HIGH);

    while (robot.startPosition == DEFAULT_START_POSITION) {
      if (GyroUART.available()) {
        int uart_read_data = GyroUART.read();

        robot.startPosition = map(uart_read_data, 0, 255, 0, 360);
      }
    }
    robot.turnCompleted = false;
    robot.angle = robot.startPosition;
    robot.targetAngle = 0;

    robot.ui_gyro = 0;
    robot.errAngle = 100;
  }

  if (robot.startPosition == DEFAULT_START_POSITION) robot.turnCompleted = true;

  if (!robot.turnCompleted) {
    if (robot.startPosition > 90) {
      robot.targetAngle = robot.startPosition - 90;

      robot.errAngle = robot.startPosition - robot.targetAngle;

      if (abs(robot.errAngle) > MAX_ERR_ANGLE) {



        if ((millis() - robot.timeGyro) > GYRO_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = GYRO_READ_DATA;
        }
        display.clearDisplay();

        display.setTextSize(1);

        display.setCursor(0, 0);
        display.println("angle: " + String(robot.angle));
        display.setCursor(0, 10);
        display.println("startPos: " + String(robot.startPosition));
        display.setCursor(0, 20);
        display.println("robot.targetAngle: " + String(robot.targetAngle));
        display.setCursor(0, 30);
        display.println("robot.errAngle: " + String(robot.errAngle));

        display.display();
        robot.angle = robot.angle_uart;
        robot.errAngle = robot.angle - robot.targetAngle;
        robot.up_gyro = robot.errAngle * kp_gyro;
        robot.ui_gyro = robot.errAngle * ki_gyro + robot.ui_gyro;
        robot.u_gyro = robot.ui_gyro + robot.up_gyro;
        if (robot.u_gyro > max_v) robot.u_gyro = max_v;
        if (robot.u_gyro < -max_v) robot.u_gyro = -max_v;
        if (abs(robot.u_gyro) < min_v) {
          if (robot.u_gyro > 0) robot.u_gyro = min_v;
          else if (robot.u_gyro < 0)robot.u_gyro = -min_v;
        }
        robot.v1_target = round(robot.u_gyro);
        robot.v2_target = -round(robot.u_gyro);
        if (millis() - robot.timeMotors > MOTORS_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = MOTORS_PWM_COMPUTE;
        }
        motors(robot.motor1, robot.motor2);
      }

    } else {
      robot.targetAngle = robot.startPosition - 90;

      robot.errAngle = robot.startPosition - robot.targetAngle;

      if (abs(robot.errAngle) > MAX_ERR_ANGLE) {



        if ((millis() - robot.timeGyro) > GYRO_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = GYRO_READ_DATA;
        }
        display.clearDisplay();

        display.setTextSize(1);

        display.setCursor(0, 0);
        display.println("angle: " + String(robot.angle));
        display.setCursor(0, 10);
        display.println("startPos: " + String(robot.startPosition));
        display.setCursor(0, 20);
        display.println("robot.targetAngle: " + String(robot.targetAngle));
        display.setCursor(0, 30);
        display.println("robot.errAngle: " + String(robot.errAngle));

        display.display();
        if (robot.angle_uart > 270) robot.angle = robot.angle_uart - 360;
        else robot.angle = robot.angle_uart;
        robot.errAngle = robot.angle - robot.targetAngle;
        robot.up_gyro = robot.errAngle * kp_gyro;
        robot.ui_gyro = robot.errAngle * ki_gyro + robot.ui_gyro;
        robot.u_gyro = robot.ui_gyro + robot.up_gyro;
        if (robot.u_gyro > max_v) robot.u_gyro = max_v;
        if (robot.u_gyro < -max_v) robot.u_gyro = -max_v;
        if (abs(robot.u_gyro) < min_v) {
          if (robot.u_gyro > 0) robot.u_gyro = min_v;
          else if (robot.u_gyro < 0) robot.u_gyro = -min_v;
        }
        robot.v1_target = round(robot.u_gyro);
        robot.v2_target = -round(robot.u_gyro);
        if (millis() - robot.timeMotors > MOTORS_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = MOTORS_PWM_COMPUTE;
        }
        motors(robot.motor1, robot.motor2);
      }
    }
  }
  if (abs(robot.errAngle) <= MAX_ERR_ANGLE) {

    robot.ui_gyro = 0;
    robot.up_gyro = 0;
    robot.u_gyro = 0;
    robot.errAngle = 100;

    robot.startPosition = DEFAULT_START_POSITION;
    robot.turnCompleted = true;
    state_robot = LINE;
    robot.motor1 = 0;
    robot.motor2 = 0;
    motors(0, 0);

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Rotation 90 right completed");
    display.setCursor(0, 10);
    display.display();
    long int time_begin = millis();
    while ((millis() - time_begin) < 1300) {}
    dir = -1;
    robot.sensors[0] = 0;
    robot.sensors[1] = 0;
    robot.sensors[2] = 0;
    robot.sensors[3] = 0;
    robot.sensors[4] = 0;
    robot.sensors[5] = 0;

  }
}

void turnAngle90Left(int max_v, int min_v) {  //поворот на данный угол относительно текущего


  if (robot.turnCompleted) {
    robot.startPosition = DEFAULT_START_POSITION;

    long int time_uart_available = millis();
    while (!GyroUART.available()) {
      if ((millis() - time_uart_available) > 8000) {
        robot.v1_target = 0;
        robot.v2_target = 0;
        resetGyro();
        time_uart_available = millis();
      }
    }
    digitalWrite(LED1, HIGH);

    while (robot.startPosition == DEFAULT_START_POSITION) {
      if (GyroUART.available()) {
        int uart_read_data = GyroUART.read();

        robot.startPosition = map(uart_read_data, 0, 255, 0, 360);
      }
    }
    robot.turnCompleted = false;
    robot.angle = robot.startPosition;
    robot.targetAngle = 0;

    robot.ui_gyro = 0;
    robot.errAngle = 100;
  }

  if (robot.startPosition == DEFAULT_START_POSITION) robot.turnCompleted = true;

  if (!robot.turnCompleted) {
    if (robot.startPosition < 270) {
      robot.targetAngle = robot.startPosition + 90;

      robot.errAngle = robot.startPosition - robot.targetAngle;

      if (abs(robot.errAngle) > MAX_ERR_ANGLE) {



        if ((millis() - robot.timeGyro) > GYRO_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = GYRO_READ_DATA;
        }
        display.clearDisplay();

        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("angle: " + String(robot.angle_uart));
        display.setCursor(0, 10);
        display.println("startPos: " + String(robot.startPosition));
        display.setCursor(0, 20);
        display.println("robot.targetAngle: " + String(robot.targetAngle));
        display.setCursor(0, 30);
        display.println("robot.errAngle: " + String(robot.errAngle));

        display.display();
        robot.angle = robot.angle_uart;
        robot.errAngle = robot.angle - robot.targetAngle;
        robot.up_gyro = robot.errAngle * kp_gyro;
        robot.ui_gyro = robot.errAngle * ki_gyro + robot.ui_gyro;
        robot.u_gyro = robot.ui_gyro + robot.up_gyro;
        if (robot.u_gyro > max_v) robot.u_gyro = max_v;
        if (robot.u_gyro < -max_v) robot.u_gyro = -max_v;
        if (abs(robot.u_gyro) < min_v) {
          if (robot.u_gyro > 0) robot.u_gyro = min_v;
          else if (robot.u_gyro < 0)robot.u_gyro = -min_v;
        }
        robot.v1_target = round(robot.u_gyro);
        robot.v2_target = -round(robot.u_gyro);
        if (millis() - robot.timeMotors > MOTORS_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = MOTORS_PWM_COMPUTE;
        }
        motors(robot.motor1, robot.motor2);
      }

    } else {
      robot.targetAngle = robot.startPosition + 90;

      robot.errAngle = robot.startPosition - robot.targetAngle;

      if (abs(robot.errAngle) > MAX_ERR_ANGLE) {



        if ((millis() - robot.timeGyro) > GYRO_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = GYRO_READ_DATA;
        }
        display.clearDisplay();

        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("angle: " + String(robot.angle_uart));
        display.setCursor(0, 10);
        display.println("startPos: " + String(robot.startPosition));
        display.setCursor(0, 20);
        display.println("robot.targetAngle: " + String(robot.targetAngle));
        display.setCursor(0, 30);
        display.println("robot.errAngle: " + String(robot.errAngle));

        display.display();
        if (robot.angle_uart < 90) robot.angle = robot.angle_uart + 360;
        else robot.angle = robot.angle_uart;

        robot.errAngle = robot.angle - robot.targetAngle;
        robot.up_gyro = robot.errAngle * kp_gyro;
        robot.ui_gyro = robot.errAngle * ki_gyro + robot.ui_gyro;
        robot.u_gyro = robot.ui_gyro + robot.up_gyro;
        if (robot.u_gyro > max_v) robot.u_gyro = max_v;
        if (robot.u_gyro < -max_v) robot.u_gyro = -max_v;
        if (abs(robot.u_gyro) < min_v) {
          if (robot.u_gyro > 0) robot.u_gyro = min_v;
          else if (robot.u_gyro < 0) robot.u_gyro = -min_v;
        }
        robot.v1_target = round(robot.u_gyro);
        robot.v2_target = -round(robot.u_gyro);
        if (millis() - robot.timeMotors > MOTORS_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = MOTORS_PWM_COMPUTE;
        }
        motors(robot.motor1, robot.motor2);
      }
    }
  }
  if (abs(robot.errAngle) <= MAX_ERR_ANGLE) {

    robot.ui_gyro = 0;
    robot.up_gyro = 0;
    robot.u_gyro = 0;
    robot.errAngle = 100;

    robot.startPosition = DEFAULT_START_POSITION;
    robot.turnCompleted = true;
    state_robot = LINE;
    robot.motor1 = 0;
    robot.motor2 = 0;
    motors(0, 0);

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Rotation 90 left completed");
    display.setCursor(0, 10);
    display.display();
    long int time_begin = millis();
    while ((millis() - time_begin) < 1300) {}
    dir = -1;
    robot.sensors[0] = 0;
    robot.sensors[1] = 0;
    robot.sensors[2] = 0;
    robot.sensors[3] = 0;
    robot.sensors[4] = 0;
    robot.sensors[5] = 0;

  }
}



void turnAngle180(int max_v, int min_v) {  //поворот на данный угол относительно текущего


  if (robot.turnCompleted) {
    robot.startPosition = DEFAULT_START_POSITION;

    long int time_uart_available = millis();
    while (!GyroUART.available()) {
      if ((millis() - time_uart_available) > 8000) {
        robot.v1_target = 0;
        robot.v2_target = 0;
        resetGyro();
        time_uart_available = millis();
      }
    }
    digitalWrite(LED1, HIGH);

    while (robot.startPosition == DEFAULT_START_POSITION) {
      if (GyroUART.available()) {
        int uart_read_data = GyroUART.read();

        robot.startPosition = map(uart_read_data, 0, 255, 0, 360);
      }
    }
    robot.turnCompleted = false;
    robot.angle = robot.startPosition;
    robot.targetAngle = 0;

    robot.ui_gyro = 0;
    robot.errAngle = 100;
  }

  if (robot.startPosition == DEFAULT_START_POSITION) robot.turnCompleted = true;

  if (!robot.turnCompleted) {
    if (robot.startPosition > 180) {
      robot.targetAngle = robot.startPosition - 180;

      robot.errAngle = robot.startPosition - robot.targetAngle;

      if (abs(robot.errAngle) > MAX_ERR_ANGLE) {



        if ((millis() - robot.timeGyro) > GYRO_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = GYRO_READ_DATA;
        }
        display.clearDisplay();

        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("angle: " + String(robot.angle));
        display.setCursor(0, 10);
        display.println("startPos: " + String(robot.startPosition));
        display.setCursor(0, 20);
        display.println("robot.targetAngle: " + String(robot.targetAngle));
        display.setCursor(0, 30);
        display.println("robot.errAngle: " + String(robot.errAngle));

        display.display();
        robot.angle = robot.angle_uart;
        robot.errAngle = robot.angle - robot.targetAngle;
        robot.up_gyro = robot.errAngle * kp_gyro;
        robot.ui_gyro = robot.errAngle * ki_gyro + robot.ui_gyro;
        robot.u_gyro = robot.ui_gyro + robot.up_gyro;
        if (robot.u_gyro > max_v) robot.u_gyro = max_v;
        if (robot.u_gyro < -max_v) robot.u_gyro = -max_v;
        if (abs(robot.u_gyro) < min_v) {
          if (robot.u_gyro > 0) robot.u_gyro = min_v;
          else if (robot.u_gyro < 0) robot.u_gyro = -min_v;
        }
        robot.v1_target = round(robot.u_gyro);
        robot.v2_target = -round(robot.u_gyro);
        if (millis() - robot.timeMotors > MOTORS_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = MOTORS_PWM_COMPUTE;
        }
        motors(robot.motor1, robot.motor2);
      }

    } else {
      robot.targetAngle = robot.startPosition - 180;

      robot.errAngle = robot.startPosition - robot.targetAngle;

      if (abs(robot.errAngle) > MAX_ERR_ANGLE) {



        if ((millis() - robot.timeGyro) > GYRO_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = GYRO_READ_DATA;
        }
        display.clearDisplay();

        display.setTextSize(1);

        display.setCursor(0, 0);
        display.println("angle: " + String(robot.angle));
        display.setCursor(0, 10);
        display.println("startPos: " + String(robot.startPosition));
        display.setCursor(0, 20);
        display.println("robot.targetAngle: " + String(robot.targetAngle));
        display.setCursor(0, 30);
        display.println("robot.errAngle: " + String(robot.errAngle));

        display.display();
        if (robot.angle > 180) robot.angle = robot.angle_uart - 360;
        else robot.angle = robot.angle_uart;
        robot.errAngle = robot.angle - robot.targetAngle;
        robot.up_gyro = robot.errAngle * kp_gyro;
        robot.ui_gyro = robot.errAngle * ki_gyro + robot.ui_gyro;
        robot.u_gyro = robot.ui_gyro + robot.up_gyro;
        if (robot.u_gyro > max_v) robot.u_gyro = max_v;
        if (robot.u_gyro < -max_v) robot.u_gyro = -max_v;
        if (abs(robot.u_gyro) < min_v) {
          if (robot.u_gyro > 0) robot.u_gyro = min_v;
          else if (robot.u_gyro < 0) robot.u_gyro = -min_v;
        }
        robot.v1_target = round(robot.u_gyro);
        robot.v2_target = -round(robot.u_gyro);
        if (millis() - robot.timeMotors > MOTORS_DELAY) {
          last_state_robot = ROTATING_GREEN;
          state_robot = MOTORS_PWM_COMPUTE;
        }
        motors(robot.motor1, robot.motor2);
      }
    }
  }
  if (abs(robot.errAngle) <= MAX_ERR_ANGLE) {

    robot.ui_gyro = 0;
    robot.up_gyro = 0;
    robot.u_gyro = 0;
    robot.errAngle = 100;

    robot.startPosition = DEFAULT_START_POSITION;
    robot.turnCompleted = true;
    state_robot = LINE;
    robot.motor1 = 0;
    robot.motor2 = 0;
    motors(0, 0);

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Rotation 180 completed");
    display.setCursor(0, 10);
    display.display();
    long int time_begin = millis();
    while ((millis() - time_begin) < 1300) {}
    dir = -1;
    robot.sensors[0] = 0;
    robot.sensors[1] = 0;
    robot.sensors[2] = 0;
    robot.sensors[3] = 0;
    robot.sensors[4] = 0;
    robot.sensors[5] = 0;

  }
}
