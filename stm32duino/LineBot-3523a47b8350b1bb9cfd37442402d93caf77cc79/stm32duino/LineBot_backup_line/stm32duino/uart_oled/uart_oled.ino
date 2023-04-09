#include <SPI.h>
//#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
//#include <SoftwareSerial.h>

#define OLED_MOSI PC12
#define OLED_CLK PC10
#define OLED_DC PC13
#define OLED_CS PA11
#define OLED_RST PA12


#define CAMERA_INTERRUPT PA15


#define LED1 PE0
#define LED2 PE1
#define RX3 PD9

HardwareSerial Serial3(PD9, PD8);
//HardwareSerial SWSerial2(PD5, PD6);


// Create the OLED display
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

// Create components.
//                      RX    TX
//HardwareSerial SWSerial0(PA3, PA2);




/* Setup ---------------------------------------------------------------------*/
int usartData = 0;

void setup() {
  int status;
  // Led.
  pinMode(PA4, OUTPUT);
  digitalWrite(PA4, HIGH);
  // Start OLED
  display.begin(0, true);  // we dont use the i2c address but we will reset!


  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();


  Serial3.setRx(PD9);
  Serial3.setTx(PD8);
  // put your setup code here, to run once:
  pinMode(RX3, INPUT);
  attachInterrupt(RX3, rx3, RISING);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial3.begin(9600);
}


/* Loop ----------------------------------------------------------------------*/

void loop() {
  
}

void rx3() {
  //__disable_irq();
  if (Serial3.available()) {

    usartData = Serial3.read();
    digitalWrite(LED2, HIGH);
    //delay(50);
    //SWSerial2.println(usartData);
  } else {
    digitalWrite(LED2, LOW);
    // delay(50);
  }
  // __enable_irq();
}
