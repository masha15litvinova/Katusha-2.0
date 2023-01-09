#include "uart.h"

int UART6data = 0;

#ifdef __cplusplus
 extern "C" {
#endif
void InitUART6()
{
		GPIO_InitTypeDef port;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
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
        
    USART_ITConfig(USART6, USART_IT_TC, ENABLE);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
        
    USART_Cmd(USART6, ENABLE);
    NVIC_SetPriority(USART6_IRQn, 3);
    NVIC_EnableIRQ(USART6_IRQn);
}

int GetUART6data()
{
	return UART6data;
}


	void USART6_IRQHandler(void)
	{
      if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
		
				USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        UART6data = USART_ReceiveData(USART6);
					
		}
	}  

 #ifdef __cplusplus
}
#endif