#include "tcs34725.h"

void tcs34725_init(void)
{
		GPIO_InitTypeDef port;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
  // Enable the TCS34725 by writing to the ENABLE register
  uint8_t enable = TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN;
  tcs34725_writeByte(TCS34725_REG_ENABLE, enable);
  
  // Set the integration time to 2.4ms
  tcs34725_setIntegrationTime(TCS34725_INTEGRATION_2_4MS);
  
  // Set the gain to 1x
  tcs34725_writeByte(TCS34725_REG_CONTROL, TCS34725_GAIN_1X);
	

	

	
	 GPIO_StructInit(&port);
	 port.GPIO_OType = GPIO_OType_PP;
	 port.GPIO_PuPd = GPIO_PuPd_UP;
	 port.GPIO_Mode = GPIO_Mode_AF;
	 port.GPIO_Pin = GPIO_Pin_8;
	 port.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIO_Init(GPIOA, &port);
		
	 port.GPIO_Pin = GPIO_Pin_9;
	 GPIO_Init(GPIOC, &port);
	 
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,GPIO_AF_I2C3);
		 GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,GPIO_AF_I2C3);
		 
		 
	I2C_InitTypeDef I2C_InitStruct;
  I2C_InitStruct.I2C_ClockSpeed = 100000;
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStruct.I2C_OwnAddress1 = 0x00;
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(I2C3, &I2C_InitStruct);
  I2C_Cmd(I2C3, ENABLE);
	
	
	
	
}

void tcs34725_writeByte(uint8_t reg, uint8_t data)
{
  // Set up I2C data structure
  I2C_TypeDef* I2Cx = I2C3;
  uint8_t addr = TCS34725_I2C_ADDRESS;
  uint8_t d[2] = {reg, data};
  uint8_t bytesToWrite = 2;

  // Wait until I2C is not busy
  while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
  
  // Send start condition
  I2C_GenerateSTART(I2Cx, ENABLE);
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
  // Send device address
  I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter);
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  // Send data
  for (int i = 0; i < bytesToWrite; i++)
  {
    I2C_SendData(I2Cx, d[i]);
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }
  
  // Send stop condition
  I2C_GenerateSTOP(I2Cx, ENABLE);
}

void tcs34725_getData(uint16_t* r, uint16_t* g, uint16_t* b, uint16_t* c)
{
  // Read color data registers
  uint8_t data[8];
  tcs34725_readBytes(TCS34725_REG_CDATAL, data, 8);
  
  // Combine high and low bytes to get 16-bit values
  *c = (data[1] << 8) | data[0];
  *r = (data[3] << 8) | data[2];
  *g = (data[5] << 8) | data[4];
  *b = (data[7] << 8) | data[6];
}
void tcs34725_readBytes(uint8_t reg, uint8_t* data, uint8_t length)
{
  // Set up I2C data structure
  I2C_TypeDef* I2Cx = I2C3;
  uint8_t addr = TCS34725_I2C_ADDRESS;

  // Wait until I2C is not busy
  while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
  
  // Send start condition
  I2C_GenerateSTART(I2Cx, ENABLE);
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
  // Send device address
  I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter);
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  // Send register address
  I2C_SendData(I2Cx, reg);
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  // Send repeated start condition
  I2C_GenerateSTART(I2Cx, ENABLE);
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  
  // Send device address
  I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Receiver);
  while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  // Read data
  for (int i = 0; i < length; i++)
  {
    if (i == (length-1))
    {
      // Last byte, send NACK
      I2C_AcknowledgeConfig(I2Cx, DISABLE);
      I2C_GenerateSTOP(I2Cx, ENABLE);
    }
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data[i] = I2C_ReceiveData(I2Cx);
  }
  
  // Enable ACK for next transfer
  I2C_AcknowledgeConfig(I2Cx, ENABLE);
}
void tcs34725_setIntegrationTime(uint8_t time)
{
  // Write integration time to ATIME register
  tcs34725_writeByte(TCS34725_REG_ATIME, time);
}


