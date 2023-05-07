#pragma once
#include "project_config.h"


//#ifndef TCS34725_H
#define TCS34725_H



// TCS34725 I2C address
#define TCS34725_I2C_ADDRESS  0x29

// TCS34725 registers
#define TCS34725_REG_ENABLE   0x00
#define TCS34725_REG_ATIME    0x01
#define TCS34725_REG_WTIME    0x03
#define TCS34725_REG_AILTL    0x04
#define TCS34725_REG_AILTH    0x05
#define TCS34725_REG_AIHTL    0x06
#define TCS34725_REG_AIHTH    0x07
#define TCS34725_REG_PERS     0x0C
#define TCS34725_REG_CONFIG   0x0D
#define TCS34725_REG_CONTROL  0x0F
#define TCS34725_REG_ID       0x12
#define TCS34725_REG_STATUS   0x13
#define TCS34725_REG_CDATAL   0x14
#define TCS34725_REG_CDATAH   0x15
#define TCS34725_REG_RDATAL   0x16
#define TCS34725_REG_RDATAH   0x17
#define TCS34725_REG_GDATAL   0x18
#define TCS34725_REG_GDATAH   0x19
#define TCS34725_REG_BDATAL   0x1A
#define TCS34725_REG_BDATAH   0x1B

// TCS34725 enable register bits
#define TCS34725_ENABLE_AIEN  0x10
#define TCS34725_ENABLE_WEN   0x08
#define TCS34725_ENABLE_AEN   0x02
#define TCS34725_ENABLE_PON   0x01

// TCS34725 integration time constants
#define TCS34725_INTEGRATION_2_4MS  0xFF
#define TCS34725_INTEGRATION_24MS   0xF6
#define TCS34725_INTEGRATION_50MS   0xEB
#define TCS34725_INTEGRATION_101MS  0xD5
#define TCS34725_INTEGRATION_154MS  0xC0
#define TCS34725_INTEGRATION_700MS  0x00

// TCS34725 gain constants
#define TCS34725_GAIN_1X   0x00
#define TCS34725_GAIN_4X   0x01
#define TCS34725_GAIN_16X  0x02
#define TCS34725_GAIN_60X  0x03

// Function prototypes
void tcs34725_init(void);
void tcs34725_setIntegrationTime(uint8_t time);
void tcs34725_writeByte(uint8_t reg, uint8_t data);
void tcs34725_readBytes(uint8_t reg, uint8_t* data, uint8_t length);
void tcs34725_getData(uint16_t* r, uint16_t* g, uint16_t* b, uint16_t* c);

	
