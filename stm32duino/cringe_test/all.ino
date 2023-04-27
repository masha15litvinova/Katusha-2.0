void initColorSensors() {
  display.setTextColor(SH110X_WHITE);
  if (tcs1.begin(0x29, &WIRE2)) {
    display.setCursor(0, 40);
    display.println("Found TCS1 sensor");
    display.display();
    delay(300);
  } else {
    display.setCursor(0, 40);
    display.println("No TCS1 found");
    display.display();
    delay(300);
  }
  if (tcs2.begin(0x29, &WIRE3)) {
    display.setCursor(0, 50);
    display.println("Found TCS2 sensor");
    display.display();
    delay(300);
  } else {
    display.setCursor(0, 50);
    display.println("No TCS2 found");
    display.display();
    delay(300);
  }
}