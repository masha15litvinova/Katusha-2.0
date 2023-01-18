
/*********OLED DISPLAY************/
#define OLED_MOSI     PC12
#define OLED_CLK      PC10
#define OLED_DC       PC13
#define OLED_CS       PA11
#define OLED_RST      PA12
/*********************************/

/*********LED PINS************/
#define LED1 PE0
#define LED2 PE1
/*********************************/

/*********UART PINS************/
#define USART3
#define USART6

#define RX3 PD9 //GYRO
#define TX3 PD8

#define RX6 PG9 //CAMERA
#define TX6 PG14
/*********************************/

#define BUTTON1 PF14
#define BUTTON2 PF15



/*********OTHER************/
#define CAMERA_RST PA4
#define GYRO_RST PA6
#define GYRO_INTERRUPT PB9
#define PWM_LIGHTS PA3
#define LED1 PE0
#define LED2 PE1

/******LASER DIST SENSORS******/
#define XSHUT1 PG10
#define XSHUT2 PG11
#define XSHUT3 PG12
#define XSHUT4 PG13

#define SWITCH1 PA9
#define SWITCH2 PA10
#define SWITCH3 PE3
#define SWITCH4 PE2

#define SDA1 PB7
#define SCL1 PB6

/**********************************/


/**********MOTORS PINS****************/
#define MOTOR1_1 PB0
#define MOTOR1_2 PB1
#define MOTOR2_1 PE5
#define MOTOR2_2 PE6
#define MOTOR3_1 PB4
#define MOTOR3_2 PB5
#define MOTOR4_1 PB11
#define MOTOR4_2 PB10
/************************************/

/*****LIGHT SENSORS**************/
#define SENSOR1 PF_10
#define SENSOR2 PF_9
#define SENSOR3 PF_8
#define SENSOR4 PF_7
#define SENSOR5 PF_6
#define SENSOR6 PF_3
/*********************************/

/*****COLOR SENSORS**************/
#define SDA2 PF0
#define SCL2 PF1
#define SDA3 PC9
#define SCL3 PA8
/*********************************/
/******ANALOG SENSORS******/
#define CURRENT1 PC1
#define CURRENT2 PC0

#define POTENTIOMETER1 PF_4//84//PF4
#define POTENTIOMETER2 85//PF5
/*********************************/
/*****ENCODERS SENSORS********/
#define ENC1 PG3
#define ENC2 PG2
#define ENC3 PG5
#define ENC4 PG4

#define ENC_INT1 PB14
#define ENC_INT2 PB12
#define ENC_INT3 PE11
#define ENC_INT4 PE12
/*********************************/