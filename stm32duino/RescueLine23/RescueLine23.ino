#include "project_config.h"
#include "pin_config.h"

HardwareSerial GyroUART(RX3, TX3);
HardwareSerial CamUART(RX6, TX6);

TIM_TypeDef *Instance = TIM14;
HardwareTimer *Time = new HardwareTimer(Instance);

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);
Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

TwoWire WIRE1(SDA1, SCL1);
TwoWire WIRE2(SDA2, SCL2);
TwoWire WIRE3(SDA3, SCL3);

VL53L0X sensor1(&WIRE1, XSHUT1);
VL53L0X sensor2(&WIRE1, XSHUT2);
VL53L0X sensor3(&WIRE1, XSHUT3);
VL53L0X sensor4(&WIRE1, XSHUT4);

Servo servo1;
Servo servo2;
Servo servo3;

int black[6] = { 0, 0, 0, 0, 0, 0 };
int white[6] = { 1023, 1023, 1023, 1023, 1023, 1023 };
int grey[6] = { 0, 0, 0, 0, 0, 0 };


int cameraData = 0;
int gyroData = 0;

bool state = true;

uint32_t distance1;
uint32_t distance2;
uint32_t distance3;
uint32_t distance4;


float kp_line[6] = { -0.227, -0.16, -0.14, 0.14, 0.16, 0.227 };
float ki_line[6] = { -0.0012, -0.00074, -0.00072, 0.00072, 0.00074, 0.0012 };
float kd_line[6] = { -1.2, -1.1, -1.0, 1.0, 1.1, 1.2 };
int min_err_i = 25;

int err_line_sens[6] = { 0, 0, 0, 0, 0, 0 };
int err_old_line_sens[6] = { 0, 0, 0, 0, 0, 0 };




class modeSmartButton1 : public SmartButton {
public:
  modeSmartButton1(int p)
    : SmartButton(p) {}
  virtual void onClick();
  virtual void onHold();  // Методы для использования
  virtual void offClick();
};

// Действие на клик: переключаем некий режим меню.
void modeSmartButton1::onHold() {
  switch (state_robot) {
    case (STOP_SCREEN0):
      {

        state_robot = AFTER_STOP_SCREEN;
      }
      case (AFTER_STOP_SCREEN):
      {

        state_robot = last_state_robot;
      }
    default:
      state_robot = STOP_SCREEN0;
      break;
  }
}
void modeSmartButton1::onClick() {
  switch (state_robot) {
    case (STOP_SCREEN0):
      {

        state_robot = AFTER_STOP_SCREEN;
      }
      case (AFTER_STOP_SCREEN):
      {

        state_robot = last_state_robot;
      }
    default:
      state_robot = STOP_SCREEN0;
      break;
  }
}
void modeSmartButton1::offClick() {}
class modeSmartButton2 : public SmartButton {
public:
  modeSmartButton2(int p)
    : SmartButton(p) {}
  virtual void onClick();
  virtual void onHold();  // Методы для использования
  virtual void offClick();
};

// Действие на клик: переключаем некий режим меню.
void modeSmartButton2::onHold() {
  switch (state_robot) {
    case (STOP_SCREEN0):
      state_robot = STOP_SCREEN1;
      break;
    case (STOP_SCREEN1):
      state_robot = STOP_SCREEN2;
      break;
    case (STOP_SCREEN2):
      state_robot = STOP_SCREEN3;
      break;
    case (STOP_SCREEN3):
      state_robot = STOP_SCREEN4;
      break;
    case (STOP_SCREEN4):
      state_robot = STOP_SCREEN5;
      break;
    case (STOP_SCREEN5):
      state_robot = STOP_SCREEN0;
      break;
    default:
      state_robot = STOP_SCREEN0;
      break;
  }
}
void modeSmartButton2::onClick() {
  switch (state_robot) {
    case (STOP_SCREEN0):
      state_robot = STOP_SCREEN1;
      break;
    case (STOP_SCREEN1):
      state_robot = STOP_SCREEN2;
      break;
    case (STOP_SCREEN2):
      state_robot = STOP_SCREEN3;
      break;
    case (STOP_SCREEN3):
      state_robot = STOP_SCREEN4;
      break;
    case (STOP_SCREEN4):
      state_robot = STOP_SCREEN5;
      break;
    case (STOP_SCREEN5):
      state_robot = STOP_SCREEN0;
      break;
    default:
      state_robot = STOP_SCREEN0;
      break;
  }
}
void modeSmartButton2::offClick() {}

modeSmartButton1 btn1(BUTTON1);
modeSmartButton2 btn2(BUTTON2);


