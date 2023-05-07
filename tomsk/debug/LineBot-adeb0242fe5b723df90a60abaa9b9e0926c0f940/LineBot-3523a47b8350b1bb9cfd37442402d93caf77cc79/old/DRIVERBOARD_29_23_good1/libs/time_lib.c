#include "time_lib.h"

void delay(uint32_t time_delay)
{
        volatile uint32_t nCount;
        RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq (&RCC_Clocks);

        nCount=(RCC_Clocks.HCLK_Frequency/10000)*time_delay;


	for (; nCount!=0; nCount--);
}

