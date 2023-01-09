#include "uart.h"

int UART3data = 0;
int UART6data = 0;

#ifdef __cplusplus
 extern "C" {
#endif
void InitUART6()
{
		GPIO_InitTypeDef port;
		NVIC_InitTypeDef nvic;
		
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
		GPIO_StructInit(&port);
		port.GPIO_OType = GPIO_OType_PP;
		port.GPIO_PuPd = GPIO_PuPd_NOPULL;
		port.GPIO_Mode = GPIO_Mode_AF;
		port.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
		port.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOG, &port);
		
		GPIO_PinAFConfig(GPIOG, GPIO_PinSource9,GPIO_AF_USART6);
		GPIO_PinAFConfig(GPIOG, GPIO_PinSource14,GPIO_AF_USART6);
	
    USART_InitTypeDef uart;
    uart.USART_BaudRate = 9600;
    uart.USART_WordLength = USART_WordLength_8b;
    uart.USART_StopBits = USART_StopBits_1;
    uart.USART_Parity = USART_Parity_No;
    uart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART6, &uart);
        
    
		
		
		
   
		/*nvic.NVIC_IRQChannel = EXTI3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);*/
		
		USART_ITConfig(USART6, USART_IT_TC, ENABLE);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART6, ENABLE);
		 NVIC_SetPriority(USART6_IRQn, 2);
    NVIC_EnableIRQ(USART6_IRQn);	
		
		
		
}
void InitUART3()
{
		GPIO_InitTypeDef port;
		NVIC_InitTypeDef nvic;
		
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
		GPIO_StructInit(&port);
		port.GPIO_OType = GPIO_OType_PP;
		port.GPIO_PuPd = GPIO_PuPd_NOPULL;
		port.GPIO_Mode = GPIO_Mode_AF;
		port.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
		port.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOD, &port);
		
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,GPIO_AF_USART3);
	
    USART_InitTypeDef uart;
    uart.USART_BaudRate = 9600;
    uart.USART_WordLength = USART_WordLength_8b;
    uart.USART_StopBits = USART_StopBits_1;
    uart.USART_Parity = USART_Parity_No;
    uart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &uart);
        
    USART_ITConfig(USART3, USART_IT_TC, ENABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		
		
		
		
    //NVIC_SetPriority(USART3_IRQn, 8);
		nvic.NVIC_IRQChannel = EXTI3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    NVIC_EnableIRQ(USART3_IRQn);	
    
		USART_Cmd(USART3, ENABLE);
		
}

int GetUART6data()
{
	return UART6data;
}
int GetUART3data()
{
	return UART3data;
}


void USART6_IRQHandler(void)
	{
      if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
				USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        UART6data = USART_ReceiveData(USART6);
		}
		/*if (USART_GetITStatus(USART6, USART_SR_TC) != RESET)
		{
				USART_ClearITPendingBit(USART6, USART_SR_TC);
        USART_SendData(USART6, 10);
		}*/
	}
void USART3_IRQHandler(void)
	{
       if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
				USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        UART3data = USART_ReceiveData(USART3);
		}
		/*if (USART_GetITStatus(USART3, USART_SR_TC) != RESET)
		{
				USART_ClearITPendingBit(USART3, USART_SR_TC);
        USART_SendData(USART3, 10);
		}*/
	}		

 #ifdef __cplusplus
}
#endif