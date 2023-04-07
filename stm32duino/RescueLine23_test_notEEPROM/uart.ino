#define ASCII_CONVERT '0'

int parsingCam() { /* :angle/dir/;   */
  static bool parseStart = false;
  static byte counter = 0;
  static String number = "";
  if (CamUART.available()) {
    char in = CamUART.read();

    if (in == ';') {  // завершение пакета
      parseStart = false;

      return counter;
    }
    if (in == ':') {  // начало пакета
      parseStart = true;
      counter = 0;
      return 0;
    }
    if (in == '/') {
      bufferCam[counter] = number.toInt();
      number = "";
      counter++;
      return 0;
    }

    if ((parseStart) and (in != '/')) {
      // - '0' это перевод в число (если отправитель print)
      number = number + (in - ASCII_CONVERT);
    }
  }
  return 0;
}

int parsingGyro() { /* :yaw/pitch/started/;   */
  static bool parseStartg = false;
  static byte counterg = 0;
  static String numberg = "";
  if (GyroUART.available()) {
    char ing;
    ing = GyroUART.read();

    if (ing == ';') {  // завершение пакета
      parseStartg = false;
      return counterg;
    }
    if (ing == ':') {  // начало пакета
      parseStartg = true;
      counterg = 0;
      return 0;
    }
    if (ing == '/') {
      bufferGyro[counterg] = numberg.toInt();
      numberg = "";
      counterg++;
      return 0;
    }

    if ((parseStartg) and (ing != '/')) {
      // - '0' это перевод в число (если отправитель print)
      numberg = numberg + (ing - ASCII_CONVERT);
    }
  }
  return 0;
}

void GyroUpdate() {
  if (GyroUART.available()) {
    GyroUART.readBytes(bufferGyro, 8);

    robot.angle_yaw = map(bufferGyro[0], 0, 255, 0, 360);
    robot.angle_pitch = map(bufferGyro[1], 0, 255, 0, 360);

    robot.angle = module((robot.angle_pitch - robot.start_angle_p), 360);
  }

  /* if (parsingGyro()) {
    robot.angle_yaw = map(bufferGyro[0], 0, 255, 0, 360);
    robot.angle_pitch = map(bufferGyro[1], 0, 255, 0, 360);
  }*/
}
int module(int a, int mode) {
  if (a >= 0) return a % mode;
  else return (mode + a % mode);
  return 0;
}

int conv_angle(int a, int start) {
  int x1 = a - start;
  int x2 = a - start + 360;
  int x3 = a - start - 360;
  int minimum = abs(x1);
  if (abs(x2) < minimum) minimum = abs(x2);
  if (abs(x3) < minimum) minimum = abs(x3);




  if ((abs(x1) == minimum)) return x1;
  if ((abs(x2) == minimum)) return x2;
  if ((abs(x3) == minimum)) return x3;
  return 0;
}

void GyroUARTClear() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("UART clear...");
  display.display();
  StopGyro();


  while (GyroUART.available()) {
  //for (int i = 0; i < 10000; i++) {
    int t = GyroUART.read();

    //STlinkUART.print(t);
    //StopGyro();
  }
  //display.clearDisplay();

display.clearDisplay();
display.display();
  StartGyro();
}

void CamUARTClear() {
  while (CamUART.available() > 0) {
    char t = CamUART.read();
  }
}
void StopGyro() {

  //GyroUART.print('2');
  digitalWrite(LED2, LOW);
}
void StartGyro() {
  digitalWrite(LED2, HIGH);
  //GyroUART.print('3');
}
void resetGyro() {

  //GyroUART.print('1');
}
void ZeroGyro() {
  //GyroUART.print('4');
}