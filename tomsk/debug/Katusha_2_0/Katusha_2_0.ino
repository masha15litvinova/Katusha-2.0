#include "project_config.h"
#include "pin_config.h"

#define DEBUG false

HardwareSerial GyroUART(RX3, TX3);
HardwareSerial CamUART(RX6, TX6);
HardwareSerial ST_Link(RX2, TX2);

VL53L0X sensor_rf;
VL53L0X sensor_f;
VL53L0X sensor_lf;
VL53L0X sensor_r;

TIM_TypeDef *Instance = TIM14;
HardwareTimer *Time = new HardwareTimer(Instance);


Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);
Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_4X);

//TwoWire WIRE1(SDA1, SCL1);
TwoWire WIRE2(SDA2, SCL2);
TwoWire WIRE3(SDA3, SCL3);



Servo servo1;
Servo servo2;
Servo servo3;

int black[6] = { 0, 0, 0, 0, 0, 0 };
int white[6] = { 1023, 1023, 1023, 1023, 1023, 1023 };
int grey[6] = { 0, 0, 0, 0, 0, 0 };

//int dir = 3;
int cameraData = 0;
int gyroData = 0;
byte bufferCam[15];
byte bufferGyro[30];

int big_err_line_sens = 0.7;
bool gorka = 0;

bool state = true;
bool obj = true;  //false;

uint32_t distance1;
uint32_t distance2 = 1000;
uint32_t distance3;
uint32_t distance4;

uint32_t distance2_last = 1000;

<<<<<<< HEAD
float weights[6] = { -4.2, -3.5, -2.8, 2.8, 3.5, 4.2 };  //{0.5, 0.27, 0.23}; //0.7 0.6 0.5
=======
float weights[6] = { -4.6, -3.3, -2.7, 2.7, 3.3, 4.6 };  //{0.5, 0.27, 0.23}; //0.7 0.6 0.5
>>>>>>> parent of adeb024 (good.)
float weights_sum = 0;


//float kp_line[6] = { -0.7, -0.6, -0.5, 0.5, 0.6, 0.7 };
//float ki_line[6] = { -0.02, -0.01, -0.008, 0.008, 0.01, 0.02}; //{ -0.01, -0.009, -0.007, 0.007, 0.009, 0.01 };
//float kd_line[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};//{ -1.5, -1.3, -1.1, 1.1, 1.3, 1.5 };


<<<<<<< HEAD
float min_err_i = 0.016;
=======
float min_err_i = 0.014;
>>>>>>> parent of adeb024 (good.)

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

  pinMode(SDA1, OUTPUT);
  pinMode(SCL1, OUTPUT);

  pinMode(SDA2, OUTPUT);
  pinMode(SCL2, OUTPUT);

  pinMode(SDA3, OUTPUT);
  pinMode(SCL3, OUTPUT);

  //WIRE1.begin();
  initPins();
  SwitchOffOnLaserDist();
  Wire.begin();
  WIRE2.begin();
  WIRE3.begin();


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



  /*GyroUART.setRx(RX3);
  GyroUART.setTx(TX3);
  CamUART.setRx(RX6);
  CamUART.setTx(TX6);*/

  GyroUART.begin(UART_BAUDRATE);
  CamUART.begin(9600);
  ST_Link.begin(UART_BAUDRATE);

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
  StartGyro();

  initGyro();

  display.clearDisplay();
  display.display();
  ledBlinking();
  analogWrite(PWM_LIGHTS, PWM_LEDS);

  initServos();
  calibration_grab();

  sliders(0, 0);
  if (digitalRead(BUTTON2) == 0) {
    state_robot = CALIBRATION;
  } else {
    state_robot = LINE;
    eeprom_read_line();
  }
  robot.v = V_MAIN;
  CamUARTClear();
  robot.timeWhite = millis();
}



int status1_read = 0;
int status2_read = 0;
int status3_read = 0;
int status4_read = 0;




