#include "pwm.h"

#define MAX_PWM 255


void InitTimers()
{
	TIM_TimeBaseInitTypeDef TIM_BaseConfig;	
	TIM_OCInitTypeDef TIM_OCConfig;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_BaseConfig.TIM_Prescaler = (uint16_t) (SystemCoreClock / 4800000) - 1;
    TIM_BaseConfig.TIM_Period = 255;
    TIM_BaseConfig.TIM_ClockDivision = 0;
    TIM_BaseConfig.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_BaseConfig);
	TIM_TimeBaseInit(TIM3, &TIM_BaseConfig);

	TIM_BaseConfig.TIM_RepetitionCounter = 0; 
	
	TIM_TimeBaseInit(TIM1, &TIM_BaseConfig);
	
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM_BDTRInitStructure.TIM_DeadTime = 0;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
	
		
	TIM_OCConfig.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCConfig.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCConfig.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM3, &TIM_OCConfig);
	TIM_OC2Init(TIM2, &TIM_OCConfig);
	TIM_OC3Init(TIM2, &TIM_OCConfig);
	TIM_OC4Init(TIM2, &TIM_OCConfig);
	
	TIM_OCConfig.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCConfig.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCConfig.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
    TIM_OC1Init(TIM1, &TIM_OCConfig);
    TIM_OC2Init(TIM1, &TIM_OCConfig);
	TIM_OC3Init(TIM1, &TIM_OCConfig);
    TIM_OC4Init(TIM1, &TIM_OCConfig);
		
//	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
//    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
//	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
//    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
//	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
//    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
//	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
//    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
    /*TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM3, ENABLE);*/
	
	
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
//	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	//TIM_CtrlPWMOutputs(TIM3, ENABLE);
}



void InitAllMotors()
{
	GPIO_InitTypeDef GPIO_Config;

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_6;    //каналы таймера мотора
    GPIO_Config.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_Init(GPIOA, &GPIO_Config);
	
	
    GPIO_Config.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_4;   //пины драйвера
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Config);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);    //enable драйвера
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);   //inv драйвера
	
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;    //каналы таймера мотора
    GPIO_Config.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_Init(GPIOA, &GPIO_Config);
	
	
    GPIO_Config.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_5;   //пины драйвера
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Config);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);    //enable драйвера
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);   //inv драйвера
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;    //каналы таймера мотора
    GPIO_Config.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_Init(GPIOA, &GPIO_Config);
	
	
    GPIO_Config.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_8;   //пины драйвера
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Config);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);    //enable драйвера
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET);   //inv драйвера
	
	GPIO_Config.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;    //каналы таймера мотора
    GPIO_Config.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
	
    
	GPIO_Init(GPIOA, &GPIO_Config);
	
	
    GPIO_Config.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_9;   //пины драйвера
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Config);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET);    //enable драйвера
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET);   //inv драйвера
}

void SetPWMMotor1(int16_t pwm)
{
	TIM_OCInitTypeDef TIM_OCConfig;
    if (pwm > 255)
	{
		pwm = 255;
	}
	if (pwm < -255)
	{
		pwm = -255;
	}
	
	
	
	if (pwm > 0)
	{
   
		TIM_SetCompare1(TIM3, 255);
		TIM_SetCompare2(TIM2, 255-pwm);
	}
	else
	{
    TIM_SetCompare1(TIM3, 255+pwm);
		TIM_SetCompare2(TIM2, 255);
    
	}
   

   

	}

void SetPWMMotor2(int16_t pwm)
{
    
	if (pwm > 255)
	{
		pwm = 255;
	}
	if (pwm < -255)
	{
		pwm = -255;
	}
	
	
	
	
	
	if (pwm > 0)
	{
    TIM_SetCompare3(TIM2, 255-pwm);
		TIM_SetCompare4(TIM2, 255);
	}
	else
	{
    TIM_SetCompare3(TIM2, 255);
	TIM_SetCompare4(TIM2, 255+pwm);
	}
    

   
}
void SetPWMMotor3(int16_t pwm)
{
	
    if (pwm > 255)
	{
		pwm = 255;
	}
	if (pwm < -255)
	{
		pwm = -255;
	}
	

	
	
	if (pwm > 0)
	{
    TIM_SetCompare1(TIM1, 255-pwm);
		TIM_SetCompare2(TIM1, 255);
	}
	else
	{
		TIM_SetCompare1(TIM1, 255);
		TIM_SetCompare2(TIM1, 255+pwm);
    
	}
    
	
   
}
void SetPWMMotor4(int16_t pwm)
{
	
	if (pwm > 255)
	{
		pwm = 255;
	}
	if (pwm < -255)
	{
		pwm = -255;
	}
	
	

	
	
	
	if (pwm > 0)
	{
    TIM_SetCompare3(TIM1, 255-pwm);
	TIM_SetCompare4(TIM1, 255);

	}
	else
	{
    TIM_SetCompare3(TIM1, 255);
	TIM_SetCompare4(TIM1, 255+pwm);
	}
    
	
   
}

void SetPWM(int16_t pwm1, int16_t pwm2, int16_t pwm3, int16_t pwm4)
{
	SetPWMMotor1(pwm1);
	SetPWMMotor2(pwm2);
	SetPWMMotor3(pwm3);
	SetPWMMotor4(pwm4);
}

