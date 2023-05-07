#pragma once
#include "pins.h"

namespace vlxPins
{
    static pins::pinStructure sclPinVL = 
    {
        GPIO_Pin_7,
        GPIOB,
        GPIO_Mode_OUT,
        GPIO_PuPd_UP,
        GPIO_OType_OD,
        TIM4,
        TIM_Channel_2,
        GPIO_PinSource7,
        GPIO_AF_I2C2
    };
    static pins::pinStructure sdaPinVL = 
    {   
        GPIO_Pin_6,
        GPIOB,
        GPIO_Mode_OUT,
        GPIO_PuPd_UP,
        GPIO_OType_OD,
        TIM4,
        TIM_Channel_1,
        GPIO_PinSource6,
        GPIO_AF_I2C2
    };
    static pins::pinStructure xshutFr =
    {
        GPIO_Pin_11,
        GPIOG,
        GPIO_Mode_OUT,
        GPIO_PuPd_DOWN,
        GPIO_OType_PP,
        TIM1,
        TIM_Channel_3,
        GPIO_PinSource11,
        GPIO_AF_I2C2
    };
    static pins::pinStructure enFr =
    {
        GPIO_Pin_10,
        GPIOA,
        GPIO_Mode_OUT,
        GPIO_PuPd_DOWN,
        GPIO_OType_PP,
        TIM1,
        TIM_Channel_3,
        GPIO_PinSource10,
        GPIO_AF_I2C2
    };
    static pins::pinStructure xshutRiFr =
    {
        GPIO_Pin_12,
        GPIOG,
        GPIO_Mode_OUT,
        GPIO_PuPd_DOWN,
        GPIO_OType_PP,
        TIM1,
        TIM_Channel_3,
        GPIO_PinSource12,
        GPIO_AF_I2C2
    };
    static pins::pinStructure enRiFr =
    {
        GPIO_Pin_2,
        GPIOE,
        GPIO_Mode_OUT,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM1,
        TIM_Channel_3,
        GPIO_PinSource2,
        GPIO_AF_I2C2
    };
    static pins::pinStructure xshutLeFr =
    {
        GPIO_Pin_10,
        GPIOG,
        GPIO_Mode_OUT,
        GPIO_PuPd_DOWN,
        GPIO_OType_PP,
        TIM1,
        TIM_Channel_3,
        GPIO_PinSource10,
        GPIO_AF_I2C2
    };
    static pins::pinStructure enLeFr =
    {
        GPIO_Pin_9,
        GPIOA,
        GPIO_Mode_OUT,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM1,
        TIM_Channel_3,
        GPIO_PinSource9,
        GPIO_AF_I2C2
    };
    
    static pins::pinStructure xshutBo =
    {
        GPIO_Pin_13,
        GPIOG,
        GPIO_Mode_OUT,
        GPIO_PuPd_DOWN,
        GPIO_OType_PP,
        TIM1,
        TIM_Channel_3,
        GPIO_PinSource13,
        GPIO_AF_I2C2
    };
    static pins::pinStructure enBo =
    {
        GPIO_Pin_3,
        GPIOE,
        GPIO_Mode_OUT,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM1,
        TIM_Channel_3,
        GPIO_PinSource3,
        GPIO_AF_I2C2
    };
}

