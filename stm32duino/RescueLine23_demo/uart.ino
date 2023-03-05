#define ASCII_CONVERT '0'

int parsingCam() { /* :angle/dir/;   */
  static bool parseStart = false;
  static byte counter = 0;
  static String number = "";
  if (CamUART.available()) {
    char in = CamUART.read();
    CamUART.println("in = " + String(in));
    if (in == ';') {        // завершение пакета
      parseStart = false;
      
      return counter;
    }
    if (in == ':') {        // начало пакета
      parseStart = true;
      counter = 0;
      return 0;
    }
    if (in == '/')
    {
      bufferCam[counter] = number.toInt();
      number = "";
      counter++;
      return 0;
    }

    if ((parseStart) and (in != '/')) {
      // - '0' это перевод в число (если отправитель print)
      number = number + (in - ASCII_CONVERT);
      Serial.println("number = " + String(number));
    }

  }
  return 0;
}

int parsingGyro() {  /* :yaw/pitch/;   */
  static bool parseStartg = false;
  static byte counterg = 0;
  static String numberg = "";
  if (GyroUART.available()) {
    char in = GyroUART.read();
    GyroUART.println("in = " + String(in));
    if (in == ';') {        // завершение пакета
      parseStartg = false;
      
      return counterg;
    }
    if (in == ':') {        // начало пакета
      parseStartg = true;
      counterg = 0;
      return 0;
    }
    if (in == '/')
    {
      bufferGyro[counterg] = numberg.toInt();
      numberg = "";
      counterg++;
      return 0;
    }

    if ((parseStartg) and (in != '/')) {
      // - '0' это перевод в число (если отправитель print)
      numberg = numberg + (in - ASCII_CONVERT);
      Serial.println("number = " + String(numberg));
    }

  }
  return 0;
}
