#pragma once
#include "project_config.h"


#ifdef __cplusplus
extern "C"  {
#endif
	
void initPWM();
void initMotorPins();
void setPWM1(int pwm);
void setPWM2(int pwm);
void setPWM3(int pwm);
void setPWM4(int pwm);
	
void setPWMlight(int pwm);
	
#ifdef __cplusplus
}
#endif