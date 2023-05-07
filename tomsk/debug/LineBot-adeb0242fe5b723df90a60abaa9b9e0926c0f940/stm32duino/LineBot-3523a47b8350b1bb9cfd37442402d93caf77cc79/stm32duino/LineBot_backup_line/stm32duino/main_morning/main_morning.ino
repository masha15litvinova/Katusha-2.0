//#include <BMP180.h>
//#include <I2Cdev.h>
//#include <MPU9250.h>

#include "config.h"

#include <SPI.h>
#include <Wire.h>
//#include <SoftwareSerial.h>
 

#include "vl53l0x_class.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "Servo.h"


Servo winchServo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int a = 0;
int b = 0;
int pos = 0;    // variable to store the servo position
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

HardwareSerial UART2_CAMERA(UART2_RX_CAMERA, UART2_TX_CAMERA);
HardwareSerial UART1_DRIVERBOARD(UART1_RX_DRIVER_MCU, UART1_TX_DRIVER_MCU);
HardwareSerial UART3_GYRO(UART3_RX_GYRO, UART3_TX_GYRO);

TwoWire WIRE1(LASER_I2C1_SDA, LASER_I2C1_SCL);  //SDA=PB11 & SCL=PB10
VL53L0X sensor1(&WIRE1, LASER_XSHUT_1); //XSHUT=PA0
VL53L0X sensor2(&WIRE1, LASER_XSHUT_2); //XSHUT=PA1
VL53L0X sensor3(&WIRE1, LASER_XSHUT_3); //XSHUT=PA1
VL53L0X sensor4(&WIRE1, LASER_XSHUT_4); //XSHUT=PA1

int white = 0;
int black1 = 0;
int black2 = 0;
int black3 = 0;
int black4 = 0;
int black5 = 0;
int black6 = 0;

int black = 0;

int white1 = 1023;
int white2 = 1023;
int white3 = 1023;
int white4 = 1023;
int white5 = 1023;
int white6 = 1023;

int grey1 = 0;
int grey2 = 0;
int grey3 = 0;
int grey4 = 0;
int grey5 = 0;
int grey6 = 0;

#define difference 600

#define obstacle_dist 3500
#define obstacle_dist_left 2000


byte usartData = 0;
int cam_data[] = {0, 0, 0, 0};
int count = 0;

byte usartData_gyro = 0;
byte gyro_data[] = {0, 0, 0};
int count_gyro = 0;

int direction_move = 0;
int line_dev = 0;
int angle = 0;

int angle_yaw_gyro = 0; 
int angle_gyro = 0;

uint16_t byte_signal_gyro = 255; //сигнал от гироскопа
uint16_t byte_signal = 253; //сигнал от камеры
uint16_t byte_motor = 255;  //сигнал моторы

int16_t err_sensors[] = {0, 0, 0, 0, 0 , 0};
int16_t err_old_sensors[] = {0, 0, 0, 0, 0 , 0};

//float sensors_kp[] = {-0.06, -0.05, -0.04, 0.04, 0.05, 0.06};
float sensors_kp[] = {-0.045, -0.04, -0.035, 0.035, 0.04, 0.045};
//float sensors_ki[] = {-0.0012, -0.0006, -0.0004, 0.0004, 0.0006, 0.0012};
float sensors_ki[] = {-0.00017, -0.00015, -0.0001, 0.0001, 0.00015, 0.00017};

//float sensors_kd[] = {0.004, 0.003, 0.0025, -0.0025, -0.003, -0.004};
float sensors_kd[] = {0.0035, 0.0025, 0.0015, -0.0015, -0.0025, -0.0035};
/*0 - don't know
        1 - right
        2 - left
        3 - straight forward
        4 - backward*/
float angle_m = 0;
int up = 0;
int ui = 0;
int ud;
int u = 0;

int count_obstacles = 0;

uint16_t forward_dist = 0;
uint16_t left_dist = 0;

int16_t line = 0;
int16_t dir = 0;

uint16_t s6 = 0;

int u_sens_p = 0;
int u_sens_i = 0;
float u_sens_d = 0;
float u_sens = 0;

float err = 0;
float err_old = 0;

bool cross_completed = true;

float k_vel = 0.000;
int v = 20;



int status1;  //статус дальномеров
int status2;
int status3;
int status4;

uint32_t distance1; //показания дальномеров
uint32_t distance2;
uint32_t distance3;
uint32_t distance4;


