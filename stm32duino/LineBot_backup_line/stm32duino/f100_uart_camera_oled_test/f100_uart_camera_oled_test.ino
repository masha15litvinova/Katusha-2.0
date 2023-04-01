#include <SPI.h>
//#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
//#include <SoftwareSerial.h>

#define OLED_MOSI     PB15
#define OLED_CLK      PB13
#define OLED_DC       PB5
#define OLED_CS       PB12
#define OLED_RST      PC13


#define CAMERA_INTERRUPT PA15


// Create the OLED display
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

// Create components.
//                      RX    TX
HardwareSerial SWSerial0(PA3, PA2);

byte usartData = 0;
int cam_data[] = {0, 0, 0, 0};
int count = 0;
void uart_cam_read()
{
  __disable_irq();
  digitalWrite(PA4, HIGH);
  if (SWSerial0.available())
  {
    
    usartData = SWSerial0.read();
    cam_data[count] = usartData;
    count++;

  }
  __enable_irq();
}


/* Setup ---------------------------------------------------------------------*/

void setup() {
  int status;
  // Led.
  pinMode(PA4, OUTPUT);

  // Initialize serial for output.
  SWSerial0.begin(115200);

  // Start OLED
  display.begin(0, true); // we dont use the i2c address but we will reset!


  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  
  //attachInterrupt(CAMERA_INTERRUPT, uart_cam_read, RISING);

}


/* Loop ----------------------------------------------------------------------*/

void loop() {
  // Led blinking.
  digitalWrite(PA4, HIGH);
  delay(100);
  digitalWrite(PA4, LOW);

  // Read Range.
if (SWSerial0.available())
  {
    
    usartData = SWSerial0.read();

  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println(String(usartData));
  display.display();
  //Serial.println(report);
}
