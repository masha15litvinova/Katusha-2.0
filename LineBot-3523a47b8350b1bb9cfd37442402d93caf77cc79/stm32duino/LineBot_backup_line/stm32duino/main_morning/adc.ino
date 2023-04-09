#include "config.h"

void init_line_sensors()
{
  pinMode(LIGHT_SENSOR_1, INPUT);
  pinMode(LIGHT_SENSOR_2, INPUT);
  pinMode(LIGHT_SENSOR_3, INPUT);
  pinMode(LIGHT_SENSOR_4, INPUT);
  pinMode(LIGHT_SENSOR_5, INPUT);
  pinMode(LIGHT_SENSOR_6, INPUT);
}
void init_current_sensors()
{
  pinMode(MOTOR1_CURRENT, INPUT);
  pinMode(MOTOR2_CURRENT, INPUT);
}

int lineSensor1()
{
  int s = analogRead(LIGHT_SENSOR_1);
  return s;
}
int lineSensor2()
{
  int s = analogRead(LIGHT_SENSOR_2);
  return s;
}
int lineSensor3()
{
  int s = analogRead(LIGHT_SENSOR_3);
  return s;
}
int lineSensor4()
{
  int s = analogRead(LIGHT_SENSOR_4);
  return s;
}
int lineSensor5()
{
  int s = analogRead(LIGHT_SENSOR_5);
  return s;
}
int lineSensor6()
{
  int s = analogRead(LIGHT_SENSOR_6);
  return s;
}
int currentSensor1()
{
  int s = analogRead(MOTOR1_CURRENT);
  return s;
}
int currentSensor2()
{
  int s = analogRead(MOTOR2_CURRENT);
  return s;
}
