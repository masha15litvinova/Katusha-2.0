#define MOTOR1_1 PB_0_ALT1
#define MOTOR1_2 PB_1_ALT1
#define MOTOR2_1 PE_5
#define MOTOR2_2 PE_6
#define MOTOR3_1 PB_4
#define MOTOR3_2 PB_5
#define MOTOR4_1 PB_11
#define MOTOR4_2 PB_10


/* Includes ------------------------------------------------------------------*/
#include <Wire.h>
#include <vl53l0x_class.h>
#include <VL53L0X.h>

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

VL53L0X sensor;


// Create the OLED display
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

// Create components.
TwoWire WIRE1(PB7, PB6);  //SDA=PB11 & SCL=PB10

/* Setup ---------------------------------------------------------------------*/

void setup() {
  display.begin(0, true);
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init()) {

    while (1) {}
  }
  sensor.startContinuous();
}

void loop() {
  display.setCursor(0,0);
  display.println(sensor.readRangeContinuousMillimeters());
  if (sensor.timeoutOccurred()) { display.println(" TIMEOUT"); }
display.display();
delay(10);
  display.clearDisplay();
}