void setup()
{
  pinMode(PA3, OUTPUT);
  digitalWrite(PA3, HIGH);
  WIRE1.begin();
  interrupts();
  __enable_irq();
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(CAMERA_INTERRUPT, INPUT);
  pinMode(GYRO_INTERRUPT, INPUT);
  pinMode(END_SWITCH_1, INPUT);
  pinMode(END_SWITCH_2, INPUT);
  init_motors_grab();
  init_display();
  winchServo.attach(SERVO1);
  winchServo.write(0);
  UART2_CAMERA.begin(115200);
  UART1_DRIVERBOARD.begin(115200);
  UART3_GYRO.begin(115200);
  //init_button();
  attachInterrupt(CAMERA_INTERRUPT, uart_cam_read, RISING);
  attachInterrupt(GYRO_INTERRUPT, uart_gyro_read, RISING);
  //attachInterrupt(END_SWITCH_1, left_correction, CHANGE);
  //attachInterrupt(END_SWITCH_2, right_correction, CHANGE);
  /*init_distance_sensors();
  reset_distance_sensors();
  begin_distance_sensors();*/
  
  //wait_button();
  calibration_sensors();
}

void loop()
{
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  display.clearDisplay();
  //send_motors(50, 50, 50, 50);
  
 /* if (UART1_DRIVERBOARD.available())
  {
    digitalWrite(LED_1, HIGH);
    usartData = UART1_DRIVERBOARD.read();
  }
   draw_info(10, "receive: " + String(usartData));*/
  //send_motors(40, 40, 40, 40);
  //uart_gyro_read();
    //draw_info(0, "current1: " + String(analogRead(MOTOR1_CURRENT)));
    //draw_info(10, "current2: " + String(analogRead(MOTOR2_CURRENT)));
       
  //delay(50);
 /* uart_cam_read();*/

  int s1 = lineSensor1();
    int s2 = lineSensor2();
    int s3 = lineSensor3();
    int s4 = lineSensor4();
    int s5 = lineSensor5();
    int s6 = lineSensor6();

    draw_info(30, String(s1) + " " + String(s2) + " " + String(s3));
    draw_info(40, String(s4) + " " + String(s5) + " " + String(s6));
    /*draw_info(10, String(s1) + " " + String(s2) + " " + String(s3));
    draw_info(20, String(s4) + " " + String(s5) + " " + String(s6));
   // draw_info(0, "yaw: " + String(get_angle_yaw_gyro()));
    
    /*draw_info(30, "line_deviation(): " + String(line_dev));
    draw_info(40, "direction_turn(): " + String(direction_move));*/
    //delay(5);
  /*0 - don't know
        1 - right
        2 - left
        3 - straight forward
        4 - backward*/




  /*int dist = distanceSensor1();*/
    //draw_info(40, "Distance1: " + String(distanceSensor1()));
  /*draw_info(40, "Distance2: " + String(distanceSensor2()));
    draw_info(50, "Distance3: " + String(distanceSensor3()));
    draw_info(60, "Distance4: " + String(distanceSensor4()));*/


  err_sensors[0] = lineSensor1() - grey1;
    err_sensors[1] = lineSensor2() - grey2;
    err_sensors[2] = lineSensor3() - grey3;
    err_sensors[3] = lineSensor4() - grey4;
    err_sensors[4] = lineSensor5() - grey5;
    err_sensors[5] = lineSensor6() - grey6;
    u_sens_p = err_sensors[0] * sensors_kp[0] + err_sensors[1] * sensors_kp[1] + err_sensors[2] * sensors_kp[2] + err_sensors[3] * sensors_kp[3] + err_sensors[4] * sensors_kp[4] + err_sensors[5] * sensors_kp[5];
    u_sens_d = (err_sensors[0] - err_old_sensors[0]) * sensors_kd[0] + (err_sensors[1] - err_old_sensors[1]) * sensors_kd[1] + (err_sensors[3] - err_old_sensors[4]) * sensors_kd[4] + (err_sensors[5] - err_old_sensors[5]) * sensors_kd[5];
    u_sens_i = u_sens_i + err_sensors[0] * sensors_ki[0] + err_sensors[1] * sensors_ki[1] + err_sensors[2] * sensors_ki[2] + err_sensors[3] * sensors_ki[3] + err_sensors[4] * sensors_ki[4] + err_sensors[5] * sensors_ki[5];


    u_sens = u_sens_p + u_sens_i + u_sens_d;

    if ((lineSensor1() < white1+20) and (lineSensor2() < white2+20) and (lineSensor3() < white3+20) and (lineSensor4() < white4+20) and (lineSensor5() < white5+20) and (lineSensor6() < white6+20))
    {
     u_sens = 0;
    }


    for (int i = 0; i < 6; i++)
    {
     err_old_sensors[i] = err_old_sensors[i];
    }

    //forward_dist = distanceSensor1();
    //left_dist = distanceSensor2();

    //  if ((forward_dist > obstacle_dist) and (count_obstacles == 0))
    //  {
    //    obstruction();
    //    count_obstacles = 1;
    //  }

    line = line_deviation();


    err = line;


    angle_m = marker_angle();



    up = p_reg(err, -0.3, -0.000005, -0.00000000000);  //-0.0000031 //-0.000009 //-0.52 //-0.00000085  //-0.000002
    //up = 0;
    ui = i_reg(err, -0.000135);
    ud = d_reg(3.5);
    u = up + ui + ud;
    if ((lineSensor1() > white + 100) or (lineSensor2() > white + 100) or (lineSensor3() > white + 100) or (lineSensor4() > white + 100) or (lineSensor5() > white + 100) or (lineSensor6() > white + 100))
    {
     u = 0;
    }
    u = 0;
    //send_motors(50, 50, 50, 50);
    send_motors(v + u + u_sens, -v + u + u_sens, -v + u + u_sens, v + u + u_sens);

    if (abs(err) < 4)
    {
     ui = 0;
    }

    //delay(3);

    err_old = err;
    dir = direction_turn();
    if ((dir == 1) or (dir == 2) or (dir == 4) or (dir == 3))
    {
     cross_completed = false;
    }


/*
    if ((lineSensor1() + lineSensor2() + lineSensor3() + lineSensor4() + lineSensor5() + lineSensor6() > black * 3.5) and !cross_completed)
    {

     check_direction();

     //dir = direction_turn();


     if (dir == 1)
     {
       right();
       cross_completed = true;
     }
     else if (dir == 2)
     {
       left();
       cross_completed = true;
     }
     else if (dir == 4)
     {
       back();
       cross_completed = true;
     }



    }*/
  
  //uart_cam_read();
  
  display.display();

}


