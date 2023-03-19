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

//int dir = 3;
int cameraData = 0;
int gyroData = 0;
byte bufferCam[3];
byte bufferGyro[3];


bool state = true;

uint32_t distance1;
uint32_t distance2 = 1000;
uint32_t distance3;
uint32_t distance4;

float weights[6] = { -4.5, -2.5, -1.5, 1.5, 2.5, 4.5 };  //{0.5, 0.27, 0.23}; //0.7 0.6 0.5
float weights_sum = 0;


//float kp_line[6] = { -0.7, -0.6, -0.5, 0.5, 0.6, 0.7 };
//float ki_line[6] = { -0.02, -0.01, -0.008, 0.008, 0.01, 0.02}; //{ -0.01, -0.009, -0.007, 0.007, 0.009, 0.01 };
//float kd_line[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};//{ -1.5, -1.3, -1.1, 1.1, 1.3, 1.5 };


float min_err_i = 0.01;

float err_line_sens = 0;
float err_old_line_sens = 0;


boolean grab_completed = 0;

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
        break;
      }
    case (AFTER_STOP_SCREEN):
      {
        state_robot = last_state_robot;
        break;
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

        break;
      }
    case (AFTER_STOP_SCREEN):
      {

        state_robot = last_state_robot;
        break;
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
  motors(0, 0);
  ledBlinking();

  /*Time->setOverflow(TIMER_FREQ, HERTZ_FORMAT);
    Time->attachInterrupt(time);
    Time->resume();*/
  interrupts();
  //__enable_irq();
  initEncoderIRQs();
  initEndstopIRQs();
  //initButtonsIRQs();


  weights_sum = abs(weights[0] + weights[1] + weights[2]);



  resetCamera();
  //resetGyro();

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
  StartGyro();
  initGyro();

  display.clearDisplay();
  display.display();
  ledBlinking();
  analogWrite(PWM_LIGHTS, PWM_LEDS);
  //pwm_start(PWM_LIGHTS, 1000, 0, RESOLUTION_8B_COMPARE_FORMAT);
  // initServos();
  //calibration_grab();
  sliders(0, 0);
  state_robot = CALIBRATION;
  robot.v = V_MAIN;
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
        display.println("Calibrate line");

        display.display();
        calibration();
        display.setCursor(0, 10);
        display.println("White and black:");
        display.setCursor(0, 20);
        display.println(String(white[0]) + " " + String(white[1]) + " " + String(white[2]));
        display.setCursor(0, 30);
        display.println(String(white[3]) + " " + String(white[4]) + " " + String(white[5]));
        display.setCursor(0, 40);
        display.println(String(black[0]) + " " + String(black[1]) + " " + String(black[2]));
        display.setCursor(0, 50);
        display.println(String(black[3]) + " " + String(black[4]) + " " + String(black[5]));
        display.display();
        delay(3000);
        state_robot = STOP_SCREEN0;
        break;
      }
    case (LINE):
      {
        last_state_robot = LINE;

        bool analog = true;
        display.clearDisplay();

        analogWrite(PWM_LIGHTS, PWM_LEDS);
        int s[6] = { 0, 0, 0, 0, 0, 0 };
        s[0] = analogRead(SENSOR1);
        s[1] = analogRead(SENSOR2);
        s[2] = analogRead(SENSOR3);
        s[3] = analogRead(SENSOR4);
        s[4] = analogRead(SENSOR5);
        s[5] = analogRead(SENSOR6);
        for (int i = 0; i < 6; i++) {
          robot.sensors_analog[i] = map(s[i], white[i], black[i], 0, 100) * 0.01;
        }
        int grey_scaled = 24;

        for (int i = 0; i < 6; i++) {
          if (map(s[i], white[i], black[i], 0, 100) > grey_scaled) {
            robot.sensors[i] = 1;
            //display.fillRect(9 + i * 18, 9, 9, 9, SH110X_WHITE);
          } else {
            robot.sensors[i] = 0;
            // display.drawRect(9 + i * 18, 9, 9, 9, SH110X_WHITE);
          }
          //display.drawRect(10 + i * 15, 30, 7, 15, SH110X_WHITE);
          if (analog) {
            for (int i = 0; i < 6; i++) {
              int x = map(s[i], white[i], black[i], 0, 100);
              //display.fillRect(10 + i * 15, 30, 7, map(x, 0, 100, 0, 15), SH110X_WHITE);
            }
          }
          //robot.sensors[i] = map(s[i], white[i], black[i], 0, 100)/100.0;  //приведение к одному диапазону, т.к. разные поверхности по-разному отражают
          //-->где-то ошибка больше, поэтому скорость больше
        }

        digitalWrite(LED1, LOW);
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 20);


        display.println(String(robot.camLineAngle));
        display.setCursor(0, 30);
        display.println(robot.angle_pitch);
        display.setCursor(0, 40);
        display.println(robot.camDir);
        display.display();
        robot.up_line = 0;
        robot.ud_line = 0;
        err_line_sens = 0;
        for (int i = 0; i < 6; i++) {
          // err_line_sens = err_line_sens + robot.sensors[i] * weights[i];

          err_line_sens = err_line_sens + robot.sensors_analog[i] * weights[i] / abs(weights_sum);
        }

        robot.up_line = err_line_sens * robot.kp_line + err_line_sens * err_line_sens * err_line_sens * robot.kcube_line;
        robot.ud_line = (err_line_sens - err_old_line_sens) * robot.kd_line;
        robot.ui_line = robot.ui_line + err_line_sens * robot.ki_line;
        if (abs(err_line_sens) < min_err_i) {
          robot.ui_line = 0;
        }
        int delta_grey = 20;
        int u_max = 110;
        //if (err_line_sens != 0)
        //{
        robot.up_cam = robot.camLineAngle * robot.p_cam + robot.camLineDev * robot.p_cam_line + robot.camLineAngle * robot.camLineAngle * robot.camLineAngle * robot.p_cube_cam;
        robot.ud_cam = (robot.camLineAngle - robot.camLineAngleOld) * robot.d_cam;
        robot.ui_cam = robot.ui_cam + robot.camLineAngle * robot.i_cam;
        robot.u_line = (robot.up_cam + robot.ud_cam + robot.ui_cam) + robot.up_line + robot.ud_line + robot.ui_line;  //((robot.up_line + robot.ui_line + robot.ud_line)+robot.up_cam*0.7)*0.7;

        if (abs(robot.camLineAngle) < 6) {
          robot.ui_cam = 0;
        }
        /*if((robot.sensors[0]+robot.sensors[1]+robot.sensors[2]+robot.sensors[3]+robot.sensors[4]+robot.sensors[5])==0)
          {
          robot.u_line = robot.up_cam;
          }*/

        //}

        if (abs(robot.u_line) > u_max) {
          if (robot.u_line > 0) robot.u_line = u_max;
          else robot.u_line = -u_max;
        }
        /*robot.angle_p = robot.angle_uart;
          if ((abs(robot.angle_p - robot.start_angle_p) < 8) or (abs(robot.angle_p - robot.start_angle_p) > 350))robot.v = V_MAIN;
          else if ((((robot.angle_p - robot.start_angle_p) > 12) and ((robot.angle_p - robot.start_angle_p) < 65)) or (((robot.angle_p - robot.start_angle_p) < -310) and ((robot.angle_p - robot.start_angle_p) > -350))) robot.v = V_GORKA_UP;

          else if ((((robot.angle_p - robot.start_angle_p) < -10) and ((robot.angle_p - robot.start_angle_p) > -65)) or (((robot.angle_p - robot.start_angle_p) > 310) and ((robot.angle_p - robot.start_angle_p) < 350))) robot.v = V_GORKA_DOWN;
        */
        robot.v1_target = robot.v + robot.u_line;
        robot.v2_target = robot.v - robot.u_line;
        motorsCorrectedSpeed();
        err_old_line_sens = err_line_sens;
        robot.camLineAngleOld = robot.camLineAngle;


        //motors(robot.motor1, robot.motor2);

        //if (millis() - robot.timeGyro > GYRO_DELAY_LINE) state_robot = GYRO_READ_DATA;
        //if (millis() - robot.timeLineSens > LINE_SENS_DELAY) state_robot = LINE_READ_DATA;
        //if (millis() - robot.timeMotors > MOTORS_DELAY) state_robot = MOTORS_PWM_COMPUTE;
        //if (millis() - robot.timeDist2 > DIST2_DELAY) state_robot = DIST2_READ_DATA;
        //if (millis() - robot.timeCamera > CAMERA_DELAY) state_robot = CAMERA_READ_DATA;
        if (parsingCam()) {
          robot.camLineAngle = map(bufferCam[0], 0, 255, -91, 91);
          robot.camLineDev = map(bufferCam[2], 0, 255, -CAM_X_SIZE / 2, CAM_X_SIZE / 2);
          if (bufferCam[1] != 3) {
            robot.camDir = bufferCam[1];
            robot.turn_detected = true;
            robot.encoder_remember = Enc1();
            robot.time_remember = millis();
          }
        }
        if (parsingGyro()) {
          robot.angle_yaw = map(bufferGyro[0], 0, 255, 0, 360);
          robot.angle_pitch = map(bufferGyro[1], 0, 255, -180, 180);
        }

        if (abs(robot.angle_pitch) < 6) robot.v = V_MAIN;
        else if (robot.angle_pitch > ANGLE_GORKA) robot.v = V_GORKA_UP;
        else if (robot.angle_pitch < -ANGLE_GORKA) robot.v = V_GORKA_DOWN;

        /*if ((robot.sensors[0] + robot.sensors[1] + robot.sensors[2] + robot.sensors[3] + robot.sensors[4] + robot.sensors[5] >= 4) and (robot.turn_detected) and (abs(Enc1() - robot.encoder_remember) < 1000) and (abs(robot.angle_pitch) < 6) and ((millis() - robot.time_remember) < 3500))  //условие перекрестка
        {
          motors(0, 0);
          delay(300);
          state_robot = COLOR_READ_DATA;
        }*/
        /*if ((robot.sensors[0] + robot.sensors[1] + robot.sensors[2] + robot.sensors[3] + robot.sensors[4] + robot.sensors[5]) > 0) {
          robot.online = millis();
        }*/
        /*if ((robot.sensors[0] + robot.sensors[1] + robot.sensors[2] + robot.sensors[3] + robot.sensors[4] + robot.sensors[5]) == 0) {
          move_backward(100, 50);
        }*/
        /*if((millis()-robot.online)>400)
        {
          fail_save();
          robot.online = millis();
        }*/

        break;
      }
    case (MOTORS_PWM_COMPUTE):
      {
        if (abs(robot.v1_target) < min_pwm) {
          if (robot.v1_target > 0) robot.v1_target = min_pwm;
          else if (robot.v1_target < 0) robot.v1_target = -min_pwm;
        }
        if (abs(robot.v2_target) < min_pwm) {
          if (robot.v2_target > 0) robot.v2_target = min_pwm;
          else if (robot.v2_target < 0) robot.v2_target = -min_pwm;
        }
        robot.motor1 = vel1(round(robot.v1_target));
        robot.motor2 = vel2(round(robot.v2_target));
        robot.timeMotors = millis();

        state_robot = last_state_robot;

        break;
      }
    case (LINE_READ_DATA):
      {
        bool analog = true;
        display.clearDisplay();

        analogWrite(PWM_LIGHTS, PWM_LEDS);
        int s[6] = { 0, 0, 0, 0, 0, 0 };
        s[0] = analogRead(SENSOR1);
        s[1] = analogRead(SENSOR2);
        s[2] = analogRead(SENSOR3);
        s[3] = analogRead(SENSOR4);
        s[4] = analogRead(SENSOR5);
        s[5] = analogRead(SENSOR6);
        for (int i = 0; i < 6; i++) {
          robot.sensors_analog[i] = map(s[i], white[i], black[i], 0, 100) * 0.01;
        }
        int grey_scaled = 24;

        for (int i = 0; i < 6; i++) {
          if (map(s[i], white[i], black[i], 0, 100) > grey_scaled) {
            robot.sensors[i] = 1;
            //display.fillRect(9 + i * 18, 9, 9, 9, SH110X_WHITE);
          } else {
            robot.sensors[i] = 0;
            // display.drawRect(9 + i * 18, 9, 9, 9, SH110X_WHITE);
          }
          //display.drawRect(10 + i * 15, 30, 7, 15, SH110X_WHITE);
          if (analog) {
            for (int i = 0; i < 6; i++) {
              int x = map(s[i], white[i], black[i], 0, 100);
              //display.fillRect(10 + i * 15, 30, 7, map(x, 0, 100, 0, 15), SH110X_WHITE);
            }
          }
          //robot.sensors[i] = map(s[i], white[i], black[i], 0, 100)/100.0;  //приведение к одному диапазону, т.к. разные поверхности по-разному отражают
          //-->где-то ошибка больше, поэтому скорость больше
        }

        robot.timeLineSens = millis();

        display.display();
        state_robot = LINE;

        break;
      }
    case (CAMERA_READ_DATA):

      {
        robot.timeCamera = millis();
        state_robot = last_state_robot;
        break;
      }
    case (GYRO_READ_DATA):
      {
        digitalWrite(LED1, HIGH);

        if (GyroUART.available()) {
          int uart_read = GyroUART.read();
          robot.angle_uart = map(uart_read, 0, 255, 0, 360);
        }
        robot.timeGyro = millis();
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
        btn1.run();
        btn2.run();
        robot.motor1 = 0;
        robot.motor2 = 0;
        robot.ui1 = 0;
        robot.ui2 = 0;
        robot.ui_line = 0;
        robot.ui_cam = 0;
        motors(0, 0);
        StopGyro();
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
        btn1.run();
        btn2.run();
        digitalWrite(LED1, HIGH);
        motors(0, 0);
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.println("GO...");
        display.display();
        GyroUARTClear();
        CamUARTClear();
        StartGyro();
        delay(200);
        state_robot = LINE;
        robot.online = millis();
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
        if (parsingCam()) {
          robot.camLineAngle = map(bufferCam[0], 0, 255, -91, 91);
          robot.camDir = bufferCam[1];
        }
        if (parsingGyro()) {
          robot.angle_yaw = map(bufferGyro[0], 0, 255, 0, 360);
          robot.angle_pitch = map(bufferGyro[1], 0, 255, 0, 360);
        }
        display.println("Angle: " + String(robot.camLineAngle));
        display.setCursor(0, 20);
        display.println("Direction: " + String(robot.camDir));
        display.setCursor(0, 30);
        display.println("UART Gyro Data:");
        display.setCursor(0, 40);

        display.println("yaw: " + String(robot.angle_yaw));
        display.setCursor(0, 50);
        display.println("pitch: " + String(robot.angle_pitch));
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
        display.println("R: " + String(robot.current1) + " L: " + String(robot.current2));
        display.setCursor(0, 20);

        display.println("Potentiometers:");
        display.setCursor(0, 30);
        robot.potentiometer1 = analogRead(POTENTIOMETER1);
        robot.potentiometer2 = analogRead(POTENTIOMETER2);
        display.println("R: " + String(robot.potentiometer2) + " L: " + String(robot.potentiometer1));
        display.setCursor(0, 40);

        robot.ball = digitalRead(BALL);
        display.println("Ball sensor: " + String(robot.ball));
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
        display.setCursor(0, 10);
        display.setTextSize(2);
        display.print("TURN...");
        display.display();

        digitalWrite(LED1, LOW);

        // turnAngle(-179, 60, 40);

        switch (dir) {
          case (0):
            {
              turnAngle(-178, 50, 35);
              robot.turn_detected = false;
              break;
            }
          case (1):
            {
              turnAngle(-90, 50, 35);
              robot.turn_detected = false;
              break;
            }
          case (2):
            {
              turnAngle(90, 50, 35);
              robot.turn_detected = false;
              break;
            }
          default:
            {
              motors(0, 0);
              robot.sensors[0] = 0;
              robot.sensors[1] = 0;
              robot.sensors[2] = 0;
              robot.sensors[3] = 0;
              robot.sensors[4] = 0;
              robot.sensors[5] = 0;
              dir = 3;
              break;
            }
        }
        state_robot = LINE;
        robot.sensors[0] = 0;
        robot.sensors[1] = 0;
        robot.sensors[2] = 0;
        robot.sensors[3] = 0;
        robot.sensors[4] = 0;
        robot.sensors[5] = 0;
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.println("GO...");
        display.display();
        robot.time_remember = 0;
        break;
      }
    case (COLOR_READ_DATA):
      {


        dir = 3;
        display.clearDisplay();
        dir = direction_color();
        state_robot = ROTATING_GREEN;
        break;
      }
    case (MOVE_SLIDERS):
      {
        move_servos_180_up();
        sliders_movement(15);
        grab();
        sliders_movement(15);
        long int time_begin = millis();
        while ((millis() - time_begin) < 2000) {
          motors(V_GRAB_BALL, V_GRAB_BALL);
        }
        motors(0, 0);


        open_iris();
        delay(1000);
        time_begin = millis();
        while ((millis() - time_begin) < 1000) {
          motors(-V_GRAB_BALL, -V_GRAB_BALL);
        }
        motors(0, 0);

        move_servos_180_down();
        close_iris();
        delay(1000);

        break;
      }
    case (OBSTACLE):
      {

        motors(0, 0);
        long int e1 = Enc1();
        while ((Enc1() - e1) < 680) {
          motors(V_OBSTACLE, -V_OBSTACLE);
        }
        motors(0, 0);
        e1 = Enc1();
        while ((Enc1() - e1) < 700) {
          motors(V_OBSTACLE, V_OBSTACLE);
        }
        motors(0, 0);
        long int e2 = Enc2();
        while ((Enc2() - e2) < 750) {
          motors(-V_OBSTACLE, V_OBSTACLE);
        }
        motors(0, 0);
        e1 = Enc1();
        while ((Enc1() - e1) < 1600) {
          motors(V_OBSTACLE, V_OBSTACLE);
        }
        motors(0, 0);
        e2 = Enc2();
        while ((Enc2() - e2) < 740) {
          motors(-V_OBSTACLE, V_OBSTACLE);
        }
        motors(0, 0);
        e1 = Enc1();
        while ((Enc1() - e1) < 730) {
          motors(V_OBSTACLE, V_OBSTACLE);
        }
        motors(0, 0);
        e1 = Enc1();
        while ((Enc1() - e1) < 720) {
          motors(V_OBSTACLE, -V_OBSTACLE);
        }
        motors(0, 0);
        e1 = Enc1();
        while ((Enc1() - e1) < 700) {
          motors(V_OBSTACLE, V_OBSTACLE);
        }
        motors(0, 0);
        state_robot = LINE;
        distance2 = 1000;
        robot.ui1 = 0;
        robot.ui2 = 0;
        robot.up1 = 0;
        robot.u1 = 0;
        robot.up2 = 0;
        robot.ud1 = 0;
        robot.ud2 = 0;
        robot.u2 = 0;
        robot.ui_line = 0;
        robot.sensors[0] = 0;
        robot.sensors[1] = 0;
        robot.sensors[2] = 0;
        robot.sensors[3] = 0;
        robot.sensors[4] = 0;
        robot.sensors[5] = 0;
        robot.err1 = 0;
        robot.err1_old = 0;
        robot.err2 = 0;
        robot.err2_old = 0;
        robot.motor1 = 0;
        robot.motor2 = 0;
        robot.v1_target = 0;
        robot.v2_target = 0;
        robot.current_speed1 = 0;
        robot.current_speed2 = 0;
        delay(2000);
        break;
      }
  }
}

void fail_save() {
  move_backward(300, 50);
}
