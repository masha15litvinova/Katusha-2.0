#include <Adafruit_TCS34725.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SmartButton.h>


#define OLED_MOSI PC_12
#define OLED_CLK PC_10
#define OLED_DC PC_13
#define OLED_CS PA_11
#define OLED_RST PA_12

#define SDA2 PF_0
#define SCL2 PF_1
#define SDA3 PC_9
#define SCL3 PA_8

TwoWire WIRE2(SDA2, SCL2);
TwoWire WIRE3(SDA3, SCL3);


Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_4X);

void setup() {
  // put your setup code here, to run once:
  display.begin(0, true);
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
}

void loop() {
  display.clearDisplay();
  uint16_t r, g, b, c;

  delay(25);
  tcs1.getRawData(&r, &g, &b, &c);

  display.setCursor(0, 40);
  display.print("R: " + String(r) + " " + String(g) + " " + String(b));

  delay(25);  // Delay for one old integ. time period (to finish old reading)

  tcs2.getRawData(&r, &g, &b, &c);
  display.setCursor(0, 50);
  display.print("L: " + String(r) + " " + String(g) + " " + String(b) );
  display.display();
}
