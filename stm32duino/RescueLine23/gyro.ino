#define MAX_ERR_ANGLE 4

float u_gyro = 0;
float up_gyro = 0;
float ui_gyro = 0;
float kp_gyro = 2.0;
float ki_gyro = 0.025;

void turnTargetAngle(int targetAngle, int max_v, int min_v) {  //поворот на абсолютный угол (зависит исключительно от того, в каком положении откалиброван гироскоп)
  int startPosition = 10000;
  long int time_uart_available = millis();
  while (!GyroUART.available()) {
    if ((millis() - time_uart_available) > 8000) {
      v1_target = 0;
      v2_target = 0;
      resetGyro();
      time_uart_available = millis();
    }
  }
  digitalWrite(LED1, LOW);
  while (startPosition == 10000) {
    if (GyroUART.available()) {
      int uart_read = GyroUART.read();
      if (uart_read != GYRO_BYTE_SIGNAL) startPosition = map(uart_read, 0, 254, 0, 360);
    }
  }
  int angle = startPosition;
  int err = targetAngle - startPosition;

  while (abs(err) > MAX_ERR_ANGLE) {
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("angle: " + String(angle));
    display.setCursor(0, 10);
    display.println("startPos: " + String(startPosition));
    display.setCursor(0, 20);
    display.println("targetAngle: " + String(targetAngle));
    display.setCursor(0, 30);
    display.println("err: " + String(err));
    display.display();
    if (millis() - timeMotors > MOTORS_DELAY) {
      motor1 = vel1(round(v1_target));
      motor2 = vel2(round(v2_target));
      timeMotors = millis();
    }
    if (GyroUART.available()) {
      int uart_read = GyroUART.read();
      if (uart_read != GYRO_BYTE_SIGNAL) angle = map(uart_read, 0, 254, 0, 360);
    }
    err = angle - targetAngle;
    up_gyro = err * kp_gyro;
    ui_gyro = err * ki_gyro + ui_gyro;
    u_gyro = ui_gyro + up_gyro;
    if (u_gyro > max_v) u_gyro = max_v;
    if (u_gyro < -max_v) u_gyro = -max_v;
    if (abs(u_gyro) < min_v) {
      if (u_gyro > 0) u_gyro = min_v;
      else u_gyro = -min_v;
    }
    v1_target = round(u_gyro);
    v2_target = -round(u_gyro);
    motors(motor1, motor2);
  }
  if (abs(err) < MAX_ERR_ANGLE) {
    ui_gyro = 0;
    motors(0, 0);
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Rotation completed");
    display.setCursor(0, 10);
    display.display();
  }
}
Ы

