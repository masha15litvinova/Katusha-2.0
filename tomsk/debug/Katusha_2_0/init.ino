
void resetCamera() {
  digitalWrite(CAMERA_RST, LOW);
  delay(500);
  digitalWrite(CAMERA_RST, HIGH);
}
void initGyro() {

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Init gyro...");
  display.display();
  GyroUARTClear();
  long int time_uart_available = millis();
  while (!GyroUART.available()) {
    if ((millis() - time_uart_available) > 8000) {
      robot.v1_target = 0;
      robot.v2_target = 0;
      resetGyro();
      time_uart_available = millis();
    }
  }
  long int gyro_test_start = millis();
  //int gyroData = 0;
  while ((millis() - gyro_test_start) < 20000) {
    display.clearDisplay();
    //if (GyroUART.available()) {
    // GyroUART.readBytes(bufferGyro, 8);
    if (parsingGyro()) {
      robot.angle_yaw = map(bufferGyro[0], 0, 255, 0, 360);
      robot.angle_pitch = map(bufferGyro[1], 0, 255, -90, 90);

      robot.gyroStarted = bufferGyro[1];
      if (robot.gyroStarted == 1) {
        break;
      }
    }
    /*if (GyroUART.available()) {
       gyroData = GyroUART.read();
    }*/

    /*if (parsingGyro())
    {
      robot.angle_yaw =  map(bufferGyro[0], 0, 255, 0, 360);
      robot.angle_pitch =  map(bufferGyro[1], 0, 255, 0, 360);

    }*/
    display.setCursor(0, 10);
    display.println("Gyro yaw: " + String(robot.angle_yaw));
    //display.println(String(bufferGyro[0])+" "+String(bufferGyro[1])+" "+String(bufferGyro[2])+" "+String(bufferGyro[3])+" "+String(bufferGyro[4])+" "+String(bufferGyro[5])+" "+String(bufferGyro[6])+" "+String(bufferGyro[7]));
    display.setCursor(0, 20);
    display.println("Gyro pitch: " + String(robot.angle_pitch));
    display.setCursor(0, 30);
    display.println(String(robot.gyroStarted));
    display.setCursor(0, 40);
    if ((millis() - gyro_test_start) < 5000) {
      display.println("Wait for 20 seconds");
    } else if ((millis() - gyro_test_start) < 10000) {
      display.println("Wait for 15 seconds");
    } else if ((millis() - gyro_test_start) < 15000) {
      display.println("Wait for 10 seconds");
    } else if ((millis() - gyro_test_start) < 16000) {
      display.println("Wait for 5 seconds");
    } else if ((millis() - gyro_test_start) < 17000) {
      display.println("Wait for 4 seconds");
    } else if ((millis() - gyro_test_start) < 18000) {
      display.println("Wait for 3 seconds");
    } else if ((millis() - gyro_test_start) < 19000) {
      display.println("Wait for 2 seconds");
    } else if ((millis() - gyro_test_start) < 20000) {
      display.println("Wait for 1 second");
    }

    display.display();
  }
  GyroUARTClear();
  for (int i = 0; i < 10; i++) {
    while (1) {
      if (parsingGyro()) {
        robot.angle_yaw = map(bufferGyro[0], 0, 255, 0, 360);
        robot.angle_pitch = map(bufferGyro[1], 0, 255, 0, 360);

        break;
      }
    }
  }
  robot.start_angle_p = robot.angle_pitch;
  robot.start_angle_y = robot.angle_yaw;
  //eeprom_write_gyro();
}

