

void resetGyro() {

  digitalWrite(GYRO_RST, LOW);
  delay(500);
  digitalWrite(GYRO_RST, HIGH);
}

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
  long int time_uart_available = millis();
  while (!GyroUART.available()) {
    if ((millis() - time_uart_available) > 8000) {
      v1_target = 0;
      v2_target = 0;
      resetGyro();
      time_uart_available = millis();
    }
  }
  long int gyro_test_start = millis();
  int gyro_data = 0;
  while ((millis() - gyro_test_start) < 20000) {
    display.clearDisplay();
    if (GyroUART.available()) {
      int uart_read = GyroUART.read();
      if (uart_read != GYRO_BYTE_SIGNAL) gyro_data = map(uart_read, 0, 254, 0, 360);
    }
    display.setCursor(0, 10);
    display.println("Gyro data: " + String(gyro_data));
    display.setCursor(0, 20);
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
void initPins() {
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

  pinMode(ENDSTOP1, INPUT);
  pinMode(ENDSTOP2, INPUT);
  pinMode(ENDSTOP3, INPUT);
  pinMode(ENDSTOP4, INPUT);

  pinMode(ENC_INT1, INPUT);
  pinMode(ENC_INT2, INPUT);
  pinMode(ENC_INT3, INPUT);
  pinMode(ENC_INT4, INPUT);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);

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

  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  pinMode(XSHUT3, OUTPUT);
  pinMode(XSHUT4, OUTPUT);

  pinMode(SWITCH1, OUTPUT);
  pinMode(SWITCH2, OUTPUT);
  pinMode(SWITCH3, OUTPUT);
  pinMode(SWITCH4, OUTPUT);

  digitalWrite(SWITCH1, LOW);
  digitalWrite(SWITCH2, LOW);
  digitalWrite(SWITCH3, LOW);
  digitalWrite(SWITCH4, LOW);
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
  servo1.write(90);
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
  delay(400);
}

void initLaserDists() {
  display.setTextColor(SH110X_WHITE);
  digitalWrite(XSHUT1, HIGH);
  sensor1.begin();
  int status1 = sensor1.InitSensor(0xC0);

  digitalWrite(XSHUT2, HIGH);
  sensor2.begin();
  int status2 = sensor2.InitSensor(0x31);

  digitalWrite(XSHUT3, HIGH);
  sensor3.begin();
  int status3 = sensor3.InitSensor(0x10);

  digitalWrite(XSHUT4, HIGH);
  sensor4.begin();
  int status4 = sensor4.InitSensor(0x3C);

  if (status1) {
    display.setCursor(0, 0);
    display.println("Init sensor1 failed...");
    display.display();
    Serial.println("");
  } else {
    display.setCursor(0, 0);
    display.println("Init sensor1 ok...");
    display.display();
    Serial.println("");
  }

  if (status2) {
    display.setCursor(0, 10);
    display.println("Init sensor2 failed...");
    display.display();
    Serial.println("");
  } else {
    display.setCursor(0, 10);
    display.println("Init sensor2 ok...");
    display.display();
    Serial.println("");
  }
  if (status3) {
    display.setCursor(0, 20);
    display.println("Init sensor3 failed...");
    display.display();
    Serial.println("");
  } else {
    display.setCursor(0, 20);
    display.println("Init sensor3 ok...");
    display.display();
    Serial.println("");
  }
  if (status4) {
    display.setCursor(0, 30);
    display.println("Init sensor4 failed...");
    display.display();
    Serial.println("");
  } else {
    display.setCursor(0, 30);
    display.println("Init sensor4 ok...");
    display.display();
    Serial.println("");
  }
  delay(400);
}