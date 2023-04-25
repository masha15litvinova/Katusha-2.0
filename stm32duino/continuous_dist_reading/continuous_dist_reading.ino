#include <Wire.h>
#include <VL53L0X.h>

//TwoWire WIRE1(PB7, PB6);  //SDA=PB11 & SCL=PB10

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

#define LONG_RANGE

// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
//#define HIGH_ACCURACY

#define sensor_r_newAddress 0xC0
#define sensor_u_newAddress 0x31
#define sensor_l_newAddress 0x10
#define sensor_b_newAddress 0x3C

#define XSHUT_pin1 PG_10
#define XSHUT_pin2 PG_11
#define XSHUT_pin3 PG_12
#define XSHUT_pin4 PG_13

VL53L0X sensor_r;
VL53L0X sensor_u;
VL53L0X sensor_l;
VL53L0X sensor_b;


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define OLED_MOSI PC12
#define OLED_CLK PC10
#define OLED_DC PC13
#define OLED_CS PA11
#define OLED_RST PA12

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

void setup() {
  display.begin(0, true);
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);

  init_dis();

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor_r.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  // sensor_r.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 16);
  // sensor_r.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 12);

  sensor_u.setSignalRateLimit(0.1);
  // //sensor_u.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  // //sensor_u.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  sensor_l.setSignalRateLimit(0.1);
  // //sensor_l.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  // //sensor_l.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  sensor_b.setSignalRateLimit(0.1);
#endif

  // #if defined HIGH_SPEED
  //   // reduce timing budget to 20 ms (default is about 33 ms)
  //   sensor.setMeasurementTimingBudget(20000);
  // #elif defined HIGH_ACCURACY
  //   // increase timing budget to 200 ms
  //   sensor.setMeasurementTimingBudget(200000);
  // #endif
}

void loop() {
  debug_dis();
}




void init_dis() {
  pinMode(PA_9, OUTPUT);
  pinMode(PA_10, OUTPUT);
  pinMode(PE_3, OUTPUT);
  pinMode(PE_2, OUTPUT);

  digitalWrite(PA_9, 0);
  digitalWrite(PA_10, 0);
  digitalWrite(PE_3, 0);
  digitalWrite(PE_2, 0);

  pinMode(XSHUT_pin1, OUTPUT);
  pinMode(XSHUT_pin2, OUTPUT);
  pinMode(XSHUT_pin3, OUTPUT);
  pinMode(XSHUT_pin4, OUTPUT);

  digitalWrite(XSHUT_pin1, 0);
  digitalWrite(XSHUT_pin2, 0);
  digitalWrite(XSHUT_pin3, 0);
  digitalWrite(XSHUT_pin4, 0);

  Wire.begin();
  delay(500);

  digitalWrite(XSHUT_pin1, 1);
  delay(100);
  //sensor_r.setBus(WIRE1);
  sensor_r.setAddress(sensor_r_newAddress);
  delay(10);

  digitalWrite(XSHUT_pin2, 1);
  delay(100);
  sensor_u.setAddress(sensor_u_newAddress);
  //sensor_u.setBus(WIRE1);
  delay(10);

  digitalWrite(XSHUT_pin3, 1);
  delay(100);
  sensor_l.setAddress(sensor_l_newAddress);
  delay(10);

  digitalWrite(XSHUT_pin4, 1);
  delay(100);
  sensor_b.setAddress(sensor_b_newAddress);
  delay(10);

  sensor_r.init();
  sensor_u.init();
  sensor_l.init();
  sensor_b.init();

  delay(2000);

  sensor_r.setTimeout(500);
  sensor_u.setTimeout(500);
  sensor_l.setTimeout(500);
  sensor_b.setTimeout(500);

  sensor_r.startContinuous(5);
  sensor_u.startContinuous(5);
  sensor_l.startContinuous();
  sensor_b.startContinuous();
}

int get_distance(VL53L0X* sensor) {
  static int sensor_old_dis = 0;

  if (sensor->timeoutOccurred()) Serial.print(" TIMEOUT");
  int sensor_dis = sensor->readRangeContinuousMillimeters();

  if (sensor_dis == 0) sensor_dis = -1;
  if (sensor_dis == 8190) sensor_dis = sensor_old_dis;
  else sensor_old_dis = sensor_dis;

  if (sensor_dis == 8191) return -1;
  else return sensor_dis;
}

void debug_dis() {
  display.clearDisplay();
  int right_S = get_distance(&sensor_r);
  int central_S = get_distance(&sensor_u);
  int left_S = get_distance(&sensor_l);  //sensor_r.readRangeContinuousMillimeters()
  int back_S = get_distance(&sensor_b);
  display.setCursor(0, 0);
  display.print("Right_R: " + String(right_S));
  display.setCursor(0, 10);
  display.print("Central_R: " + String(central_S));
  display.setCursor(0, 20);
  display.print("Left_R: " + String(left_S));
  display.setCursor(0, 30);
  display.print("Back_R: " + String(back_S));
  display.display();
  delay(1);
  // Serial.print(" Left_R: ");
  // Serial.print(left_S);
  // Serial.print(" Back_R: ");
  // Serial.println(back_S);
}
