
/* Includes ------------------------------------------------------------------*/
#include <Wire.h>
#include <vl53l0x_class.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define OLED_MOSI PC12
#define OLED_CLK PC10
#define OLED_DC PC13
#define OLED_CS PA11
#define OLED_RST PA12

//#define HIGH_SPEED

// Create the OLED display
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

// Create components.
TwoWire WIRE1(PB7, PB6);        //SDA=PB11 & SCL=PB10
VL53L0X sensor1(&WIRE1, PG10);  //XSHUT=PG10
VL53L0X sensor2(&WIRE1, PG11);  //XSHUT=PG11
VL53L0X sensor3(&WIRE1, PG12);  //XSHUT=PG11
VL53L0X sensor4(&WIRE1, PG13);  //XSHUT=PG11
/* Setup ---------------------------------------------------------------------*/

void setup() {
  int status1;
  int status2;
  int status3;
  int status4;
  // Led.
  pinMode(PE6, OUTPUT);

  // Initialize serial for output.
  Serial.begin(9600);

  

  pinMode(PG10, OUTPUT);
  pinMode(PG11, OUTPUT);
  pinMode(PG12, OUTPUT);
  pinMode(PG13, OUTPUT);

  pinMode(PA9, OUTPUT);
  pinMode(PA10, OUTPUT);
  pinMode(PE2, OUTPUT);
  pinMode(PE3, OUTPUT);


  /*digitalWrite(PA9, LOW);
  digitalWrite(PA10, LOW);
  digitalWrite(PE2, LOW);
  digitalWrite(PE3, LOW);
  delay(500);
  digitalWrite(PA9, HIGH);
  digitalWrite(PA10, HIGH);
  digitalWrite(PE2, HIGH);
  digitalWrite(PE3, HIGH);
  delay(500);*/

// Initialize I2C bus.
  WIRE1.begin();
  //reset all sensors
  digitalWrite(PG10, LOW);
  digitalWrite(PG11, LOW);
  digitalWrite(PG12, LOW);
  digitalWrite(PG13, LOW);
  delay(10);
  digitalWrite(PG10, HIGH);
  digitalWrite(PG11, HIGH);
  digitalWrite(PG12, HIGH);
  digitalWrite(PG13, HIGH);
  delay(10);

  digitalWrite(PG10, LOW);
  digitalWrite(PG11, LOW);
  digitalWrite(PG12, LOW);
  digitalWrite(PG13, LOW);
  /*sensor1.SetMeasurementTimingBudgetMicroSeconds(200000);
  sensor2.SetMeasurementTimingBudgetMicroSeconds(200000);
  sensor3.SetMeasurementTimingBudgetMicroSeconds(200000);
  sensor4.SetMeasurementTimingBudgetMicroSeconds(200000);*/
  // Configure VL53L0X component.
  digitalWrite(PG10, HIGH);
  sensor1.begin();
  status1 = sensor1.InitSensor(0xC0);


  digitalWrite(PG11, HIGH);
  sensor2.begin();
  status2 = sensor2.InitSensor(0x31);

  digitalWrite(PG12, HIGH);
  sensor3.begin();
  status3 = sensor3.InitSensor(0x10);

  digitalWrite(PG13, HIGH);
  sensor4.begin();
  status4 = sensor4.InitSensor(0x3C);


  // Start OLED
  display.begin(0, true);  // we dont use the i2c address but we will reset!
  display.display();
  delay(400);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SH110X_INVERSE);
  display.setCursor(0, 0);
  display.println("START.INIT");
  display.display();
  delay(1000);
  Serial.println("");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.


  // Clear the buffer.
  display.clearDisplay();

  // Initialize VL53L0X component.
  //digitalWrite(PG10, HIGH);



  //sensor2.VL53L0X_Off();
  //digitalWrite(PG11, LOW);
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
}


/* Loop ----------------------------------------------------------------------*/
int status1 = sensor1.StaticInit();
int status2 = sensor2.StaticInit();
int status3 = sensor3.StaticInit();
int status4 = sensor4.StaticInit();

void loop() {
  // Led blinking.

  // Read Range.
  uint32_t distance1;
  uint32_t distance2;
  uint32_t distance3;
  uint32_t distance4;
  //int status1;
  //int status2;

  //digitalWrite(A0, LOW);

  //sensor1.VL53L0X_On();

  int time_now = millis();

  digitalWrite(PG10, HIGH);
  status1 = sensor1.GetDistance(&distance1);

  digitalWrite(PG11, HIGH);
  status2 = sensor2.GetDistance(&distance2);

  digitalWrite(PG12, HIGH);
  status3 = sensor3.GetDistance(&distance3);

  digitalWrite(PG13, HIGH);
  status4 = sensor4.GetDistance(&distance4);

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
    Serial.println(report);
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
    Serial.println(report);
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
    Serial.println(report);
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
    Serial.println(report);
  } else {
    display.setCursor(0, 30);
    display.println("Error4: " + String(status4));
    display.display();
  }
  int period = millis() - time_now;
  char report[64];

  snprintf(report, sizeof(report), "| Time : %ld |", period);
  display.setCursor(0, 40);
  display.println(report);
  display.display();
  Serial.println(report);
}
