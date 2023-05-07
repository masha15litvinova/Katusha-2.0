#pragma once
#define GPIO_SetPin(x) GPIO_SetBits(x.GPIOx,x.pinNum)
#define GPIO_ResetPin(x) GPIO_ResetBits(x.GPIOx,x.pinNum)
#define GPIO_Read(x) (x.GPIOx->IDR & x.pinNum)
#include "string.h"
#include "stdlib.h"
#include "stm32f4xx.h"
#include "time_service.h"
#include "global_macros.h"
#include "stm32f4xx_global_macros.h"

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_dma2d.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx.h"
#include "misc.h"

#include "stm32f407_sysFunc.h"





#define DISPLAY_SPI	_SPI3
#define DISPLAY_CS	PA11
#define DISPLAY_SCK	PC10
#define DISPLAY_MOSI	PC12
#define DISPLAY_DC	PA15
#define DISPLAY_RESET	PA12
#define DISPLAY_PWR_EN	PB13




/*STATES*/
#define LINE 0
#define BUTTON1_ON 3
#define BUTTON2_ON 4
#define BUTTON3_ON 5
#define BUTTON4_ON 6
