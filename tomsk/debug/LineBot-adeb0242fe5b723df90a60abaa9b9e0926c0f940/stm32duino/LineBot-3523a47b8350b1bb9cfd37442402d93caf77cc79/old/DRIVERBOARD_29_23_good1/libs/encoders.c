#include "encoders.h"

int32_t enc1 = 0;
int32_t enc2 = 0;
int32_t enc3 = 0;
int32_t enc4 = 0;


#ifdef __cplusplus
 extern "C" {
#endif

void EXTI9_5_IRQHandler(void)
{
	int signal1;
	if (EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
	EXTI_ClearFlag(EXTI_Line9);
	
	 signal1 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
	if (signal1)
	{
		enc1 = enc1 + 1;
	}
	else
	{
		enc1 = enc1 - 1;
	}
}
	

	
}
void EXTI15_10_IRQHandler(void)
{
	int signal2;
	int signal3;
	int signal4;
	if (EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
	EXTI_ClearFlag(EXTI_Line10);
	
	 signal2 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
	if (signal2)
	{
		enc2 = enc2 + 1;
	}
	else
	{
		enc2 = enc2 - 1;
	}
}
	if (EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
	EXTI_ClearFlag(EXTI_Line11);
	
	 signal3 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
	if (signal3)
	{
		enc3 = enc3 + 1;
	}
	else
	{
		enc3 = enc3 - 1;
	}
}
	if (EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
	EXTI_ClearFlag(EXTI_Line12);
	
	 signal4 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
	if (signal4)
	{
		enc4 = enc4 + 1;
	}
	else
	{
		enc4 = enc4 - 1;
	}
}
}

 #ifdef __cplusplus
}
#endif


void Init_Encoders()
{

    GPIO_InitTypeDef GPIO_Config;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;
	
	
    GPIO_Config.GPIO_Pin = GPIO_Pin_9;
    GPIO_Config.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_Config);
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_10;
    GPIO_Config.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_Config);
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_11;
    GPIO_Config.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_Config);
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_12;
    GPIO_Config.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_Config);
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource12);
	
    exti.EXTI_Line = EXTI_Line9;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
	exti.EXTI_Line = EXTI_Line10;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
	exti.EXTI_Line = EXTI_Line11;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
	exti.EXTI_Line = EXTI_Line12;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
	
	
	
    nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	
	 nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_5;   
    GPIO_Config.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_Config);
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_6;   
    GPIO_Config.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_Config);
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_7;   
    GPIO_Config.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_Config);
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_8;   
    GPIO_Config.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_Config);
	
    
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_15;
    GPIO_Config.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Config);
	
	
	
}

int Encoder1()
{
	return enc1;
}

int Encoder2()
{
	return enc2;
}

int Encoder3()
{
	return enc3;
}

int Encoder4()
{
	return enc4;
}