void turnAngle90Left(int max_v, int min_v) {  //поворот на данный угол относительно текущего


  int startPosition = 10000;
  long int time_uart_available = millis();
  while (!GyroUART.available()) {
    if ((millis() - time_uart_available) > 16000) {
      v1_target = 0;
      v2_target = 0;
      resetGyro();
      time_uart_available = millis();
    }
  }
  digitalWrite(LED1, LOW);
  while (startPosition == 10000) {
    if (GyroUART.available()) {
      btn1.run();
      btn2.run();
      int uart_read = GyroUART.read();
      if (uart_read != GYRO_BYTE_SIGNAL) startPosition = map(uart_read, 0, 254, 0, 360);
    }
  }
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("startPos: " + String(startPosition));


  display.display();

  int angle = startPosition;
  int targetAngle = 0;
  int err = 0;
  if (startPosition < 270) {
    targetAngle = startPosition + 90;
    if (targetAngle == 0) targetAngle = 359;
    err = startPosition - targetAngle;

    while (abs(err) > MAX_ERR_ANGLE) {
      btn1.run();
      btn2.run();

      if (millis() - timeMotors > MOTORS_DELAY) {
        motor1 = vel1(round(v1_target));
        motor2 = vel2(round(v2_target));
        timeMotors = millis();
      }
      if (GyroUART.available()) {
        int uart_read = GyroUART.read();
        if (uart_read != GYRO_BYTE_SIGNAL) angle = map(uart_read, 0, 254, 0, 360);
      }
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("angle: " + String(angle));
      display.setCursor(0, 10);
      display.println("startPos: " + String(startPosition));
      display.setCursor(0, 20);
      display.println("targetAngle: " + String(targetAngle));
      display.setCursor(0, 30);
      display.println("err: " + String(err));

      display.display();
      err = angle - targetAngle;
      up_gyro = err * kp_gyro;
      ui_gyro = err * ki_gyro + ui_gyro;
      u_gyro = ui_gyro + up_gyro;
      if (u_gyro > max_v) u_gyro = max_v;
      if (u_gyro < -max_v) u_gyro = -max_v;
      if (abs(u_gyro) < min_v) {
        if (u_gyro > 0) u_gyro = min_v;
        else u_gyro = -min_v;
      }
      v1_target = round(u_gyro);
      v2_target = -round(u_gyro);
      motors(motor1, motor2);
    }
    if (abs(err) <= MAX_ERR_ANGLE) {
      ui_gyro = 0;
      motors(0, 0);
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Rotation completed");
      display.setCursor(0, 10);
      display.display();
    }
  } else {
    targetAngle = startPosition + 90;
    if (targetAngle == 0) targetAngle = 359;
    err = startPosition - targetAngle;

    while (abs(err) > MAX_ERR_ANGLE) {
      btn1.run();
      btn2.run();

      if (millis() - timeMotors > MOTORS_DELAY) {
        motor1 = vel1(round(v1_target));
        motor2 = vel2(round(v2_target));
        timeMotors = millis();
      }
      if (GyroUART.available()) {
        int uart_read = GyroUART.read();
        if (uart_read != GYRO_BYTE_SIGNAL) {
          angle = map(uart_read, 0, 254, 0, 360);
          if (angle < 100) angle = angle + 360;
        }
      }
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("angle: " + String(angle));
      display.setCursor(0, 10);
      display.println("startPos: " + String(startPosition));
      display.setCursor(0, 20);
      display.println("targetAngle: " + String(targetAngle));
      display.setCursor(0, 30);
      display.println("err: " + String(err));

      display.display();
      err = angle - targetAngle;
      up_gyro = err * kp_gyro;
      ui_gyro = err * ki_gyro + ui_gyro;
      u_gyro = ui_gyro + up_gyro;
      if (u_gyro > max_v) u_gyro = max_v;
      if (u_gyro < -max_v) u_gyro = -max_v;
      if (abs(u_gyro) < min_v) {
        if (u_gyro > 0) u_gyro = min_v;
        else u_gyro = -min_v;
      }
      v1_target = round(u_gyro);
      v2_target = -round(u_gyro);
      motors(motor1, motor2);
    }


    if (abs(err) <= MAX_ERR_ANGLE) {
      ui_gyro = 0;

      motors(0, 0);
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Rotation completed");
      display.setCursor(0, 10);
      display.display();
    }
  }
}