Robot robot;

void setup() {


  initPins();
  ledBlinking();

  /*Time->setOverflow(TIMER_FREQ, HERTZ_FORMAT);
  Time->attachInterrupt(time);
  Time->resume();*/
  interrupts();
  //__enable_irq();
  initEncoderIRQs();
  initEndstopIRQs();
  //initButtonsIRQs();




  resetGyro();
  resetCamera();

  initServos();

  WIRE1.begin();
  WIRE2.begin();
  WIRE3.begin();


  GyroUART.setRx(RX3);
  GyroUART.setTx(TX3);
  CamUART.setRx(RX6);
  CamUART.setTx(TX6);

  GyroUART.begin(UART_BAUDRATE);
  CamUART.begin(UART_BAUDRATE);

  display.begin(0, true);
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);



  resetLaserDist();
  initLaserDists();

  initColorSensors();
  initGyro();
  display.clearDisplay();
  display.display();
  ledBlinking();
  analogWrite(PWM_LIGHTS, PWM_LEDS);
  state_robot = COLOR_READ_DATA;
}



int status1_read = 0;
int status2_read = 0;
int status3_read = 0;
int status4_read = 0;




void loop() {
  display.clearDisplay();
  btn1.run();
  btn2.run();








  int time_now = millis();
  //motors(-180, -180);
  if (state) {
    digitalWrite(LED2, HIGH);
  } else {
    digitalWrite(LED2, LOW);
  }

  switch (state_robot) {
    case (CALIBRATION):
      {

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Calibration");
        display.setCursor(0, 10);
        display.println("light sensors");
        display.display();
        calibration();
        display.setCursor(0, 20);
        display.println("Grey:");
        display.setCursor(0, 30);
        display.println(String(grey[0]) + " " + String(grey[1]) + " " + String(grey[2]));
        display.setCursor(0, 40);
        display.println(String(grey[3]) + " " + String(grey[4]) + " " + String(grey[5]));
        display.display();
        delay(3000);
        state_robot = STOP_SCREEN0;
        break;
      }
    case (LINE):
      {

        robot.up_line = 0;
        robot.ud_line = 0;
        motors(robot.motor1, robot.motor2);
        for (int i = 0; i < 6; i++) {
          err_line_sens[i] = robot.sensors[i] - grey[i];
        }
        for (int i = 0; i < 6; i++) {
          robot.up_line = robot.up_line + err_line_sens[i] * kp_line[i];
        }
        for (int i = 0; i < 6; i++) {
          robot.ud_line = robot.ud_line + (err_line_sens[i] - err_old_line_sens[i]) * kd_line[i];
        }
        for (int i = 0; i < 6; i++) {
          if (abs(err_line_sens[i]) >= min_err_i) {
            robot.ui_line = robot.ui_line + err_line_sens[i] * ki_line[i];
          }
        }

        robot.u_line = robot.up_line + robot.ui_line + robot.ud_line;
        robot.v1_target = V_MAIN + robot.u_line;
        robot.v2_target = V_MAIN - robot.u_line;
        for (int i = 0; i < 6; i++) {
          err_old_line_sens[i] = err_line_sens[i];
        }
        //if (millis() - robot.timeDist2 > DIST2_DELAY) state_robot = DIST2_READ_DATA;
        if (millis() - robot.timeLineSens > LINE_SENS_DELAY) state_robot = LINE_READ_DATA;
        if (millis() - robot.timeMotors > MOTORS_DELAY) state_robot = MOTORS_PWM_COMPUTE;
        //if (millis() - robot.timeCamera > CAMERA_DELAY) state_robot = CAMERA_READ_DATA;
        //if (millis() - robot.timeGyro > GYRO_DELAY) state_robot = GYRO_READ_DATA;*/
        last_state_robot = LINE;

        if ((robot.sensors[0] + robot.sensors[1] + robot.sensors[2] + robot.sensors[3] + robot.sensors[4] + robot.sensors[5]) > GREY_THRESHOLD * 6)  //условие перекрестка
        {
          motors(0, 0);
          delay(300);
          state_robot = COLOR_READ_DATA;
        }
        break;
      }
    case (MOTORS_PWM_COMPUTE):
      {

        robot.motor1 = vel1(round(robot.v1_target));
        robot.motor2 = vel2(round(robot.v2_target));
        robot.timeMotors = millis();

        state_robot = last_state_robot;

        break;
      }
    case (LINE_READ_DATA):
      {
        robot.sensors[0] = analogRead(SENSOR1);
        robot.sensors[1] = analogRead(SENSOR2);
        robot.sensors[2] = analogRead(SENSOR3);
        robot.sensors[3] = analogRead(SENSOR4);
        robot.sensors[4] = analogRead(SENSOR5);
        robot.sensors[5] = analogRead(SENSOR6);
        for (int i = 0; i < 6; i++) {

          robot.sensors[i] = map(robot.sensors[i], white[i], black[i], 0, 100);  //приведение к одному диапазону, т.к. разные поверхности по-разному отражают
          //-->где-то ошибка больше, поэтому скорость больше
        }
        robot.timeLineSens = millis();

        state_robot = LINE;

        break;
      }
    case (CAMERA_READ_DATA):

      {
        if (CamUART.available()) {
          cameraData = CamUART.read();
          robot.timeCamera = millis();
        }

        state_robot = last_state_robot;

        break;
      }
    case (GYRO_READ_DATA):

      {
        if (GyroUART.available()) {
          int uart_read = GyroUART.read();
          if (uart_read != GYRO_BYTE_SIGNAL) robot.angle = uart_read;
          robot.timeGyro = millis();
        }

        state_robot = last_state_robot;
        break;
      }
    case (DIST2_READ_DATA):
      {
        int distance2_last = distance2;
        digitalWrite(XSHUT2, HIGH);
        status2_read = sensor2.GetDistance(&distance2);
        if (status2_read != VL53L0X_ERROR_NONE) {
          distance2 = distance2_last;
        }
        robot.timeDist2 = millis();
        state_robot = LINE;
        break;
      }
    case (STOP_SCREEN0):
      {

        robot.motor1 = 0;
        robot.motor2 = 0;
        robot.ui_line = 0;
        motors(0, 0);

        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.println("STOP");
        display.setTextSize(1);
        display.setCursor(0, 20);
        display.println("Press 1: moving");
        display.setCursor(0, 30);
        display.println("Press 2: watch values");

        display.setCursor(0, 50);
        if (last_state_robot == LINE) display.println("Do LINE");
        if (last_state_robot == ROTATING_GREEN) display.println("Do ROTATING_GREEN");
        display.display();
        break;
      }
    case (AFTER_STOP_SCREEN):


      {

        motors(0, 0);
        display.clearDisplay();
        display.setTextSize(2);

        display.setCursor(0, 0);
        display.println("GO...");


        display.display();
        delay(500);
        
        break;
      }

    case (STOP_SCREEN1):
      {

        motors(0, 0);

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Line sensors:");
        display.setCursor(0, 10);
        analogWrite(PWM_LIGHTS, PWM_LEDS);
        robot.sensors[0] = analogRead(SENSOR1);
        robot.sensors[1] = analogRead(SENSOR2);
        robot.sensors[2] = analogRead(SENSOR3);
        robot.sensors[3] = analogRead(SENSOR4);
        robot.sensors[4] = analogRead(SENSOR5);
        robot.sensors[5] = analogRead(SENSOR6);
        display.println(String(robot.sensors[0]) + "    " + String(robot.sensors[1]) + "    " + String(robot.sensors[2]));
        display.setCursor(0, 20);
        display.println(String(robot.sensors[3]) + "    " + String(robot.sensors[4]) + "    " + String(robot.sensors[5]));
        display.setCursor(0, 30);
        display.println("RGB Color sensors:");
        uint16_t r, g, b, c;
        tcs1.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
        //delay(300); // Delay for one old integ. time period (to finish old reading)
        delay(154);  // Delay for one new integ. time period (to allow new reading)
        tcs1.getRawData(&r, &g, &b, &c);
        display.setCursor(0, 40);
        display.print("R: " + String(r) + " " + String(g) + " " + String(b) + " " + String(c));

        tcs2.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
        //delay(300); // Delay for one old integ. time period (to finish old reading)
        //delay(154);  // Delay for one new integ. time period (to allow new reading)
        tcs2.getRawData(&r, &g, &b, &c);
        display.setCursor(0, 50);
        display.print("L: " + String(r) + " " + String(g) + " " + String(b) + " " + String(c));
        display.display();
        break;
      }
    case (STOP_SCREEN2):
      {

        motors(0, 0);
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("UART Camera Data:");
        display.setCursor(0, 10);
        if (CamUART.available()) {
          cameraData = CamUART.read();
        }
        display.println(String(cameraData));
        display.setCursor(0, 30);
        display.println("UART Gyro Data:");
        display.setCursor(0, 40);
        if (GyroUART.available()) {
          int uart_read = GyroUART.read();
          if (uart_read != GYRO_BYTE_SIGNAL) gyroData = map(uart_read, 0, 254, 0, 360);
        }
        display.println(String(gyroData));
        display.display();
        break;
      }
    case (STOP_SCREEN3):
      {

        motors(0, 0);

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Laser dist sensors:");
        digitalWrite(XSHUT1, HIGH);
        status1_read = sensor1.GetDistance(&distance1);
        int status1 = sensor1.StaticInit();

        //digitalWrite(XSHUT1, LOW);

        digitalWrite(XSHUT2, HIGH);

        status2_read = sensor2.GetDistance(&distance2);
        int status2 = sensor2.StaticInit();

        //digitalWrite(XSHUT2, LOW);

        digitalWrite(XSHUT3, HIGH);
        status3_read = sensor3.GetDistance(&distance3);
        int status3 = sensor3.StaticInit();

        //digitalWrite(XSHUT3, LOW);

        digitalWrite(XSHUT4, HIGH);
        status4_read = sensor4.GetDistance(&distance4);
        int status4 = sensor4.StaticInit();
        //digitalWrite(XSHUT4, LOW);

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        if (status1_read == VL53L0X_ERROR_NONE) {
          // Output data.
          char report[64];
          snprintf(report, sizeof(report), "| Sensor1 [mm]: %ld |", distance1);

          display.setCursor(0, 10);
          display.println(report);
          display.display();

        } else {
          display.setCursor(0, 10);
          display.println("Error1: " + String(status1));
          display.display();
        }
        if (status2_read == VL53L0X_ERROR_NONE) {
          // Output data.
          char report[64];
          snprintf(report, sizeof(report), "| Sensor2 [mm]: %ld |", distance2);

          display.setCursor(0, 20);
          display.println(report);
          display.display();

        } else {
          display.setCursor(0, 20);
          display.println("Error2: " + String(status2));
          display.display();
        }
        if (status3_read == VL53L0X_ERROR_NONE) {
          // Output data.
          char report[64];
          snprintf(report, sizeof(report), "| Sensor3 [mm]: %ld |", distance3);

          display.setCursor(0, 30);
          display.println(report);
          display.display();

        } else {
          display.setCursor(0, 30);
          display.println("Error3: " + String(status3));
          display.display();
        }

        if (status4_read == VL53L0X_ERROR_NONE) {
          // Output data.
          char report[64];
          snprintf(report, sizeof(report), "| Sensor4 [mm]: %ld |", distance4);

          display.setCursor(0, 40);
          display.println(report);
          display.display();

        } else {
          display.setCursor(0, 40);
          display.println("Error4: " + String(status4));
          display.display();
        }
        int period = millis() - time_now;
        char report[64];

        snprintf(report, sizeof(report), "| Time : %ld |", period);
        display.setCursor(0, 50);
        display.println(report);
        display.display();
        break;
      }
    case (STOP_SCREEN4):
      {

        motors(0, 0);

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);

        display.println("Current sensors:");
        display.setCursor(0, 10);
        robot.current1 = analogRead(CURRENT1);
        robot.current2 = analogRead(CURRENT2);
        display.println("right: " + String(robot.current1) + " left: " + String(robot.current2));
        display.setCursor(0, 20);

        display.println("Potentiometers:");
        display.setCursor(0, 30);
        robot.potentiometer1 = analogRead(POTENTIOMETER1);
        robot.potentiometer2 = analogRead(POTENTIOMETER2);
        display.println("right: " + String(robot.potentiometer1) + " left: " + String(robot.potentiometer2));
        display.setCursor(0, 40);

        display.println("Ball sensor:");
        display.display();
        break;
      }
    case (STOP_SCREEN5):
      {

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Encoders:");
        display.setCursor(0, 10);
        display.println("Enc1 = " + String(Enc1()));
        display.setCursor(0, 20);
        display.println("Enc2 = " + String(Enc2()));
        display.display();
        break;
      }
    case (ROTATING_GREEN):
      {

        display.clearDisplay();
        display.display();

        digitalWrite(LED1, LOW);



        last_state_robot = ROTATING_GREEN;
        if (dir == 0) {
          turnAngle180(65, 30);
        } else if (dir == 1) {
          turnAngle90Right(65, 30);
        } else if (dir == 2) {
          turnAngle90Left(65, 30);
        }

        //state_robot = last_state_robot;

        break;
      }
    case (COLOR_READ_DATA):
      {
        dir = -1;
        display.clearDisplay();
        dir = direction();

        if (dir != -1) {
          state_robot = ROTATING_GREEN;
        }
        break;
      }
  }
}
