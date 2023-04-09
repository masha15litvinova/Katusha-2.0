#pragma once
#include "pins.h"
namespace otherPins
{
    static pins::pinStructure button1 =
    {
        GPIO_Pin_1,
        GPIOE,
        GPIO_Mode_IN,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM12,
        TIM_Channel_2,
        GPIO_PinSource15,
        GPIO_AF_TIM12
    };
    static pins::pinStructure button2 =
    {
        GPIO_Pin_2,
        GPIOE,
        GPIO_Mode_IN,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM12,
        TIM_Channel_2,
        GPIO_PinSource15,
        GPIO_AF_TIM12
    };
    static pins::pinStructure errorLed =
    {
        GPIO_Pin_14,
        GPIOF,
        GPIO_Mode_OUT,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM12,
        TIM_Channel_2,
        GPIO_PinSource15,
        GPIO_AF_TIM12
    };
    static pins::pinStructure workLed =
    {
        GPIO_Pin_13,
        GPIOF,
        GPIO_Mode_OUT,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM12,
        TIM_Channel_2,
        GPIO_PinSource15,
        GPIO_AF_TIM12
    };
    static pins::pinStructure victimLed =
    {
        GPIO_Pin_0,
        GPIOE,
        GPIO_Mode_OUT,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM12,
        TIM_Channel_2,
        GPIO_PinSource15,
        GPIO_AF_TIM12
    };
    static pins::pinStructure servPin = 
    {
        GPIO_Pin_15,
        GPIOB,
        GPIO_Mode_AF,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM12,
        TIM_Channel_2,
        GPIO_PinSource15,
        GPIO_AF_TIM12
    };
    static pins::pinStructure LeftSensEn = 
    {
        GPIO_Pin_11,
        GPIOG,
        GPIO_Mode_OUT,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_4,
        GPIO_PinSource11,
        GPIO_AF_I2C2
    };
    static pins::pinStructure RightSensEn = 
    {
        GPIO_Pin_12,
        GPIOG,
        GPIO_Mode_OUT,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_4,
        GPIO_PinSource12,
        GPIO_AF_I2C2
    };
    static pins::pinStructure lightSesor = 
    {   
        GPIO_Pin_10,
        GPIOF,
        GPIO_Mode_AIN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM8,
        TIM_Channel_1,
        GPIO_PinSource10,
        GPIO_AF_TIM8
    };
    static pins::pinStructure SHARPPin = 
    {   
        GPIO_Pin_0,
        GPIOC,
        GPIO_Mode_AIN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM8,
        TIM_Channel_1,
        GPIO_PinSource1,
        GPIO_AF_TIM8
    };
};
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
namespace mlxPins
{
    static pins::pinStructure sdaPinML = 
    {
        GPIO_Pin_8,
        GPIOA,
        GPIO_Mode_OUT,
        GPIO_PuPd_UP,
        GPIO_OType_OD,
        TIM2,
        TIM_Channel_3,
        GPIO_PinSource8,
        GPIO_AF_I2C2
    };
    static pins::pinStructure sclPinML = 
    {   
        GPIO_Pin_9,
        GPIOC,
        GPIO_Mode_OUT,
        GPIO_PuPd_UP,
        GPIO_OType_OD,
        TIM2,
        TIM_Channel_4,
        GPIO_PinSource9,
        GPIO_AF_I2C2
    };
};
namespace FrLeMotPins
{
    static pins::pinStructure out2 = 
    {   
        GPIO_Pin_6,
        GPIOC,
        GPIO_Mode_AF,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM8,
        TIM_Channel_1,
        GPIO_PinSource6,
        GPIO_AF_TIM8
    };
    static pins::pinStructure out1 = 
    {   
        GPIO_Pin_7,
        GPIOC,
        GPIO_Mode_AF,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM8,
        TIM_Channel_2,
        GPIO_PinSource7,
        GPIO_AF_TIM8
    };
    static pins::pinStructure en = 
    {   
        GPIO_Pin_3,
        GPIOD,
        GPIO_Mode_OUT,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_4,
        GPIO_PinSource0,
        GPIO_AF_I2C2
    };
    static pins::pinStructure inv = 
    {   
        GPIO_Pin_8,
        GPIOD,
        GPIO_Mode_OUT,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_4,
        GPIO_PinSource0,
        GPIO_AF_I2C2
    };
    static pins::pinStructure encPin1 = 
    {   
        GPIO_Pin_12,
        GPIOD,
        GPIO_Mode_AF,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM4,
        TIM_Channel_1,
        GPIO_PinSource12,
        GPIO_AF_TIM4
    };
    static pins::pinStructure encPin2 = 
    {   
        GPIO_Pin_13,
        GPIOD,
        GPIO_Mode_AF,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM4,
        TIM_Channel_2,
        GPIO_PinSource13,
        GPIO_AF_TIM4
    };
    static pins::pinStructure posSens = 
    {   
        GPIO_Pin_1,
        GPIOB,
        GPIO_Mode_IN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM3,
        TIM_Channel_4,
        GPIO_PinSource1,
        GPIO_AF_TIM3
    };
    static pins::pinStructure current = 
    {   
        GPIO_Pin_3,
        GPIOF,
        GPIO_Mode_AIN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM8,
        TIM_Channel_1,
        GPIO_PinSource6,
        GPIO_AF_TIM8
    };
};
namespace BoRiMotPins
{
    static pins::pinStructure out2 = 
    {   
        GPIO_Pin_9,
        GPIOF,
        GPIO_Mode_AF,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM14,
        TIM_Channel_1,
        GPIO_PinSource9,
        GPIO_AF_TIM14
    };
    static pins::pinStructure out1 = 
    {   
        GPIO_Pin_8,
        GPIOF,
        GPIO_Mode_AF,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM13,
        TIM_Channel_1,
        GPIO_PinSource8,
        GPIO_AF_TIM13
    };
    static pins::pinStructure en = 
    {   
        GPIO_Pin_2,
        GPIOD,
        GPIO_Mode_OUT,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_4,
        GPIO_PinSource0,
        GPIO_AF_I2C2
    };
    static pins::pinStructure inv = 
    {   
        GPIO_Pin_7,
        GPIOD,
        GPIO_Mode_OUT,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_4,
        GPIO_PinSource0,
        GPIO_AF_I2C2
    };
    static pins::pinStructure encPin1 = 
    {   
        GPIO_Pin_6,
        GPIOA,
        GPIO_Mode_AF,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM3,
        TIM_Channel_1,
        GPIO_PinSource6,
        GPIO_AF_TIM3
    };
    static pins::pinStructure encPin2 = 
    {   
        GPIO_Pin_7,
        GPIOA,
        GPIO_Mode_AF,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM3,
        TIM_Channel_2,
        GPIO_PinSource7,
        GPIO_AF_TIM3
    };
    static pins::pinStructure posSens = 
    {   
        GPIO_Pin_0,
        GPIOB,
        GPIO_Mode_IN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM3,
        TIM_Channel_3,
        GPIO_PinSource0,
        GPIO_AF_TIM3
    };
    static pins::pinStructure current = 
    {   
        GPIO_Pin_6,
        GPIOF,
        GPIO_Mode_AIN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM8,
        TIM_Channel_1,
        GPIO_PinSource6,
        GPIO_AF_TIM8
    };
};
namespace BoLeMotPins
{
    static pins::pinStructure out2 = 
    {   
        GPIO_Pin_5,
        GPIOE,
        GPIO_Mode_AF,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM9,
        TIM_Channel_1,
        GPIO_PinSource5,
        GPIO_AF_TIM9
    };
    static pins::pinStructure out1 = 
    {   
        GPIO_Pin_6,
        GPIOE,
        GPIO_Mode_AF,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM9,
        TIM_Channel_2,
        GPIO_PinSource6,
        GPIO_AF_TIM9
    };
    static pins::pinStructure en = 
    {   
        GPIO_Pin_1,
        GPIOD,
        GPIO_Mode_OUT,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_4,
        GPIO_PinSource0,
        GPIO_AF_I2C2
    };
    static pins::pinStructure inv = 
    {   
        GPIO_Pin_5,
        GPIOD,
        GPIO_Mode_OUT,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_4,
        GPIO_PinSource0,
        GPIO_AF_I2C2
    };
    static pins::pinStructure encPin1 = 
    {   
        GPIO_Pin_0,
        GPIOA,
        GPIO_Mode_AF,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_1,
        GPIO_PinSource0,
        GPIO_AF_TIM5
    };
    static pins::pinStructure encPin2 = 
    {   
        GPIO_Pin_1,
        GPIOA,
        GPIO_Mode_AF,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_2,
        GPIO_PinSource1,
        GPIO_AF_TIM5
    };
    static pins::pinStructure posSens = 
    {   
        GPIO_Pin_8,
        GPIOB,
        GPIO_Mode_IN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM10,
        TIM_Channel_1,
        GPIO_PinSource8,
        GPIO_AF_TIM10
    };
    static pins::pinStructure current = 
    {   
        GPIO_Pin_4,
        GPIOF,
        GPIO_Mode_AIN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM8,
        TIM_Channel_1,
        GPIO_PinSource6,
        GPIO_AF_TIM8
    };
};
namespace FrRiMotPins
{
    static pins::pinStructure out1 = 
    {   
        GPIO_Pin_2,
        GPIOA,
        GPIO_Mode_AF,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM2,
        TIM_Channel_3,
        GPIO_PinSource2,
        GPIO_AF_TIM2
    };
    static pins::pinStructure out2 = 
    {   
        GPIO_Pin_3,
        GPIOA,
        GPIO_Mode_AF,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM2,
        TIM_Channel_4,
        GPIO_PinSource3,
        GPIO_AF_TIM2
    };
    static pins::pinStructure en = 
    {   
        GPIO_Pin_0,
        GPIOD,
        GPIO_Mode_OUT,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_4,
        GPIO_PinSource0,
        GPIO_AF_I2C2
    };
    static pins::pinStructure inv = 
    {   
        GPIO_Pin_4,
        GPIOD,
        GPIO_Mode_OUT,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM5,
        TIM_Channel_4,
        GPIO_PinSource0,
        GPIO_AF_I2C2
    };
    static pins::pinStructure encPin1 = 
    {   
        GPIO_Pin_9,
        GPIOE,
        GPIO_Mode_AF,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM1,
        TIM_Channel_1,
        GPIO_PinSource9,
        GPIO_AF_TIM1
    };
    static pins::pinStructure encPin2 = 
    {   
        GPIO_Pin_11,
        GPIOE,
        GPIO_Mode_AF,
        GPIO_PuPd_UP,
        GPIO_OType_PP,
        TIM1,
        TIM_Channel_2,
        GPIO_PinSource11,
        GPIO_AF_TIM1
    };
    static pins::pinStructure posSens = 
    {   
        GPIO_Pin_7,
        GPIOF,
        GPIO_Mode_IN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM11,
        TIM_Channel_1,
        GPIO_PinSource7,
        GPIO_AF_TIM11
    };
    static pins::pinStructure current = 
    {   
        GPIO_Pin_5,
        GPIOF,
        GPIO_Mode_AIN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM8,
        TIM_Channel_1,
        GPIO_PinSource6,
        GPIO_AF_TIM8
    };
};
namespace cameraPins
{
    static pins::pinStructure pinL1 = 
    {
        GPIO_Pin_12,
        GPIOE,
        GPIO_Mode_IN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM2,
        TIM_Channel_4,
        GPIO_PinSource11,
        GPIO_AF_I2C2
    };
    static pins::pinStructure pinL2 = 
    {
        GPIO_Pin_13,
        GPIOE,
        GPIO_Mode_IN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM2,
        TIM_Channel_4,
        GPIO_PinSource11,
        GPIO_AF_I2C2
    };
    static pins::pinStructure pinR1 = 
    {
        GPIO_Pin_14,
        GPIOE,
        GPIO_Mode_IN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM2,
        TIM_Channel_4,
        GPIO_PinSource11,
        GPIO_AF_I2C2
    };
    static pins::pinStructure pinR2 = 
    {
        GPIO_Pin_15,
        GPIOE,
        GPIO_Mode_IN,
        GPIO_PuPd_NOPULL,
        GPIO_OType_PP,
        TIM2,
        TIM_Channel_4,
        GPIO_PinSource11,
        GPIO_AF_I2C2
    };
};
