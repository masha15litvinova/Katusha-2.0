#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"


#ifdef __cplusplus
extern "C" {
#endif 
void InitMotor1(void);
void SetPWMMotor1(int16_t pwm);
void InitMotor2(void);
void SetPWMMotor2(int16_t pwm);
void InitMotor3(void);
void SetPWMMotor3(int16_t pwm);
void InitMotor4(void);
void SetPWMMotor4(int16_t pwm);
void SetPWM(int16_t pwm1, int16_t pwm2, int16_t pwm3, int16_t pwm4);
void InitAllMotors(void);
void InitTimers(void);

#ifdef __cplusplus
}
#endif