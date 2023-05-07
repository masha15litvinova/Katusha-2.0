#include "pwm.h"


void initMotorPins()
{
	GPIO_InitTypeDef port;
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 GPIO_StructInit(&port);
	 port.GPIO_OType = GPIO_OType_PP;
	 port.GPIO_PuPd = GPIO_PuPd_NOPULL;
	 port.GPIO_Mode = GPIO_Mode_AF;
	 port.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_11 | GPIO_Pin_10;
	 port.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIO_Init(GPIOB, &port);
	
	port.GPIO_Pin = GPIO_Pin_3;
	 GPIO_Init(GPIOA, &port);
	
	 port.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	 GPIO_Init(GPIOE, &port);
		
	 GPIO_PinAFConfig(GPIOB, GPIO_PinSource0,GPIO_AF_TIM3);
	 GPIO_PinAFConfig(GPIOB, GPIO_PinSource1,GPIO_AF_TIM3);
	 GPIO_PinAFConfig(GPIOB, GPIO_PinSource4,GPIO_AF_TIM3);
	 GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10,GPIO_AF_TIM2);
	 GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_TIM2);
	 GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,GPIO_AF_TIM5);
	 GPIO_PinAFConfig(GPIOE, GPIO_PinSource5,GPIO_AF_TIM9);
	 GPIO_PinAFConfig(GPIOE, GPIO_PinSource6,GPIO_AF_TIM9);
	
	
	
	
}
void initPWM()
{
	TIM_OCInitTypeDef ch;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	 TIM_TimeBaseInitTypeDef tim;
	
		tim.TIM_Prescaler = 1;
		tim.TIM_CounterMode = TIM_CounterMode_Up;
		tim.TIM_Period = 4096;
		tim.TIM_ClockDivision = TIM_CKD_DIV1;
		tim.TIM_RepetitionCounter = 0; 
	
		TIM_TimeBaseInit(TIM3, &tim);
	TIM_TimeBaseInit(TIM2, &tim);
	TIM_TimeBaseInit(TIM9, &tim);
	TIM_TimeBaseInit(TIM5, &tim);
	
		ch.TIM_OCMode = TIM_OCMode_PWM1;
		ch.TIM_OutputState = TIM_OutputState_Enable;
		ch.TIM_OutputNState = TIM_OutputNState_Disable;
		ch.TIM_Pulse = 0;
		ch.TIM_OCPolarity = TIM_OCPolarity_High;
		ch.TIM_OCNPolarity = TIM_OCNPolarity_High;
		ch.TIM_OCIdleState = TIM_OCIdleState_Set;
		ch.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
		TIM_OC1Init(TIM3, &ch);
		TIM_OC2Init(TIM3, &ch);
		TIM_OC3Init(TIM3, &ch);
		TIM_OC4Init(TIM3, &ch);
		TIM_OC3Init(TIM2, &ch);
		TIM_OC4Init(TIM2, &ch);
		TIM_OC1Init(TIM9, &ch);
		TIM_OC2Init(TIM9, &ch);
		TIM_OC4Init(TIM5, &ch);
		TIM_Cmd(TIM3, ENABLE);
		TIM_Cmd(TIM9, ENABLE);
		TIM_Cmd(TIM2, ENABLE);
		TIM_Cmd(TIM5, ENABLE);
}

/*MOTORS PLATFORM*/
void setPWM1(int pwm)
{
		if(pwm>4096)
	{
		pwm=4096;
	}
	if(pwm<-4096)
	{
		pwm=-4096;
	}
	if(pwm>0)
	{
	TIM_SetCompare3(TIM3,pwm);
	TIM_SetCompare4(TIM3,0);
	}
	else
	{
		TIM_SetCompare4(TIM3,-pwm);
	TIM_SetCompare3(TIM3,0);
	}
}
void setPWM2(int pwm)
{
	if(pwm>4096)
	{
		pwm=4096;
	}
	if(pwm<-4096)
	{
		pwm=-4096;
	}
	if(pwm>0)
	{
	TIM_SetCompare1(TIM9,pwm);
	TIM_SetCompare2(TIM9,0);
	}
	else
	{
		TIM_SetCompare2(TIM9,-pwm);
	TIM_SetCompare1(TIM9,0);
	}
}
/*MOTORS PLATFORM / LIFTING*/
void setPWM3(int pwm)
{
		if(pwm>4096)
	{
		pwm=4096;
	}
	if(pwm<-4096)
	{
		pwm=-4096;
	}
	if(pwm>0)
	{
	TIM_SetCompare1(TIM3,pwm);
	TIM_SetCompare2(TIM3,0);
	}
	else
	{
		TIM_SetCompare2(TIM3,-pwm);
	TIM_SetCompare1(TIM3,0);
	}
}
void setPWM4(int pwm)
{
		if(pwm>4096)
	{
		pwm=4096;
	}
	if(pwm<-4096)
	{
		pwm=-4096;
	}
	if(pwm>0)
	{
	TIM_SetCompare3(TIM2,pwm);
	TIM_SetCompare4(TIM2,0);
	}
	else
	{
		TIM_SetCompare4(TIM2,-pwm);
	TIM_SetCompare3(TIM2,0);
	}
}
/*PWM FOR LIGHT SENSORS*/

void setPWMlight(int pwm)
{
		if(pwm>4096)
	{
		pwm=4096;
	}
	
	TIM_SetCompare4(TIM5,pwm);
	
	
}