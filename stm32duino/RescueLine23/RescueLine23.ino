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

long int timeGyro = 0;
long int timeCamera = 0;
long int timeDist2 = 0;  //передний дальномер снизу (опрашивается, когда робот следует по линии)
long int timeMotors = 0;
long int timeLED = 0;
long int timeLineSens = 0;

int cameraData = 0;
int gyroData = 0;

bool state = true;


int v1_target = 0;
int v2_target = 0;

int motor1 = 0;
int motor2 = 0;

float kp_line[6] = { -0.227, -0.16, -0.14, 0.14, 0.16, 0.227 };
float ki_line[6] = { -0.0012, -0.00074, -0.00072, 0.00072, 0.00074, 0.0012};
float kd_line[6] = { -1.2, -1.1, -1.0, 1.0, 1.1, 1.2 };
int min_err_i = 25;

int err_line_sens[6] = { 0, 0, 0, 0, 0, 0 };
int err_old_line_sens[6] = { 0, 0, 0, 0, 0, 0 };
int u_line = 0;

int up_line = 0;
int ui_line = 0;
int ud_line = 0;

uint32_t distance1;
uint32_t distance2;
uint32_t distance3;
uint32_t distance4;

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
      state_robot = AFTER_STOP_SCREEN;
      break;
    case (AFTER_STOP_SCREEN):
      state_robot = LINE;
      break;
    default:
      state_robot = STOP_SCREEN0;
      break;
  }
}
void modeSmartButton1::onClick() {
  switch (state_robot) {
    case (STOP_SCREEN0):
      state_robot = AFTER_STOP_SCREEN;
      break;
    case (AFTER_STOP_SCREEN):
      state_robot = LINE;
      break;
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

  GyroUART.begin(115200);
  CamUART.begin(115200);

  display.begin(0, true);
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);



  resetLaserDist();
  initLaserDists();

  initColorSensors();
  display.clearDisplay();
  display.display();
  ledBlinking();
  analogWrite(PWM_LIGHTS, PWM_LEDS);
  state_robot = ROTATING_GREEN;
}



int status1_read = 0;
int status2_read = 0;
int status3_read = 0;
int status4_read = 0;

int sensors[6] = { 0, 0, 0, 0, 0, 0 };


void loop() {
  display.clearDisplay();
  btn1.run();
  btn2.run();
  //pwm_start(PWM_LIGHTS, 8000, 100, PERCENT_COMPARE_FORMAT);




  int current1 = 0;
  int current2 = 0;

  int potentiometer1 = 0;
  int potentiometer2 = 0;


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
        up_line = 0;
        ud_line = 0;
        motors(motor1, motor2);
        for (int i = 0; i < 6; i++) {
          err_line_sens[i] = sensors[i] - grey[i];
        }
        for (int i = 0; i < 6; i++) {
          up_line = up_line + err_line_sens[i] * kp_line[i];
        }
        for (int i = 0; i < 6; i++) {
          ud_line = ud_line + (err_line_sens[i] - err_old_line_sens[i]) * kd_line[i];
        }
        for (int i = 0; i < 6; i++) {
          if (abs(err_line_sens[i]) >= min_err_i) {
            ui_line = ui_line + err_line_sens[i] * ki_line[i];
          }
        }

        u_line = up_line + ui_line + ud_line;
        v1_target = V_MAIN + u_line;
        v2_target = V_MAIN - u_line;
        for (int i = 0; i < 6; i++) {
          err_old_line_sens[i] = err_line_sens[i];
        }
        //if (millis() - timeDist2 > DIST2_DELAY) state_robot = DIST2_SENS_READ;
        if (millis() - timeLineSens > LINE_SENS_DELAY) state_robot = LINE_SENS_READ;
        if (millis() - timeMotors > MOTORS_DELAY) state_robot = MOTORS_PWM_COMPUTE;
        //if (millis() - timeCamera > CAMERA_DELAY) state_robot = CAMERA_READ_DATA;
        //if (millis() - timeGyro > GYRO_DELAY) state_robot = GYRO_READ_DATA;*/

        break;
      }
    case (MOTORS_PWM_COMPUTE):
      {
        motor1 = vel1(round(v1_target));
        motor2 = vel2(round(v2_target));
        timeMotors = millis();
        state_robot = LINE;

        break;
      }
    case (LINE_SENS_READ):
      {
        sensors[0] = analogRead(SENSOR1);
        sensors[1] = analogRead(SENSOR2);
        sensors[2] = analogRead(SENSOR3);
        sensors[3] = analogRead(SENSOR4);
        sensors[4] = analogRead(SENSOR5);
        sensors[5] = analogRead(SENSOR6);
        timeLineSens = millis();
        state_robot = LINE;

        break;
      }
    case (CAMERA_READ_DATA):

      {
        if (CamUART.available()) {
          cameraData = CamUART.read();
          timeCamera = millis();
        }

        state_robot = LINE;

        break;
      }
    case (GYRO_READ_DATA):

      {
        if (GyroUART.available()) {
          int uart_read = GyroUART.read();
          if(uart_read!=GYRO_BYTE_SIGNAL) gyroData = uart_read;
          timeGyro = millis();
        }
        state_robot = LINE;
        break;
      }
    case (DIST2_SENS_READ):
      {
        int distance2_last = distance2;
        digitalWrite(XSHUT2, HIGH);
        status2_read = sensor2.GetDistance(&distance2);
        if (status2_read != VL53L0X_ERROR_NONE) {
          distance2 = distance2_last;
        }
        timeDist2 = millis();
        state_robot = LINE;
        break;
      }
    case (STOP_SCREEN0):
      {
        motor1 = 0;
        motor2 = 0;
        ui_line = 0;
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
        display.display();
        break;
      }
    case (AFTER_STOP_SCREEN):


      {
        motors(0, 0);
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.println("LINE FOLLOWING...");
        display.display();
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
        sensors[0] = analogRead(SENSOR1);
        sensors[1] = analogRead(SENSOR2);
        sensors[2] = analogRead(SENSOR3);
        sensors[3] = analogRead(SENSOR4);
        sensors[4] = analogRead(SENSOR5);
        sensors[5] = analogRead(SENSOR6);
        display.println(String(sensors[0]) + "    " + String(sensors[1]) + "    " + String(sensors[2]));
        display.setCursor(0, 20);
        display.println(String(sensors[3]) + "    " + String(sensors[4]) + "    " + String(sensors[5]));
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
          if(uart_read!=GYRO_BYTE_SIGNAL) gyroData = uart_read;
          
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
        current1 = analogRead(CURRENT1);
        current2 = analogRead(CURRENT2);
        display.println("right: " + String(current1) + " left: " + String(current2));
        display.setCursor(0, 20);

        display.println("Potentiometers:");
        display.setCursor(0, 30);
        potentiometer1 = analogRead(POTENTIOMETER1);
        potentiometer2 = analogRead(POTENTIOMETER2);
        display.println("right: " + String(potentiometer1) + " left: " + String(potentiometer2));
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
      case(ROTATING_GREEN):
      {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Rotating to target angle...");
        display.display();

      turnTargetAngle(40, 100);
      }
  }


  /*if (state_robot == LINE) {
   
  } else if (state_robot == MOTORS_PWM_COMPUTE) {

    
    motor1 = vel1(round(v1));
     motor2 = vel2(round(v2));
    state_robot = LINE;
  }


  motors(0, 0);*/
  //  motors(motor1, motor2);
  /**/
}
