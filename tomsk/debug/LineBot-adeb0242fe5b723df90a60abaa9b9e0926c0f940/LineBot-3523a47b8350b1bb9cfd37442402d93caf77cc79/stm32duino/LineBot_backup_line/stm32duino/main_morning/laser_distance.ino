#include "config.h"

void init_distance_sensors()
{
  pinMode(LASER_XSHUT_1, OUTPUT);
  pinMode(LASER_XSHUT_2, OUTPUT);
  pinMode(LASER_XSHUT_3, OUTPUT);
  pinMode(LASER_XSHUT_4, OUTPUT);
}

void reset_distance_sensors()
{
  digitalWrite(LASER_XSHUT_1, LOW);
  digitalWrite(LASER_XSHUT_2, LOW);
  digitalWrite(LASER_XSHUT_3, LOW);
  digitalWrite(LASER_XSHUT_4, LOW);
  delay(10);
  digitalWrite(LASER_XSHUT_1, HIGH);
  digitalWrite(LASER_XSHUT_2, HIGH);
  digitalWrite(LASER_XSHUT_3, HIGH);
  digitalWrite(LASER_XSHUT_4, HIGH);
  delay(10);


}

void begin_distance_sensors()
{
  digitalWrite(LASER_XSHUT_2, LOW);
  digitalWrite(LASER_XSHUT_3, LOW);
  digitalWrite(LASER_XSHUT_4, LOW);

  sensor1.begin();
  status1 = sensor1.InitSensor(0x30);


  digitalWrite(LASER_XSHUT_2, HIGH);
  sensor2.begin();
  status2 = sensor2.InitSensor(0x31);

  digitalWrite(LASER_XSHUT_3, HIGH);
  sensor3.begin();
  status3 = sensor3.InitSensor(0x3D);

  digitalWrite(LASER_XSHUT_4, HIGH);
  sensor4.begin();
  status4 = sensor4.InitSensor(0x3F);


  display.clearDisplay();

  // Initialize VL53L0X component.
  //digitalWrite(PA0, HIGH);



  //sensor2.VL53L0X_Off();
  //digitalWrite(PA1, LOW);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  if (status1)
  {
    display.setCursor(0, 0);
    display.println("Init sensor1 failed.");
  
    
  }
  else
  {
    display.setCursor(0, 0);
    display.println("Init sensor1 ok...");
    
   
  }
  if (status2)
  {
    display.setCursor(0, 10);
    display.println("Init sensor2 failed.");
    
   
  }
  else
  {
    display.setCursor(0, 10);
    display.println("Init sensor2 ok...");
  
    
  }
  if (status3)
  {
    display.setCursor(0, 20);
    display.println("Init sensor3 failed.");
  
    
  }
  else
  {
    display.setCursor(0, 20);
    display.println("Init sensor3 ok...");
  
    
  }
  if (status4)
  {
    display.setCursor(0, 30);
    display.println("Init sensor4 failed.");
   
    
  }
  else
  {
    display.setCursor(0, 30);
    display.println("Init sensor4 ok...");
   
    
  }
   display.display();

  delay(1000);

  status1 = sensor1.StaticInit();
  status2 = sensor2.StaticInit();
  status3 = sensor3.StaticInit();
  status4 = sensor4.StaticInit();
}

int distanceSensor1() //справа сбоку
{
  int dist = 0;
  digitalWrite(LASER_XSHUT_1, HIGH);
  status1 = sensor1.GetDistance(&distance1);

  if (status1 == VL53L0X_ERROR_NONE)
  {
    dist = distance1;
  }
  else
  {
    dist = 2000;
  }
  return dist;
}

int distanceSensor2()  //справа спереди
{
  int dist = 0;
  digitalWrite(LASER_XSHUT_2, HIGH);
  status1 = sensor1.GetDistance(&distance2);

  if (status2 == VL53L0X_ERROR_NONE)
  {
    dist = distance2;
  }
  else
  {
    dist = 2000;
  }
  return dist;
}

int distanceSensor3() //слева спереди
{
  int dist = 0;
  digitalWrite(LASER_XSHUT_3, HIGH);
  status3 = sensor3.GetDistance(&distance3);

  if (status3 == VL53L0X_ERROR_NONE)
  {
    dist = distance3;
  }
  else
  {
    dist = 2000;
  }
  return dist;
}

int distanceSensor4() //слева сбоку
{
  int dist = 0;
  digitalWrite(LASER_XSHUT_4, HIGH);
  status4 = sensor4.GetDistance(&distance4);

  if (status4 == VL53L0X_ERROR_NONE)
  {
    dist = distance4;
  }
  else
  {
    dist = 2000;
  }
  return dist;
}