void obstruction()
{
  send_motors(0, 0, 0, 0);
  delay(300);
  int sp = 40;

  send_motors(-sp, -sp, -sp, -sp);
  delay(3200);
  int tim = 0;

  while (((lineSensor1() < black1) and (lineSensor6() < black6)) and (tim < 200)) //(sensor1()<black)and(sensor6()<black)
  {
    send_motors(-sp, sp, sp, -sp);
    delay(1);
    tim++;
  }
  send_motors(0, 0, 0, 0);
  send_motors(-sp, sp, sp, -sp);
  delay(300);

  if ((lineSensor1() > black1) and (lineSensor6() < black6))
  {
    while ((-difference < (lineSensor1() - lineSensor6())) and (difference > (lineSensor1() - lineSensor6())))
    {
      int s_turn = 25;
      send_motors(s_turn, s_turn, s_turn, s_turn);
      delay(1);
    }
    send_motors(0, 0, 0, 0);
  }
  else if ((-difference < (lineSensor1() - lineSensor6())) and (difference > (lineSensor1() - lineSensor6())))
  {
    while ((lineSensor1() < black1 - 100) and (lineSensor6() > black6))
    {
      int s_turn = -25;
      send_motors(s_turn, s_turn, s_turn, s_turn);
      delay(1);
    }
    send_motors(0, 0, 0, 0);
  }

  send_motors(sp, -sp, -sp, sp);
  delay(2800);
  send_motors(sp, sp, sp, sp);
  delay(3100);
  left_dist = distanceSensor1();
  while (left_dist < obstacle_dist_left)
  {
    send_motors(sp, -sp, -sp, sp);
    delay(1);
    left_dist = distanceSensor1();
  }
  while (left_dist > obstacle_dist_left)
  {
    send_motors(sp, -sp, -sp, sp);
    delay(1);
    left_dist = distanceSensor1();
  }
  send_motors(0, 0, 0, 0);
  delay(300);
  send_motors(sp, -sp, -sp, sp);
  delay(1200);
  sp = 44;
  send_motors(sp, sp, sp, sp);
  delay(3200);

  while ((lineSensor1() + lineSensor2() + lineSensor5() + lineSensor6()) < 3 * black)
  {
    send_motors(sp, -sp, -sp, sp);
    delay(1);
  }

  send_motors(sp, -sp, -sp, sp);
  delay(600);

  while (lineSensor6() < black - 400)
  {
    send_motors(-sp, -sp, -sp, -sp);
    delay(1);

  }
  while (lineSensor6() > white + 400)
  {
    send_motors(-sp, -sp, -sp, -sp);
    delay(1);

  }
  send_motors(0, 0, 0, 0);
  delay(300);
  send_motors(-sp, -sp, -sp, -sp);
  delay(700);
  send_motors(0, 0, 0, 0);
  send_motors(-sp, sp, sp, -sp);
  delay(600);
  send_motors(0, 0, 0, 0);
}

