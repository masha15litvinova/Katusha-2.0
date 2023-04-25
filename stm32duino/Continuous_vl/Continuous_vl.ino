/* This example shows how to use continuous mode to take
range measurements with the VL53L0X. It is based on
vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

The range readings are in units of mm. */

#include <Wire.h>
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

VL53L0X sensor;
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

TwoWire WIRE1(PB7, PB6);  //SDA=PB11 & SCL=PB10


void setup() {
  Serial.begin(9600);
  Wire.begin();

  display.begin(0, true);
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  sensor.setBus(WIRE1);
  sensor.setAddress(0x31);
  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
}

void loop() {
  display.print(sensor.readRangeContinuousMillimeters());
  if (sensor.timeoutOccurred()) { display.print(" TIMEOUT"); }

  display.display();
  delay(10);
  display.clearDisplay();
}
