#include "project_config.h"
#include "pin_config.h"

HardwareSerial GyroUART(RX3, TX3);
HardwareSerial CamUART(RX6, TX6);

TIM_TypeDef *Instance = TIM14;
HardwareTimer *Time = new HardwareTimer(Instance);

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);
Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

TwoWire WIRE1(SDA1, SCL1);
TwoWire WIRE2(SDA2, SCL2);
TwoWire WIRE3(SDA3, SCL3);

VL53L0X sensor1(&WIRE1, XSHUT1);
VL53L0X sensor2(&WIRE1, XSHUT2);
VL53L0X sensor3(&WIRE1, XSHUT3);
VL53L0X sensor4(&WIRE1, XSHUT4);

long int timeGyro = 0;
long int timeCamera = 0;
long int timeDist = 0;
long int timeMotors = 0;
long int timeLED = 0;

int usartData = 0;
int usartData1 = 0;

bool state = true;

int v = 50;
int motor1 = 0;
int motor2 = 0;

int state_robot = LINE;
int last_state_robot = LINE;

void setup() {
  initPins();
  ledBlinking();

  Time->setOverflow(TIMER_FREQ, HERTZ_FORMAT);
  Time->attachInterrupt(time);
  Time->resume();




  //__enable_irq();
  interrupts();
  initEncoderIRQs();

  resetGyro();
  resetCamera();

  WIRE1.begin();
  WIRE2.begin();
  WIRE3.begin();
  resetLaserDist();

  GyroUART.setRx(RX3);
  GyroUART.setTx(TX3);
  CamUART.setRx(RX6);
  CamUART.setTx(TX6);

  GyroUART.begin(115200);
  CamUART.begin(115200);

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

  display.begin(0, true);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  if (status1) {
    display.setCursor(0, 0);
    display.println("Init sensor1 failed...");
    display.display();
    Serial.println("");
  }
  if (status2) {
    display.setCursor(0, 10);
    display.println("Init sensor2 failed...");
    display.display();
    Serial.println("");
  }
  if (status3) {
    display.setCursor(0, 20);
    display.println("Init sensor3 failed...");
    display.display();
    Serial.println("");
  }
  if (status4) {
    display.setCursor(0, 30);
    display.println("Init sensor4 failed...");
    display.display();
    Serial.println("");
  }
  if (tcs1.begin(0x29, &WIRE2)) {
    display.setCursor(0, 40);
    display.println("Found TCS1 sensor");
    display.display();
    delay(1000);
  } else {
    display.setCursor(0, 40);
    display.println("No TCS1 found");
    display.display();
    delay(1000);
  }
  if (tcs2.begin(0x29, &WIRE3)) {
    display.setCursor(0, 50);
    display.println("Found TCS2 sensor");
    display.display();
    delay(1000);
  } else {
    display.setCursor(0, 50);
    display.println("No TCS2 found");
    display.display();
    delay(1000);
  }
  display.clearDisplay();
  display.display();
  ledBlinking();
  analogWrite(PWM_LIGHTS, 0);
}

int status1 = sensor1.StaticInit();
int status2 = sensor2.StaticInit();
int status3 = sensor3.StaticInit();
int status4 = sensor4.StaticInit();