int p_reg(int err_in, float kp, float kp2, float kp3)
{
  int u = kp * err_in + kp2 * err_in * err_in * err_in;
  /*if (err_in>0)
    {
    u = u+kp2*err_in*err_in*err_in*err_in;
    }
    else
    {
    u = u-kp2*err_in*err_in*err_in*err_in;
    }*/
  return u;
}

int i_reg(int err_in, float ki)
{
  ui = ui + err_in * ki;
  int u = ui;
  return u;
}

int d_reg(float kd)
{
  ud = (err - err_old) * kd;
  int u = ud;
  return u;
}
void right()
{

  display.clearDisplay();

  draw_info(0, "RIGHT");
  delay(1000);
  display.display();

  int sp = -40;


  send_motors(0, 0, 0, 0);
  delay(600);

  int t = 0;

  while ((lineSensor5() < black5 - 30) or (t < 250))
  {
    send_motors(sp, sp, sp, sp);
    delay(1);
    t++;
  }
  while (lineSensor6() > white6 + 40)
  {
    send_motors(sp, sp, sp, sp);
    delay(1);
  }
  send_motors(sp, sp, sp, sp);
  delay(700);
  send_motors(0, 0, 0, 0);
  delay(600);
  send_motors(0, 0, 0, 0);


}

void left()
{

  display.clearDisplay();

  draw_info(0, "LEFT");
  delay(1000);
  display.display();

  int sp = 40;


  send_motors(0, 0, 0, 0);
  delay(600);

  int t = 0;

  while ((lineSensor2() < black2-30) or (t < 250))
  {
    send_motors(sp, sp, sp, sp);
    delay(1);
    t++;
  }
  while (lineSensor1() > white1 + 30)
  {
    send_motors(sp, sp, sp, sp);
    delay(1);
  }
  send_motors(sp, sp, sp, sp);
  delay(700);
  send_motors(0, 0, 0, 0);
  delay(600);
  send_motors(0, 0, 0, 0);


}
void back()
{

  display.clearDisplay();

  draw_info(0, "BACKWARD");
  delay(1000);
  display.display();

  int sp = 30;

  send_motors(0, 0, 0, 0);
  delay(300);


  send_motors(2 * sp, 2 * sp, 2 * sp, 2 * sp);
  delay(1000);
  while (lineSensor1() < black1 - 45)
  {
    send_motors(2 * sp, 2 * sp, 2 * sp, 2 * sp);
    delay(1);
  }
  while (lineSensor1() > white1 + 45)
  {
    send_motors(2 * sp, 2 * sp, 2 * sp, 2 * sp);
    delay(1);
  }
  send_motors(2 * sp, 2 * sp, 2 * sp, 2 * sp);
  delay(400);

  send_motors(0, 0, 0, 0);


}


void check_direction()
{
  int sp = 45;
  //send_motors(-sp, sp, sp, -sp);
  //delay(1200);
  send_motors(0, 0, 0, 0);
  delay(800);
  send_motors(-sp, sp, sp, -sp);
  delay(1000);
  send_motors(0,0,0,0);
delay(500);
  dir = direction_turn();


  while (lineSensor2() + lineSensor3() + lineSensor4() + lineSensor5() < black * 3.5)
  {
    send_motors(sp, -sp, -sp, sp);
    delay(1);
  }
  send_motors(0, 0, 0, 0);
  delay(500);
  send_motors(sp, -sp, -sp, sp);
  delay(300);
  send_motors(0, 0, 0, 0);
  delay(500);
}

