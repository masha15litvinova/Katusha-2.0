#include "uart.h"


int usartData = 0;
int convertedUsartData = 0;
int count = 0;
uint16_t byte_motor = 255;  //сигнал моторы

int motherboard_data[] = {0, 0, 0, 0, 0};
int motors[] = {0, 0, 0, 0};


void uart_init()
{
	GPIO_InitTypeDef GPIO_InitStructure; //Структура содержащая настройки порта
	USART_InitTypeDef USART_InitStructure;//Структура содержащая настройки USART
  NVIC_InitTypeDef NVIC_InitStructure;
  
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //rx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE); 
 
  USART_StructInit(&USART_InitStructure); //Инициализируем UART с дефолтными настройками: скорость 9600, 8 бит данных, 1 стоп бит
 
 USART_InitStructure.USART_BaudRate = 9600; //Скорость обмена 9600 бод
/*USART_InitStructure.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
USART_InitStructure.USART_StopBits = USART_StopBits_1; //1 стоп-бит
USART_InitStructure.USART_Parity = USART_Parity_No ; //Без проверки четности
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля*/
USART_InitStructure.USART_Mode = USART_Mode_Rx; //Включен передатчик и приемник USART2
  
  USART_Init(USART1, &USART_InitStructure);
  
  


  // Configure the NVIC Preemption Priority Bits  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
   //Enable the USARTy Interrupt 
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_EnableIRQ(USART1_IRQn); //Включаем прерывания от UART
	(USART1_IRQn, 0); //Прерывание от UART, приоритет 0, самый высокий
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  
  USART_Cmd(USART1, ENABLE); //Включаем UART
  
}

int Usart_Data()
{
	return usartData;
}

int map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
	int ret = 0;
	if (x > in_max)
	{
		x = in_max;
	}
	else if (x < in_min)
	{
		x = in_min;
	}
	ret = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	if (ret > out_max)
	{
		ret = out_max;
	}
	else if (ret < out_min)
	{
		ret = out_min;
	}
  return ret;
}

int motor1()
{
	return motors[0];
}
int motor2()
{
	return motors[1];
}
int motor3()
{
	return motors[2];
}
int motor4()
{
	return motors[3];
}

#ifdef __cplusplus
 extern "C" {
#endif


void USART1_IRQHandler()
{
   
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
		
		
		 USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        usartData = USART_ReceiveData(USART1);
		
		motherboard_data[count] = usartData;
		count++;
		if (count == 5)
		{
			count = 0;
			if (motherboard_data[4] == byte_motor)
			{
				motors[0] = map(motherboard_data[0], 0, 254, -255, 255);
				motors[1] = map(motherboard_data[1], 0, 254, -255, 255);
				motors[2] = map(motherboard_data[2], 0, 254, -255, 255);
				motors[3] = map(motherboard_data[3], 0, 254, -255, 255);
			}
			else if (motherboard_data[3] == byte_motor)
			{
				motors[0] = map(motherboard_data[4], 0, 254, -255, 255);
				motors[1] = map(motherboard_data[0], 0, 254, -255, 255);
				motors[2] = map(motherboard_data[1], 0, 254, -255, 255);
				motors[3] = map(motherboard_data[2], 0, 254, -255, 255);
			}
			else if (motherboard_data[2] == byte_motor)
			{
				motors[0] = map(motherboard_data[3], 0, 254, -255, 255);
				motors[1] = map(motherboard_data[4], 0, 254, -255, 255);
				motors[2] = map(motherboard_data[0], 0, 254, -255, 255);
				motors[3] = map(motherboard_data[1], 0, 254, -255, 255);
			}
			else if (motherboard_data[1] == byte_motor)
			{
				motors[0] = map(motherboard_data[2], 0, 254, -255, 255);
				motors[1] = map(motherboard_data[3], 0, 254, -255, 255);
				motors[2] = map(motherboard_data[4], 0, 254, -255, 255);
				motors[3] = map(motherboard_data[0], 0, 254, -255, 255);
			}
			else if (motherboard_data[0] == byte_motor)
			{
				motors[0] = map(motherboard_data[1], 0, 254, -255, 255);
				motors[1] = map(motherboard_data[2], 0, 254, -255, 255);
				motors[2] = map(motherboard_data[3], 0, 254, -255, 255);
				motors[3] = map(motherboard_data[4], 0, 254, -255, 255);
			}
			
		}
	}
 }

 #ifdef __cplusplus
}
#endif