void loop() {
  display.clearDisplay();
  btn1.run();
  btn2.run();


  //motors(-180, -180);
  /*if (state) {
    digitalWrite(LED2, HIGH);
  } else {
    digitalWrite(LED2, LOW);
  }
*/

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
        eeprom_write_line();
        robot.timeWhite = millis();
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

        robot.up_line = 0;
        robot.ud_line = 0;
        err_line_sens = 0;
        /*for (int i = 0; i < 6; i++) {
          // err_line_sens = err_line_sens + robot.sensors[i] * weights[i];

          err_line_sens = err_line_sens + robot.sensors_analog[i] * weights[i] / abs(weights_sum);
        }*/
        err_line_sens = err_line_sens + robot.sensors_analog[0] * weights[0] / abs(weights_sum);
        err_line_sens = err_line_sens + robot.sensors_analog[1] * weights[1] / abs(weights_sum);
        err_line_sens = err_line_sens + (robot.sensors_analog[2] - 1) * weights[2] / abs(weights_sum);
        err_line_sens = err_line_sens + (robot.sensors_analog[3] - 1) * weights[3] / abs(weights_sum);
        err_line_sens = err_line_sens + robot.sensors_analog[4] * weights[4] / abs(weights_sum);
        err_line_sens = err_line_sens + robot.sensors_analog[5] * weights[5] / abs(weights_sum);

        robot.up_line = err_line_sens * robot.kp_line + err_line_sens * err_line_sens * err_line_sens * robot.kcube_line;
        robot.ud_line = (err_line_sens - err_old_line_sens) * robot.kd_line;
        robot.ui_line = robot.ui_line + err_line_sens * robot.ki_line;

        /*display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 20);
        display.println(robot.up_line);
        display.setCursor(0, 30);
        display.println(robot.ud_line);
        display.setCursor(0, 40);
        display.println(robot.ui_line);
        display.display();*/

        if (abs(err_line_sens) < min_err_i) {
          robot.ui_line = 0;
        }
        int delta_grey = 20;
<<<<<<< HEAD
        int u_max = 125;
=======
        int u_max = 110;
>>>>>>> parent of adeb024 (good.)
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

        robot.dist_front = get_distance(&sensor_f);
        if (robot.dist_front < 0) {
          robot.dist_front = robot.dist_front_last;
        } else {
          robot.dist_front_last = robot.dist_front;
        }

        robot.camLineAngleOld = robot.camLineAngle;


        if ((CamUART.available()) and (!obj)) {
          motors(0, 0);
          state_robot = TAKE_CUBE;
          break;
        }
        if (parsingGyro()) {
          robot.angle_yaw = map(bufferGyro[0], 0, 255, 0, 360);
          int x = map(bufferGyro[1], 0, 255, -360, 360);
          int err = robot.start_angle_p - x;
          robot.angle_pitch = (err - err / 180 * 360);
        }
        //gorka = !digitalRead(GORKA_PIN);
        if (abs(robot.angle_pitch) > 5) {
          robot.v = V_GORKA_UP;
          if (robot.u_line > 10) robot.u_line = 10;
          if (robot.u_line < -10) robot.u_line = -10;
        } else robot.v = V_MAIN;
        if ((robot.sensors[0] + robot.sensors[1] + robot.sensors[2] + robot.sensors[3] + robot.sensors[4] + robot.sensors[5]) > 0) {
          robot.timeWhite = millis();
        }
        if ((millis() - robot.timeWhite) > 4200) {
          state_robot = EVAC_ZONE;

          break;
        }
<<<<<<< HEAD
        if ((robot.sensors[0] + robot.sensors[1] + robot.sensors[2] + robot.sensors[3] + robot.sensors[4] + robot.sensors[5] >= 4) and (abs(robot.angle_pitch) < 6) and ((millis() - robot.timeColors) > COLORS_DELAY))  //условие перекрестка
=======
        if ((robot.sensors[0] + robot.sensors[1] + robot.sensors[2] + robot.sensors[3] + robot.sensors[4] + robot.sensors[5] >= 4) and (!gorka) and ((millis() - robot.timeColors) > COLORS_DELAY))  //условие перекрестка
>>>>>>> parent of adeb024 (good.)
        {
          if (DEBUG) ST_Link.println("check cross");
          motors(0, 0);
          uint16_t r1, g1, b1, c1, r2, g2, b2, c2;
          delay(150);
          tcs1.getRawData(&r1, &g1, &b1, &c1);
          robot.colorDist1 = colorDistance(RED_R, GREEN_R, BLUE_R, r1, g1, b1);
          delay(150);
          tcs2.getRawData(&r2, &g2, &b2, &c2);
          robot.colorDist2 = colorDistance(RED_L, GREEN_L, BLUE_L, r2, g2, b2);
          robot.timeColors = millis();

          if (isCross()) {
            state_robot = COLOR_READ_DATA;
          }

          robot.timeColors = millis();
        }

        if ((robot.dist_front < DIST_THRESHOLD) and (robot.dist_front > 0) and (abs(robot.angle_pitch) < 6)) {
          state_robot = OBSTACLE;
          break;
        }



        robot.v1_target = robot.v + robot.u_line;
        robot.v2_target = robot.v - robot.u_line;
        if (abs(robot.v1_target) > MAX_V) {
          if (robot.v1_target > 0) robot.v1_target = MAX_V;
          else if (robot.v1_target < 0) robot.v1_target = -MAX_V;
        }
        if (abs(robot.v2_target) > MAX_V) {
          if (robot.v2_target > 0) robot.v2_target = MAX_V;
          else if (robot.v2_target < 0) robot.v2_target = -MAX_V;
        }
        motorsCorrectedSpeed();
        err_old_line_sens = err_line_sens;
        break;
      }

    case (STOP_SCREEN0):
      {
        btn1.run();
        btn2.run();
        ZeroGyro();
        ZeroGyro();
        ZeroGyro();
        robot.motor1 = 0;
        robot.motor2 = 0;
        robot.ui1 = 0;
        robot.ui2 = 0;
        robot.ui_line = 0;
        robot.ui_cam = 0;
        distance2 = 1000;
        distance2_last = 1000;
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
        CamUARTClear();
        robot.timeWhite = millis();
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
        analogWrite(PWM_LIGHTS, 0);
        robot.sensors[0] = map(analogRead(SENSOR1), white[0], black[0], 0, 100);
        robot.sensors[1] = map(analogRead(SENSOR2), white[1], black[1], 0, 100);
        robot.sensors[2] = map(analogRead(SENSOR3), white[2], black[2], 0, 100);
        robot.sensors[3] = map(analogRead(SENSOR4), white[3], black[3], 0, 100);
        robot.sensors[4] = map(analogRead(SENSOR5), white[4], black[4], 0, 100);
        robot.sensors[5] = map(analogRead(SENSOR6), white[5], black[5], 0, 100);
        display.println(String(robot.sensors[0]) + "    " + String(robot.sensors[1]) + "    " + String(robot.sensors[2]));
        display.setCursor(0, 20);
        display.println(String(robot.sensors[3]) + "    " + String(robot.sensors[4]) + "    " + String(robot.sensors[5]));
        display.setCursor(0, 30);
        display.println("RGB Color sensors:");
        uint16_t r, g, b, c;
        //float r, g, b, c;

        delay(25);
        tcs1.getRawData(&r, &g, &b, &c);

        display.setCursor(0, 40);
        display.print("R: " + String(r) + " " + String(g) + " " + String(b) + " " + String(colorDistance(RED_R, GREEN_R, BLUE_R, r, g, b)));

        delay(25);  // Delay for one old integ. time period (to finish old reading)

        tcs2.getRawData(&r, &g, &b, &c);
        display.setCursor(0, 50);
        display.print("L: " + String(r) + " " + String(g) + " " + String(b) + " " + String(colorDistance(RED_L, GREEN_L, BLUE_L, r, g, b)));
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

          int x = map(bufferGyro[1], 0, 255, -360, 360);
          int err = robot.start_angle_p - x;
          robot.angle_pitch = (err - (err / 180) * 360);
        }
        display.println("Angle: " + String(robot.camLineAngle));
        display.setCursor(0, 20);
        display.println("Direction: " + String(robot.camDir));
        display.setCursor(0, 30);
        display.println("UART Gyro Data:");
        display.setCursor(0, 40);

        display.println("yaw: " + String(robot.angle_yaw));
        display.setCursor(0, 50);