void turnAngle90Right(int max_v, int min_v) {  //поворот на данный угол относительно текущего


  int startPosition = 10000;
  long int time_uart_available = millis();
  while (!GyroUART.available()) {
    if ((millis() - time_uart_available) > 16000) {
      v1_target = 0;
      v2_target = 0;
      resetGyro();
      time_uart_available = millis();
    }
  }
  digitalWrite(LED1, LOW);
  while (startPosition == 10000) {
    if (GyroUART.available()) {
      btn1.run();
      btn2.run();
      int uart_read = GyroUART.read();
      if (uart_read != GYRO_BYTE_SIGNAL) startPosition = map(uart_read, 0, 254, 0, 360);
    }
  }
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("startPos: " + String(startPosition));


  display.display();

  int angle = startPosition;
  int targetAngle = 0;
  int err = 0;
  if (startPosition > 90) {
    targetAngle = startPosition - 90;
    if (targetAngle == 0) targetAngle = 1;
    err = startPosition - targetAngle;

    while (abs(err) > MAX_ERR_ANGLE) {
      btn1.run();
      btn2.run();

      if (millis() - timeMotors > MOTORS_DELAY) {
        motor1 = vel1(round(v1_target));
        motor2 = vel2(round(v2_target));
        timeMotors = millis();
      }
      if (GyroUART.available()) {
        int uart_read = GyroUART.read();
        if (uart_read != GYRO_BYTE_SIGNAL) angle = map(uart_read, 0, 254, 0, 360);
      }
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("angle: " + String(angle));
      display.setCursor(0, 10);
      display.println("startPos: " + String(startPosition));
      display.setCursor(0, 20);
      display.println("targetAngle: " + String(targetAngle));
      display.setCursor(0, 30);
      display.println("err: " + String(err));

      display.display();
      err = angle - targetAngle;
      up_gyro = err * kp_gyro;
      ui_gyro = err * ki_gyro + ui_gyro;
      u_gyro = ui_gyro + up_gyro;
      if (u_gyro > max_v) u_gyro = max_v;
      if (u_gyro < -max_v) u_gyro = -max_v;
      if (abs(u_gyro) < min_v) {
        if (u_gyro > 0) u_gyro = min_v;
        else u_gyro = -min_v;
      }
      v1_target = round(u_gyro);
      v2_target = -round(u_gyro);
      motors(motor1, motor2);
    }
    if (abs(err) <= MAX_ERR_ANGLE) {
      ui_gyro = 0;
      motors(0, 0);
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Rotation completed");
      display.setCursor(0, 10);
      display.display();
    }
  } else {
    targetAngle = startPosition - 90;
    if (targetAngle == 0) targetAngle = 1;
    err = startPosition - targetAngle;

    while (abs(err) > MAX_ERR_ANGLE) {
      btn1.run();
      btn2.run();

      if (millis() - timeMotors > MOTORS_DELAY) {
        motor1 = vel1(round(v1_target));
        motor2 = vel2(round(v2_target));
        timeMotors = millis();
      }
      if (GyroUART.available()) {
        int uart_read = GyroUART.read();
        if (uart_read != GYRO_BYTE_SIGNAL) {
          angle = map(uart_read, 0, 254, 0, 360);
          if (angle > 260) angle = angle - 360;
        }
      }
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("angle: " + String(angle));
      display.setCursor(0, 10);
      display.println("startPos: " + String(startPosition));
      display.setCursor(0, 20);
      display.println("targetAngle: " + String(targetAngle));
      display.setCursor(0, 30);
      display.println("err: " + String(err));

      display.display();
      err = angle - targetAngle;
      up_gyro = err * kp_gyro;
      ui_gyro = err * ki_gyro + ui_gyro;
      u_gyro = ui_gyro + up_gyro;
      if (u_gyro > max_v) u_gyro = max_v;
      if (u_gyro < -max_v) u_gyro = -max_v;
      if (abs(u_gyro) < min_v) {
        if (u_gyro > 0) u_gyro = min_v;
        else u_gyro = -min_v;
      }
      v1_target = round(u_gyro);
      v2_target = -round(u_gyro);
      motors(motor1, motor2);
    }


    if (abs(err) <= MAX_ERR_ANGLE) {
      ui_gyro = 0;

      motors(0, 0);
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Rotation completed");
      display.setCursor(0, 10);
      display.display();
    }
  }
}

