#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "encoders.h"
#include "pwm.h"
#include "time_lib.h"
#include "speed_control.h"
#include "uart.h"

void InitRCC(void);

void LEDInit(void);
void ButtonInit(void);

long int enc_1 = 0;
long int enc_2 = 0;
long int enc_3 = 0;
long int enc_4 = 0;

int v = 75;

int speed_motor1 = 0;
int speed_motor2 = 0;
int speed_motor3 = 0;
int speed_motor4 = 0;

int main(void)
{
	__enable_irq ();
	InitRCC();
    Init_Encoders();

	InitTimers();
	InitAllMotors();
	uart_init();
	LEDInit();
	
	
	
    while(1)
    {
		
		
		
		
		
		
		
		
		speed_motor1 = motor1();
		speed_motor2 = motor2();
		speed_motor3 = motor3();
		speed_motor4 = motor4();
		/*speed_motor1 = 40;
		speed_motor2 = 40;
		speed_motor3 = 40;
		speed_motor4 = 40;*/
		
		SetPWMMotor3(vel3(50, enc_3));
		
		//SetPWM(vel1(200, enc_1), vel2(200, enc_2), vel3(0, enc_3), vel4(0, enc_4));
		//SetPWM(vel1(speed_motor1, enc_1), vel2(speed_motor2, enc_2),vel3(speed_motor3, enc_3),vel4(speed_motor4, enc_4));
		//SetPWMMotor2(100);
		//SetPWM(-100, 100, 100, -100);
		//SetPWMMotor3(vel1(100, enc_3));
		
		//SetPWM(vel1(100, enc_1), vel2(100, enc_2),vel3(100, enc_3),vel4(100, enc_4));
		enc_1 = Encoder1();
		enc_2 = Encoder2();
		enc_3 = Encoder3();
		enc_4 = Encoder4();	
		delay(5);
		
		
		
		
    }
    return 0;
}


void InitRCC()
{
	// Подаем тактовую частоту на таймеры и порт A
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //Разрешаем тактирование
    
	
}



void LEDInit()
{
	GPIO_InitTypeDef GPIO_Config;
    
	GPIO_Config.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_15;    
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;  
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_Init(GPIOA, &GPIO_Config);
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
	
}




 

  