<<<<<<< HEAD

=======
>>>>>>> parent of adeb024 (good.)
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

        int left_front_S = get_distance(&sensor_lf);
        int front_S = get_distance(&sensor_f);
        int right_front_S = get_distance(&sensor_rf);  //sensor_r.readRangeContinuousMillimeters()
        int right_S = get_distance(&sensor_r);

        display.setCursor(0, 0);
        display.print("left front: " + String(left_front_S));
        display.setCursor(0, 10);
        display.print("front: " + String(front_S));
        display.setCursor(0, 20);
        display.print("right front: " + String(right_front_S));
        display.setCursor(0, 30);
        display.print("right: " + String(right_S));
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
        if (DEBUG) {
          ST_Link.println("turn begin");
        }
        digitalWrite(LED1, LOW);

        // turnAngle(-179, 60, 40);
        if (DEBUG) {
          ST_Link.println("dir = " + String(dir));
        }
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
          case (3):
            {
              motors(0, 0);
              //move_forward(100, V_MAIN);
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
        robot.timeColors = millis();
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
        robot.timeWhite = millis();
        break;
      }
    case (COLOR_READ_DATA):
      {
        if (DEBUG) ST_Link.println("start read color data");

        dir = 3;
        display.clearDisplay();
        dir = direction_color();
        state_robot = ROTATING_GREEN;
        if (DEBUG) ST_Link.println("dir after color read = " + String(dir));
        break;
      }
    case (MOVE_SLIDERS):
      {
        move_servos_180_up();
        sliders_movement(15);
        grab();
        sliders_movement(15);
        move_servos_180_down();
        vyravn();
        state_robot = LINE;
        obj = true;

        break;
      }
    case (OBSTACLE):
      {

        float kp_obst = 0.05;
        float kd_obst = 0.07;
        int ideal_dist = 115;
        int err_obst = 0;
        int err_old_obst = 0;
        int u_obst = 0;
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 20);
        display.println("OBSTACLE");
        display.display();
        motors(0, 0);
        move_backward(100, 45);
        turnAngle(60, 50, 35);
        move_forward(150, 40);
        for (int i = 0; i < 6; i++) robot.sensors[i] = 0;
        long int time_begin_obst = millis();
        while ((millis() - time_begin_obst) < 1000) {
          robot.dist_right_front = get_distance(&sensor_rf);
          if (robot.dist_right_front == -2) {
            digitalWrite(SWITCH3, HIGH);
            delay(10);
            digitalWrite(SWITCH3, LOW);
          }
        }
        while ((robot.sensors[0] + robot.sensors[1] + robot.sensors[2] + robot.sensors[3] + robot.sensors[4] + robot.sensors[5]) < 3) {
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
          }

          robot.dist_right_front = get_distance(&sensor_rf);
          err_obst = robot.dist_right_front - ideal_dist;
          u_obst = err_obst * kp_obst + (err_obst - err_old_obst) * kd_obst;
          robot.v1_target = V_OBST + u_obst;
          robot.v2_target = V_OBST - u_obst;
          motorsCorrectedSpeed();
          display.setCursor(0, 0);
          display.setTextSize(1);
          display.println(u_obst);
          display.display();
          GyroUARTClear();
          //delay(5);
          err_old_obst = err_obst;
        }
        motors(0, 0);
        move_forward(200, 40);
        turnAngle(85, 50, 35);
        state_robot = last_state_robot;
        robot.dist_front = -1;
        robot.dist_right_front = -1;
        robot.dist_right = -1;
        robot.timeWhite = millis();
        robot.sensors[0] = 0;
        robot.sensors[1] = 0;
        robot.sensors[2] = 0;
        robot.sensors[3] = 0;
        robot.sensors[4] = 0;
        robot.sensors[5] = 0;
        break;
      }
    case (TAKE_CUBE):
      {
        display.clearDisplay();
        display.display();
        display.setCursor(0, 0);
        int err_cube = 1000;
        float kp_cube = 0.2;
        float ki = 0.02;
        int ui_cube = 0;
        while (abs(err_cube) > 16) {
          display.clearDisplay();
          if (CamUART.available()) {
            int data = CamUART.read();
            err_cube = map(data, 0, 255, -160, 160);
          }
          display.println(err_cube);
          display.display();
          ui_cube = ui_cube + err_cube * ki;
          int u_cube = err_cube * kp_cube + ui_cube;
          if (u_cube > 34) u_cube = 34;
          else if (u_cube < -34) u_cube = -34;
          robot.v1_target = u_cube;
          robot.v2_target = -(u_cube);
          motorsCorrectedSpeed();
        }
        motors(0, 0);
        delay(500);
        robot.motor1 = 0;
        robot.motor2 = 0;
        robot.ui1 = 0;
        robot.ui2 = 0;
        while (digitalRead(CAM_SIGNAL) == 0) {
          robot.v1_target = 50;
          robot.v2_target = 50;
          motorsCorrectedSpeed();
          delay(5);
        }
        motors(0, 0);
        state_robot = MOVE_SLIDERS;
        break;
      }
    case (EVAC_ZONE):
      {
        /* vyravn();
        turnAngle(-90, 28, 45);
        move_forward(200, 50);*/
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 20);
        display.println("EVAC ZONE");
        display.display();
        delay(300);
        display.setTextSize(1);
        float kp_wall = 0.15;
        float kd_wall = 0.03;
        int ideal_dist = 80;
        int err_wall = 0;
        int err_old_wall = 0;
        int u_wall = 0;
        robot.dist_right_front = 0;
        robot.dist_right_front_last = 0;
        robot.dist_right = 0;
        robot.dist_right_last = 0;
        robot.dist_front = 0;
        robot.dist_front_last = 0;
        robot.dist_left_front = 0;
        robot.dist_left_front_last = 0;
        vyravn();
        move_backward(250, 45);
        vyravn();
        move_center_zone();
        rotate_find_balls();
        state_robot = LINE;
        break;
      }
  }
}
