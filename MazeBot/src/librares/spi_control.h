#pragma once
#include "project_config.h"

namespace SPI_control
{
    static void init()
    {
        GPIO_InitTypeDef port;
        SPI_InitTypeDef spi;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
     
        SPI_StructInit(&spi);
        spi.SPI_Direction = SPI_Direction_1Line_Tx;
        spi.SPI_Mode = SPI_Mode_Master;
        spi.SPI_DataSize = SPI_DataSize_8b;
        spi.SPI_CPOL = SPI_CPOL_Low;
        spi.SPI_CPHA = SPI_CPHA_1Edge;
        spi.SPI_NSS = SPI_NSS_Soft;
        spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
        spi.SPI_FirstBit = SPI_FirstBit_MSB;
        spi.SPI_CRCPolynomial = 7;
        SPI_Init(SPI3, &spi);
        
        SPI_Cmd(SPI3,ENABLE);
        
        SPI_NSSInternalSoftwareConfig(SPI3, SPI_NSSInternalSoft_Set);
        
        GPIO_StructInit(&port);
        port.GPIO_OType = GPIO_OType_PP;
        port.GPIO_PuPd = GPIO_PuPd_NOPULL;
        port.GPIO_Mode = GPIO_Mode_AF;
        port.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_10;
        port.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &port);
        
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource12,GPIO_AF_SPI3);
        //GPIO_PinAFConfig(GPIOC, GPIO_PinSource11,GPIO_AF_SPI3);
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource10,GPIO_AF_SPI3); 
    }
    STRONG_ENUM(reg,
    ENCODER_VALUE = 0x00,
    LEFT_SIDE_VALUE = 0x01,
    RIGHT_SIDE_VALUE = 0x02,
    LIGHT_SENSOR_VALUE = 0x03);
    static uint8_t reg_use;
    static int32_t data[4];
    static uint8_t flagTR;
    static void Change_Value(reg REG, int32_t Value)
    {
        data[REG.toInt()] = Value;
    }
    static int32_t Get_reg(reg REG)
    {
        return data[REG.toInt()];
    }
    static bool isOutput(uint8_t REG)
    {
        return ((REG) == reg::ENCODER_VALUE
            || (REG) == reg::LIGHT_SENSOR_VALUE);
    }
    static void read()
    {
        if (SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==SET) 
        {
            if(flagTR == 0)
            {
                uint16_t dt = SPI3->DR;
                if (dt == 0xA5)
                    flagTR = 1;
            }
            if (flagTR == 1)
            {
                reg_use = SPI3->DR; //Читаем то что пришло
                if(isOutput(reg_use))
                {
                    SPI3->DR = data[reg_use];
                    flagTR = 0;
                }
                else
                    flagTR = 2;
            }
            if (flagTR == 2)
            {
                data[reg_use] = SPI3->DR;
                flagTR = 0;
            }
        }
    }
    extern "C"
    {
        void SPI3_IRQHandler (void) 
        {
            read();
        }
    };
}
