#include "config.h"

void init_display()
{
  display.begin(0, true); // we dont use the i2c address but we will reset!
  display.display();
  delay(400);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SH110X_INVERSE);
  display.setCursor(0, 0);
  display.println("START.INIT");
  display.display();
  delay(1000);
  display.clearDisplay();
}

void draw_info(int str, String text)
{
  display.setTextSize(1);
  display.setTextColor(SH110X_INVERSE);

  display.setCursor(0, str);
  display.println(text);

}