void loop() {
  display.clearDisplay();
  //pwm_start(PWM_LIGHTS, 8000, 100, PERCENT_COMPARE_FORMAT);
  analogWrite(PWM_LIGHTS, 0);
  uint32_t distance1;
  uint32_t distance2;
  uint32_t distance3;
  uint32_t distance4;

  int sensors[6] = { 0, 0, 0, 0, 0, 0 };


  uint16_t h1 = 0;
  uint16_t h2 = 0;


  int time_now = millis();
  //motors(-180, -180);
  if (state) {
    digitalWrite(LED2, HIGH);
  } else {
    digitalWrite(LED2, LOW);
  }

  if (state_robot == LINE) {
    motors(motor1, 0);
  } else if (state_robot == MOTORS_PWM_COMPUTE) {

    
    motor1 = vel1(round(v));
    // motor2 = vel2(round(v));
    state_robot = LINE;
  }
  /*digitalWrite(XSHUT1, HIGH);
  status1 = sensor1.GetDistance(&distance1);
  //digitalWrite(XSHUT1, LOW);

  digitalWrite(XSHUT2, HIGH);
  status2 = sensor2.GetDistance(&distance2);
  //digitalWrite(XSHUT2, LOW);

  digitalWrite(XSHUT3, HIGH);
  status3 = sensor3.GetDistance(&distance3);
  //digitalWrite(XSHUT3, LOW);

  digitalWrite(XSHUT4, HIGH);
  status4 = sensor4.GetDistance(&distance4);
  //digitalWrite(XSHUT4, LOW);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  if (status1 == VL53L0X_ERROR_NONE) {
    // Output data.
    char report[64];
    snprintf(report, sizeof(report), "| Sensor1 [mm]: %ld |", distance1);

    display.setCursor(0, 0);
    display.println(report);
    display.display();

  } else {
    display.setCursor(0, 0);
    display.println("Error1: " + String(status1));
    display.display();
  }
  if (status2 == VL53L0X_ERROR_NONE) {
    // Output data.
    char report[64];
    snprintf(report, sizeof(report), "| Sensor2 [mm]: %ld |", distance2);

    display.setCursor(0, 10);
    display.println(report);
    display.display();

  } else {
    display.setCursor(0, 10);
    display.println("Error2: " + String(status2));
    display.display();
  }
  if (status3 == VL53L0X_ERROR_NONE) {
    // Output data.
    char report[64];
    snprintf(report, sizeof(report), "| Sensor3 [mm]: %ld |", distance3);

    display.setCursor(0, 20);
    display.println(report);
    display.display();

  } else {
    display.setCursor(0, 20);
    display.println("Error3: " + String(status3));
    display.display();
  }

  if (status4 == VL53L0X_ERROR_NONE) {
    // Output data.
    char report[64];
    snprintf(report, sizeof(report), "| Sensor4 [mm]: %ld |", distance4);

    display.setCursor(0, 30);
    display.println(report);
    display.display();

  } else {
    display.setCursor(0, 30);
    display.println("Error4: " + String(status4));
    display.display();
  }
  int period = millis() - time_now;
  char report[64];

  snprintf(report, sizeof(report), "| Time : %ld |", period);
  display.setCursor(0, 40);
  display.println(report);*/


  sensors[0] = analogRead(SENSOR1);
  sensors[1] = analogRead(SENSOR2);
  sensors[2] = analogRead(SENSOR3);
  sensors[3] = analogRead(SENSOR4);
  sensors[4] = analogRead(SENSOR5);
  sensors[5] = analogRead(SENSOR6);

  /*h1 = adc_read_value(POTENTIOMETER1,12);//adc_get_raw(ADC_CHANNEL_8);//adc_read_value(POTENTIOMETER1,16);//get_adc_channel(POTENTIOMETER1,(uint32_t )3);//get_adc_internal_channel(POTENTIOMETER1);//analogRead(POTENTIOMETER1);
  h2 = analogRead(POTENTIOMETER2);*/
  display.setCursor(0, 0);
  display.println("enc1 = " + String(Enc1()));
  display.setCursor(0, 10);
  display.println("u1 = " + String(U1()));
  display.setCursor(0, 20);
  display.println("curr_speed1 = " + String(CurrentSpeed1()));
  display.setCursor(0, 30);
  display.println("state = " + String(state_robot));
  /*display.setCursor(0, 30);
  display.println("enc2 = "+String(Enc2()));
  display.setCursor(0, 40);
  display.println("u2 = "+String(U2()));
  display.setCursor(0, 50);
  display.println("curr_speed2 = "+String(CurrentSpeed2()));*/
  //display.println(String(sensors[0])+"    "+String(sensors[1])+"    "+String(sensors[2]));
  //display.println(String(sensors[3])+"    "+String(sensors[4])+"    "+String(sensors[5]));
  //display.println(String(usartData1));
  display.display();

  //display.println(String(h1) + " " + String(h2));
  /*uint16_t r, g, b, c;
  tcs1.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
  //delay(300); // Delay for one old integ. time period (to finish old reading)
  delay(154); // Delay for one new integ. time period (to allow new reading)
  tcs1.getRawData(&r, &g, &b, &c);
  display.setCursor(0, 0);
  display.print("Integ. time: 153.6ms ");
  display.setCursor(0, 10);
  display.print(String(r)+" "+String(g)+" "+String(b));
  
  display.display();
  delay(100);*/
  /*motors(vel1(90,enc1));
  enc_1 = Encoder1();*/
}

void gyro() {
  __disable_irq();
  if (GyroUART.available()) {

    int u = GyroUART.read();
    if (u != 255) {
      usartData = u;
    }
    digitalWrite(LED2, HIGH);

  } else {
    digitalWrite(LED2, LOW);
  }
  __enable_irq();
}

void camera() {
  __disable_irq();
  if (CamUART.available()) {

    usartData1 = CamUART.read();
  }
  __enable_irq();
}
void time() {
  //gyro();
  //camera();
  timeMotors++;
  timeLED++;
  if (timeMotors == MOTORS_TICK) {
    state_robot = MOTORS_PWM_COMPUTE;
    timeMotors = 0;
  }
  if (timeLED == LED_TICK) {
    //state = !state;
    timeLED = 0;
  }
}

int lineSensor1() {
  int s = analogRead(SENSOR1);
  return s;
}
