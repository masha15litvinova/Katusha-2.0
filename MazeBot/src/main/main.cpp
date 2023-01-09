#include "project_config.h"
#include "stm32f4xx.h"
#include "pinsStr.h"
#include "pins.h"
#include "vl53l0x.h"
#include "softI2C.h"
#include "analog_input.h"
#include "math.h"

#include "SSD1306.h"
#include "Display.h"
#include "pwm.h"
#include "irqs.h"
#include "InitImage.h"
#include "tcs34725.h"
#include "uart.h"


		
//Display display;
	
static const uint8_t distFrontAdress = 20;
static const uint8_t distBackAdress = 18;
static const uint8_t distRightFrontAdress = 12;
static const uint8_t distLeftFrontAdress = 10;

int64_t e1 = 0;
int x = 0;
int m = 0;
int camData = 0;
uint16_t r, g, b, c;

int state = LINE;

int main()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
	
	 initEndstops();
	 initEXTI();
	 initNVIC();
	 
	
	
	
	 InitUART6();
	
	 __enable_irq();
	
	sysStart();
  int initImageTimer = millis();
	time_service::init();
  time_service::startTime();
	SSD1306 disp(DISPLAY_SPI, DISPLAY_DC, DISPLAY_RESET, DISPLAY_CS, DISPLAY_PWR_EN, DISPLAY_SCK, DISPLAY_MOSI);
	
	/*disp.begin();
	disp.clear();
	
	disp.invert(0);
	disp.rotate(2);
	
	//delay(500);
	disp.clear();
	disp.display();
	//Initialize display
	//display.init(&disp);

	
	delay(500);*/
		//setupScreens();


	
	
/*initMotorPins();
		initPWM();*/
		
		
  
  // Initialize TCS34725
  //tcs34725_init();

		
		 while(1)
		{
		
   // tcs34725_getData(&r, &g, &b, &c);
    
			/*if(Button1() == 1)
			{
				state = BUTTON1_ON;
			}
			else if(Button2() == 1)
			{
				state = BUTTON2_ON;
			}
			else if(Button3() == 1)
			{
				state = BUTTON3_ON;
			}
			else if(Button4() == 1)
			{
				state = BUTTON4_ON;
			}*/
			
			camData = GetUART6data();//GetUART6data();
			/*switch(state)
			{
				case LINE:
					
					break;
				case BUTTON1_ON: 
					x = 3;
					delay(1000);
					x = 0;
					state = LINE;
				resetButton1();
					break;
				case BUTTON2_ON:
					x = 4;
					delay(1000);
					x = 0;
					state = LINE;
				resetButton2();
					break;
				case BUTTON3_ON:
					x = 5;
					delay(1000);
					x = 0;
					state = LINE;
				resetButton3();
					break;
				case BUTTON4_ON:
					x = 6;
					delay(1000);
					x = 0;
					state = LINE;
				resetButton4();
					break;
			}*/
			
    
    //delay(10);
			//resetButtons();
		}
		
		
		
	}