void turnAngle180(int max_v, int min_v) {  //поворот на данный угол относительно текущего


  int startPosition = 10000;
  long int time_uart_available = millis();
  while (!GyroUART.available()) {
    if ((millis() - time_uart_available) > 16000) {
      v1_target = 0;
      v2_target = 0;
      resetGyro();
      time_uart_available = millis();
    }
  }
  digitalWrite(LED1, LOW);
  while (startPosition == 10000) {
    if (GyroUART.available()) {
      btn1.run();
      btn2.run();
      int uart_read = GyroUART.read();
      if (uart_read != GYRO_BYTE_SIGNAL) startPosition = map(uart_read, 0, 254, 0, 360);
    }
  }
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("startPos: " + String(startPosition));


  display.display();

  int angle = startPosition;
  int targetAngle = 0;
  int err = 0;
  if (startPosition > 180) {
    targetAngle = startPosition - 180;
    if (targetAngle == 0) targetAngle = 1;
    err = startPosition - targetAngle;

    while (abs(err) > MAX_ERR_ANGLE) {
      btn1.run();
      btn2.run();

      if (millis() - timeMotors > MOTORS_DELAY) {
        motor1 = vel1(round(v1_target));
        motor2 = vel2(round(v2_target));
        timeMotors = millis();
      }
      if (GyroUART.available()) {
        int uart_read = GyroUART.read();
        if (uart_read != GYRO_BYTE_SIGNAL) angle = map(uart_read, 0, 254, 0, 360);
      }
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("angle: " + String(angle));
      display.setCursor(0, 10);
      display.println("startPos: " + String(startPosition));
      display.setCursor(0, 20);
      display.println("targetAngle: " + String(targetAngle));
      display.setCursor(0, 30);
      display.println("err: " + String(err));

      display.display();
      err = angle - targetAngle;
      up_gyro = err * kp_gyro;
      ui_gyro = err * ki_gyro + ui_gyro;
      u_gyro = ui_gyro + up_gyro;
      if (u_gyro > max_v) u_gyro = max_v;
      if (u_gyro < -max_v) u_gyro = -max_v;
      if (abs(u_gyro) < min_v) {
        if (u_gyro > 0) u_gyro = min_v;
        else u_gyro = -min_v;
      }
      v1_target = round(u_gyro);
      v2_target = -round(u_gyro);
      motors(motor1, motor2);
    }
    if (abs(err) <= MAX_ERR_ANGLE) {
      ui_gyro = 0;
      motors(0, 0);
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Rotation completed");
      display.setCursor(0, 10);
      display.display();
    }
  } else {
    targetAngle = startPosition - 180;
    if (targetAngle == 0) targetAngle = 1;
    err = startPosition - targetAngle;

    while (abs(err) > MAX_ERR_ANGLE) {
      btn1.run();
      btn2.run();

      if (millis() - timeMotors > MOTORS_DELAY) {
        motor1 = vel1(round(v1_target));
        motor2 = vel2(round(v2_target));
        timeMotors = millis();
      }
      if (GyroUART.available()) {
        int uart_read = GyroUART.read();
        if (uart_read != GYRO_BYTE_SIGNAL) {
          angle = map(uart_read, 0, 254, 0, 360);
          if (angle > 180) angle = angle - 360;
        }
      }
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("angle: " + String(angle));
      display.setCursor(0, 10);
      display.println("startPos: " + String(startPosition));
      display.setCursor(0, 20);
      display.println("targetAngle: " + String(targetAngle));
      display.setCursor(0, 30);
      display.println("err: " + String(err));

      display.display();
      err = angle - targetAngle;
      up_gyro = err * kp_gyro;
      ui_gyro = err * ki_gyro + ui_gyro;
      u_gyro = ui_gyro + up_gyro;
      if (u_gyro > max_v) u_gyro = max_v;
      if (u_gyro < -max_v) u_gyro = -max_v;
      if (abs(u_gyro) < min_v) {
        if (u_gyro > 0) u_gyro = min_v;
        else u_gyro = -min_v;
      }
      v1_target = round(u_gyro);
      v2_target = -round(u_gyro);
      motors(motor1, motor2);
    }


    if (abs(err) <= MAX_ERR_ANGLE) {
      ui_gyro = 0;

      motors(0, 0);
      display.clearDisplay();

      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Rotation completed");
      display.setCursor(0, 10);
      display.display();
    }
  }
}