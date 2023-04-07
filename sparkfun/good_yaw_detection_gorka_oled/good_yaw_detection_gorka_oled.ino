#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10  // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] = { 0b00000000, 0b11000000,
                                                  0b00000001, 0b11000000,
                                                  0b00000001, 0b11000000,
                                                  0b00000011, 0b11100000,
                                                  0b11110011, 0b11100000,
                                                  0b11111110, 0b11111000,
                                                  0b01111110, 0b11111111,
                                                  0b00110011, 0b10011111,
                                                  0b00011111, 0b11111100,
                                                  0b00001101, 0b01110000,
                                                  0b00011011, 0b10100000,
                                                  0b00111111, 0b11100000,
                                                  0b00111111, 0b11110000,
                                                  0b01111100, 0b11110000,
                                                  0b01110000, 0b01110000,
                                                  0b00000000, 0b00110000 };

#include <SparkFunMPU9250-DMP.h>

#define SerialPort SerialUSB
#define LEDPIN 13

#define TIME_DELAY 12

void (*resetFunc)(void) = 0;

MPU9250_DMP imu;

bool state = 1;
float drift = 0;
unsigned long time_begin = millis();
unsigned long time_led = millis();
unsigned long time_integrated = millis();
byte calibrated = 0;
bool send = true;
void setup() {
  // SerialPort.begin(9600);
  Serial1.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //Serial.println(F("SSD1306 allocation failed"));
  }
  display.display();
  delay(2);  // Pause for 2 seconds


  pinMode(LEDPIN, OUTPUT);
  // Call imu.begin() to verify communication and initialize
  if (imu.begin() != INV_SUCCESS) {
    while (1) {

      delay(5000);
    }
  }

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT |  // Enable 6-axis quat
                 DMP_FEATURE_GYRO_CAL,   // Use gyro calibration
               10);                      // Set DMP FIFO rate to 10 Hz
  // DMP_FEATURE_LP_QUAT can also be used. It uses the
  // accelerometer in low-power mode to estimate quat's.
  // DMP_FEATURE_LP_QUAT and 6X_LP_QUAT are mutually exclusive
}
int count = 0;

float p = 0;
float p1 = 0.0;
byte send_pitch = 0;
byte angle_y = 0;
byte angle_p = 0;
byte angle_r = 0;
float pitch_robot = 0.0;
float last_p = 0.0;
float last_last_p = 0.0;
float old_pitch_robot = 0.0;
float k_filter = 0.55;
long int time_gorka_start = 0;
void loop() {
  // Check for new data in the FIFO
  if (imu.fifoAvailable()) {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if (imu.dmpUpdateFifo() == INV_SUCCESS) {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      imu.computeEulerAngles();
      imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
      printIMUData();
      float accelX = imu.calcAccel(imu.ax);
      float accelY = imu.calcAccel(imu.ay);
      float accelZ = imu.calcAccel(imu.az);
      float gyroX = imu.calcGyro(imu.gx);
      float gyroY = imu.calcGyro(imu.gy);
      float gyroZ = imu.calcGyro(imu.gz);
      float magX = imu.calcMag(imu.mx);
      float magY = imu.calcMag(imu.my);
      float magZ = imu.calcMag(imu.mz);
      pitch_robot = (atan((accelY) / (accelZ)) * 180 / PI);
      accelY = constrain(accelY, -1.0, 1.0);
      //p = pitch_robot * k_filter + old_pitch_robot * (1 - k_filter);
      /*if (count > 199) {
        p = p + (gyroX - drift) * (millis() - time_integrated) * 0.001;
      } else {
        p = 0;
      }*/
      time_integrated = millis();


      /*if (abs(p) > 7) {
        if ((p < 20) and (p > 0)) p = 20;
        else if ((p < -20) and (p < 0)) p = -20;
      }*/
      if (p > 80) p = 80;
      else if (p < -80) p = -80;
      //if (abs(gyroZ) > 60) p = 0;

      send_pitch = map(round(p1), -180, 180, 0, 255);
      if (count < 200) {
        drift = drift + gyroX * 0.005;
        count++;
      }
      p = ((acos(accelY) * 180) / PI) - 90.0;

      //float middle = (p < last_p) ? ((last_p < last_last_p) ? last_p : ((last_last_p < p) ? p : last_last_p)) : ((p < last_last_p) ? p : ((last_last_p < last_p) ? last_p : last_last_p));
      p1 = (p)*k_filter + p1 * (1.0 - k_filter);
      /*SerialPort.print(p);
      SerialPort.print(",");
      SerialPort.println(p1);*/
      // SerialPort.println(accelY);
      last_last_p = last_p;
      last_p = p1;
    }
  }



  if ((millis() - time_begin) > 30000) calibrated = 1;
  if (Serial1.available()) {

    int signal = Serial1.read();
    //Serial.println(signal);
    if (signal == 1) {
      resetFunc();
    }
    if (signal == 2) {
      send = false;
    }
    if (signal == 3) {
      send = true;
    }
    display.clearDisplay();
    display.display();
    display.setCursor(0, 0);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println(signal);
    display.display();
  }

  if (send) {
    if ((millis() - time_led) > 90) {
      digitalWrite(LEDPIN, !state);
      time_led = millis();
    }
    Serial1.print(':');
    delay(TIME_DELAY);
    Serial1.print(angle_y);
    delay(TIME_DELAY);
    Serial1.print('/');
    delay(TIME_DELAY);
    Serial1.print(send_pitch);
    delay(TIME_DELAY);
    Serial1.print('/');
    delay(TIME_DELAY);
    Serial1.print(calibrated);
    delay(TIME_DELAY);
    Serial1.print('/');
    delay(TIME_DELAY);
    Serial1.print(';');
    delay(TIME_DELAY);
    digitalWrite(LEDPIN, LOW);
  }  //*/
  //SerialPort.println("p = " + String(p) + " drift = " + String(drift));
  old_pitch_robot = pitch_robot;
}

void printIMUData(void) {
  digitalWrite(LEDPIN, OUTPUT);
  // After calling dmpUpdateFifo() the ax, gx, mx, etc. values
  // are all updated.
  // Quaternion values are, by default, stored in Q30 long
  // format. calcQuat turns them into a float between -1 and 1
  float q0 = imu.calcQuat(imu.qw);
  float q1 = imu.calcQuat(imu.qx);
  float q2 = imu.calcQuat(imu.qy);
  float q3 = imu.calcQuat(imu.qz);

  int yaw1 = imu.yaw;
  int pitch1 = imu.pitch;
  int roll1 = imu.roll;

  angle_y = map(yaw1, 0, 360, 0, 255);
  angle_p = map(pitch1, 0, 360, 0, 255);
  angle_r = map(roll1, 0, 360, 0, 255);
}
