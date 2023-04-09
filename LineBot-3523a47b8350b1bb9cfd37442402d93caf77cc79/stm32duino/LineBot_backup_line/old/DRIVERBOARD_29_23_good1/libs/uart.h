#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"


#ifdef __cplusplus
extern "C" {
#endif
void uart_init(void);
int Usart_Data();
int map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);
int motor1();
int motor2();
int motor3();
int motor4();	
#ifdef __cplusplus
}
#endif