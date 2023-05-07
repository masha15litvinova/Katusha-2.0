#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"

#ifdef __cplusplus
 extern "C" {
#endif
void Init_Encoders(void);

int Encoder1();
int Encoder2();
int Encoder3();
int Encoder4();

 #ifdef __cplusplus
}
#endif