void calibration_sensors()
{
  int sp = 35;
  int s1, s2, s3, s4, s5, s6;
  delay(100);
  long time_start = millis();
  while ((millis() - time_start) < 2300)
  {
    send_motors(sp, sp, sp, sp);
    s1 = lineSensor1();
    s2 = lineSensor2();
    s3 = lineSensor3();
    s4 = lineSensor4();
    s5 = lineSensor5();
    s6 = lineSensor6();
    display.clearDisplay();
    draw_info(0, "BLACK/WHITE");
    draw_info(10, String(s1) + " " + String(s2) + " " + String(s3));
    draw_info(20, String(s4) + " " + String(s5) + " " + String(s6));
    display.display();
    delay(1);
    if (s1 > black1)
    {
      black1 = s1;
    }
    if (s2 > black2)
    {
      black2 = s2;
    }
    if (s3 > black3)
    {
      black3 = s3;
    }
    if (s4 > black4)
    {
      black4 = s4;
    }
    if (s5 > black5)
    {
      black5 = s5;
    }
    if (s6 > black6)
    {
      black6 = s6;
    }

    if (s1 < white1)
    {
      white1 = s1;
    }
    if (s2 < white2)
    {
      white2 = s2;
    }
    if (s3 < white3)
    {
      white3 = s3;
    }
    if (s4 < white4)
    {
      white4 = s4;
    }
    if (s5 < white5)
    {
      white5 = s5;
    }
    if (s6 < white6)
    {
      white6 = s6;
    }

  }
  time_start = millis();
  while ((millis() - time_start) < 4600)
  {
    send_motors(-sp, -sp, -sp, -sp);
    s1 = lineSensor1();
    s2 = lineSensor2();
    s3 = lineSensor3();
    s4 = lineSensor4();
    s5 = lineSensor5();
    s6 = lineSensor6();
    display.clearDisplay();
    draw_info(0, "BLACK/WHITE");
    draw_info(10, String(s1) + " " + String(s2) + " " + String(s3));
    draw_info(20, String(s4) + " " + String(s5) + " " + String(s6));
    display.display();
    delay(1);
    if (s1 > black1)
    {
      black1 = s1;
    }
    if (s2 > black2)
    {
      black2 = s2;
    }
    if (s3 > black3)
    {
      black3 = s3;
    }
    if (s4 > black4)
    {
      black4 = s4;
    }
    if (s5 > black5)
    {
      black5 = s5;
    }
    if (s6 > black6)
    {
      black6 = s6;
    }

    if (s1 < white1)
    {
      white1 = s1;
    }
    if (s2 < white2)
    {
      white2 = s2;
    }
    if (s3 < white3)
    {
      white3 = s3;
    }
    if (s4 < white4)
    {
      white4 = s4;
    }
    if (s5 < white5)
    {
      white5 = s5;
    }
    if (s6 < white6)
    {
      white6 = s6;
    }

  }
  time_start = millis();
  while ((millis() - time_start) < 2150)
  {
    send_motors(sp, sp, sp, sp);
    s1 = lineSensor1();
    s2 = lineSensor2();
    s3 = lineSensor3();
    s4 = lineSensor4();
    s5 = lineSensor5();
    s6 = lineSensor6();
    display.clearDisplay();
    draw_info(0, "BLACK/WHITE");
    draw_info(10, String(s1) + " " + String(s2) + " " + String(s3));
    draw_info(20, String(s4) + " " + String(s5) + " " + String(s6));
    display.display();
    delay(1);
    if (s1 > black1)
    {
      black1 = s1;
    }
    if (s2 > black2)
    {
      black2 = s2;
    }
    if (s3 > black3)
    {
      black3 = s3;
    }
    if (s4 > black4)
    {
      black4 = s4;
    }
    if (s5 > black5)
    {
      black5 = s5;
    }
    if (s6 > black6)
    {
      black6 = s6;
    }

    if (s1 < white1)
    {
      white1 = s1;
    }
    if (s2 < white2)
    {
      white2 = s2;
    }
    if (s3 < white3)
    {
      white3 = s3;
    }
    if (s4 < white4)
    {
      white4 = s4;
    }
    if (s5 < white5)
    {
      white5 = s5;
    }
    if (s6 < white6)
    {
      white6 = s6;
    }

  }


  black = (black1 + black2 + black3 + black4 + black5 + black6) / 6;




  grey1 = (black1 + white1) / 2;
  grey2 = (black2 + white2) / 2;
  grey3 = (black3 + white3) / 2;
  grey4 = (black4 + white4) / 2;
  grey5 = (black5 + white5) / 2;
  grey6 = (black6 + white6) / 2;
  display.clearDisplay();
  draw_info(0, "GREY");
  draw_info(10, String(grey1) + " " + String(grey2) + " " + String(grey3));
  draw_info(20, String(grey4) + " " + String(grey5) + " " + String(grey6));
  display.display();
  delay(1000);
}
