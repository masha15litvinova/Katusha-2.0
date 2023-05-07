#include <Wire.h>
#include "Adafruit_TCS34725.h"

#define RED_EMPTY 273
#define GREEN_EMPTY 350
#define BLUE_EMPTY 260

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(9600);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1)
      ;
  }

  // Now we're ready to get readings!
}

void loop(void) {
  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  /*Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r); Serial.print(" ");
  Serial.print("G: "); Serial.print(g); Serial.print(" ");
  Serial.print("B: "); Serial.print(b); Serial.print(" ");
  Serial.print("C: "); Serial.print(c); Serial.println(" ");
  Serial.println("Red dev: "+String(abs(r-RED_EMPTY))+" Green dev: "+String(abs(g-GREEN_EMPTY))+" Blue dev: "+String(abs(b-BLUE_EMPTY)));*/
  float sum = (r + g + b);

  float r1 = r/sum;
  float g1 = g/sum;
  float b1 = b/sum;
  Serial.println("r = "+String(r)+" g = "+String(g)+" b = "+String(b));
  Serial.println("r1 = "+String(r1)+" g1 = "+String(g1)+" b1 = "+String(b1));
}
