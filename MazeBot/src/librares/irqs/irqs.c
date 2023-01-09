#include "irqs.h"

int64_t enc1 = 0;
int64_t enc2 = 0;

int signal1;

int button1 = 0;
int button2 = 0;
int button3 = 0;
int button4 = 0;

#ifdef __cplusplus
 extern "C" {
#endif

void EXTI15_10_IRQHandler(void)
{
	
	if (EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
	EXTI_ClearFlag(EXTI_Line14);
	
	 signal1 = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_3);
	if (signal1)
	{
		enc1 = enc1 + 1;
	}
	else
	{
		enc1 = enc1 - 1;
	}
}
	
if (EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
	EXTI_ClearFlag(EXTI_Line11);
		
		button1 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11);
		
	}
	if (EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
	EXTI_ClearFlag(EXTI_Line15);
		
		button2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
		
	}
	

}
void EXTI2_IRQHandler(void)
{
if (EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
	EXTI_ClearFlag(EXTI_Line2);
		
		button4 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
}
	}
void EXTI3_IRQHandler(void)
{
if (EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
	EXTI_ClearFlag(EXTI_Line3);
		
		button3 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
}
	}


 #ifdef __cplusplus
}
#endif

void InitEncoders()
{

    GPIO_InitTypeDef port;
		EXTI_InitTypeDef exti;
		NVIC_InitTypeDef nvic;
	
		
	
		port.GPIO_OType = GPIO_OType_PP;
		port.GPIO_PuPd = GPIO_PuPd_UP;
		port.GPIO_Mode = GPIO_Mode_IN;
		port.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;
		port.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOB, &port);
	
		port.GPIO_OType = GPIO_OType_PP;
		port.GPIO_PuPd = GPIO_PuPd_UP;
		port.GPIO_Mode = GPIO_Mode_IN;
		port.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		port.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOG, &port);
	
    
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, GPIO_PinSource12);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, GPIO_PinSource14);
	
		  
	
		
	

	
}
void initEndstops()
{
	GPIO_InitTypeDef port;
	
		
	
		
	
		port.GPIO_OType = GPIO_OType_PP;
		port.GPIO_PuPd = GPIO_PuPd_DOWN;
		port.GPIO_Mode = GPIO_Mode_IN;
		port.GPIO_Pin = GPIO_Pin_11;
		port.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOD, &port);
	
		port.GPIO_OType = GPIO_OType_PP;
		port.GPIO_PuPd = GPIO_PuPd_DOWN;
		port.GPIO_Mode = GPIO_Mode_IN;
		port.GPIO_Pin = GPIO_Pin_15;
		port.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOB, &port);
	
	port.GPIO_OType = GPIO_OType_PP;
		port.GPIO_PuPd = GPIO_PuPd_DOWN;
		port.GPIO_Mode = GPIO_Mode_IN;
		port.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
		port.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOC, &port);
    
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, GPIO_PinSource11);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, GPIO_PinSource15);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, GPIO_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, GPIO_PinSource2);
		 
	
		
		
   
}

void initNVIC()
{
	NVIC_InitTypeDef nvic;
	 nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	 nvic.NVIC_IRQChannel = EXTI2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	 nvic.NVIC_IRQChannel = EXTI3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}
void initEXTI()
{
		EXTI_InitTypeDef exti;
	  
		exti.EXTI_Line = EXTI_Line2;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
		exti.EXTI_Line = EXTI_Line3;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
		exti.EXTI_Line = EXTI_Line11;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
		exti.EXTI_Line = EXTI_Line12;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
		exti.EXTI_Line = EXTI_Line14;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
		exti.EXTI_Line = EXTI_Line15;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
}

int Encoder1()
{
	return enc1;
}

int Encoder2()
{
	return enc2;
}
int Button1()
{
	return button1;
}
int Button2()
{
	return button2;
}

int Button3()
{
	return button3;
}

int Button4()
{
	return button4;
}


void resetButton1()
{
	button1 = 0;
	
}
void resetButton2()
{
	button2 = 0;
	
}

void resetButton3()
{
	button3 = 0;
	
}

void resetButton4()
{
	button4 = 0;
	
}