void resetLaserDist() {
  digitalWrite(XSHUT1, LOW);
  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT3, LOW);
  digitalWrite(XSHUT4, LOW);
  delay(10);
  digitalWrite(XSHUT1, HIGH);
  digitalWrite(XSHUT2, HIGH);
  digitalWrite(XSHUT3, HIGH);
  digitalWrite(XSHUT4, HIGH);
  delay(10);

  digitalWrite(XSHUT1, LOW);
  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT3, LOW);
  digitalWrite(XSHUT4, LOW);
}
void SwitchOffOnLaserDist() {
  digitalWrite(SWITCH1, HIGH);
  digitalWrite(SWITCH2, HIGH);
  digitalWrite(SWITCH3, HIGH);
  digitalWrite(SWITCH4, HIGH);
  //delay(100);
  digitalWrite(SWITCH1, LOW);
  digitalWrite(SWITCH2, LOW);
  digitalWrite(SWITCH3, LOW);
  digitalWrite(SWITCH4, LOW);
}
void initPins() {
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  pinMode(XSHUT3, OUTPUT);
  pinMode(XSHUT4, OUTPUT);
  digitalWrite(XSHUT1, LOW);
  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT3, LOW);
  digitalWrite(XSHUT4, LOW);

  pinMode(SWITCH1, OUTPUT);
  pinMode(SWITCH2, OUTPUT);
  pinMode(SWITCH3, OUTPUT);
  pinMode(SWITCH4, OUTPUT);

  digitalWrite(SWITCH1, LOW);
  digitalWrite(SWITCH2, LOW);
  digitalWrite(SWITCH3, LOW);
  digitalWrite(SWITCH4, LOW);
  pinMode(GYRO_RST, OUTPUT);
  pinMode(CAMERA_RST, OUTPUT);
  pinMode(RX3, INPUT_PULLDOWN);
  pinMode(GYRO_INTERRUPT, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(ENC1, INPUT);
  pinMode(ENC2, INPUT);
  pinMode(ENC3, INPUT);
  pinMode(ENC4, INPUT);

  pinMode(GORKA_PIN, INPUT);

  pinMode(ENC_INT1, INPUT);
  pinMode(ENC_INT2, INPUT);
  pinMode(ENC_INT3, INPUT);
  pinMode(ENC_INT4, INPUT);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);

  pinMode(ENDSTOP2, INPUT);
  pinMode(ENDSTOP4, INPUT);

  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);
  pinMode(SENSOR6, INPUT);

  pinMode(CURRENT1, INPUT);
  pinMode(CURRENT2, INPUT);

  pinMode(POTENTIOMETER1, INPUT);
  pinMode(POTENTIOMETER2, INPUT);

  pinMode(MOTOR1_1, OUTPUT);
  pinMode(MOTOR1_2, OUTPUT);
  pinMode(MOTOR2_1, OUTPUT);
  pinMode(MOTOR2_2, OUTPUT);
  pinMode(MOTOR3_1, OUTPUT);
  pinMode(MOTOR3_2, OUTPUT);
  pinMode(MOTOR4_1, OUTPUT);
  pinMode(MOTOR4_2, OUTPUT);

  pinMode(SERVO1, OUTPUT);
  pinMode(SERVO2, OUTPUT);
  pinMode(SERVO3, OUTPUT);

  pinMode(PWM_LIGHTS, OUTPUT);
  analogWriteFrequency(16000);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);



  pinMode(FRONT_DIST, INPUT);
  pinMode(CAM_SIGNAL, OUTPUT);
  digitalWrite(CAM_SIGNAL, LOW);
}



void ledBlinking() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(LED1, HIGH);
    delay(75);
    digitalWrite(LED1, LOW);
    delay(75);
  }
  digitalWrite(LED1, HIGH);
  delay(400);
  digitalWrite(LED1, LOW);
}

void initServos() {
  servo1.attach(SERVO1);
  servo2.attach(SERVO2);
  servo3.attach(SERVO3);

  servo1.write(50);
  /*servo1.write(90);
  delay(1000);
  servo2.write(180);
  servo3.write(0);*/
}
void initColorSensors() {
  display.setTextColor(SH110X_WHITE);
  if (tcs1.begin(0x29, &WIRE2)) {
    display.setCursor(0, 40);
    display.println("Found TCS1 sensor");
    display.display();
    delay(300);
  } else {
    display.setCursor(0, 40);
    display.println("No TCS1 found");
    display.display();
    delay(300);
  }
  if (tcs2.begin(0x29, &WIRE3)) {
    display.setCursor(0, 50);
    display.println("Found TCS2 sensor");
    display.display();
    delay(300);
  } else {
    display.setCursor(0, 50);
    display.println("No TCS2 found");
    display.display();
    delay(300);
  }
}

void initLaserDists() {
  display.setTextColor(SH110X_WHITE);
  digitalWrite(XSHUT1, 1);
  delay(100);
  //sensor_r.setBus(WIRE1);
  sensor_lf.setAddress(sensor_lf_newAddress);
  delay(10);

  digitalWrite(XSHUT2, 1);
  delay(100);
  sensor_f.setAddress(sensor_f_newAddress);
  //sensor_u.setBus(WIRE1);
  delay(10);

  digitalWrite(XSHUT3, 1);
  delay(100);
  sensor_rf.setAddress(sensor_rf_newAddress);
  delay(10);

  digitalWrite(XSHUT4, 1);
  delay(100);
  sensor_r.setAddress(sensor_r_newAddress);
  delay(10);

  sensor_lf.init();
  sensor_f.init();
  sensor_rf.init();
  sensor_r.init();

  delay(2000);

  sensor_lf.setTimeout(500);
  sensor_f.setTimeout(500);
  sensor_rf.setTimeout(500);
  sensor_r.setTimeout(500);

  sensor_lf.startContinuous();
  sensor_f.startContinuous();
  sensor_rf.startContinuous();
  sensor_r.startContinuous();
  delay(400);
}
