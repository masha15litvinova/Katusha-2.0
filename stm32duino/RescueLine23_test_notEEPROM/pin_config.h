
/*********OLED DISPLAY************/
#define OLED_MOSI PC_12
#define OLED_CLK PC_10
#define OLED_DC PC_13
#define OLED_CS PA_11
#define OLED_RST PA_12
/*********************************/

/*********LED PINS************/
#define LED1 PE0
#define LED2 PE1
/*********************************/

/*********UART PINS************/
#define USART3
#define USART6
#define USART2

#define RX3 PD_9  //GYRO
#define TX3 PD_8

#define RX6 PG_9  //CAMERA
#define TX6 PG_14

#define RX2 PD_6  //STLINK
#define TX2 PD_5
/*********************************/

#define BUTTON1 PC_6  //102
#define BUTTON2 PC_7  //103



/*********OTHER************/
#define CAMERA_RST PA_4
#define GYRO_RST PA_6
#define GYRO_INTERRUPT PB_9
#define PWM_LIGHTS PA_3_ALT1
#define LED1 PE_0
#define LED2 PE_1
#define BALL PA_2
#define FRONT_DIST PA_7

/******LASER DIST SENSORS******/
#define XSHUT1 PG_10
#define XSHUT2 PG_11
#define XSHUT3 PG_12
#define XSHUT4 PG_13

#define SWITCH1 PA_9
#define SWITCH2 PA_10
#define SWITCH3 PE_3
#define SWITCH4 PE_2

#define SDA1 PB_7
#define SCL1 PB_6

/**********************************/


/**********MOTORS PINS****************/
#define MOTOR1_1 PB_0_ALT1
#define MOTOR1_2 PB_1_ALT1
#define MOTOR2_1 PE_5
#define MOTOR2_2 PE_6
#define MOTOR3_1 PB_4
#define MOTOR3_2 PB_5
#define MOTOR4_1 PB_11
#define MOTOR4_2 PB_10
/************************************/

/*****LIGHT SENSORS**************/
#define SENSOR1 PF10
#define SENSOR2 PF9
#define SENSOR3 PF8
#define SENSOR4 PF7
#define SENSOR5 PF6
#define SENSOR6 PF3
/*********************************/
/*************SERVOS**************/
#define SERVO1 PD_12
#define SERVO2 PD_13
#define SERVO3 PA_5
/********************************/
/*****COLOR SENSORS**************/
#define SDA2 PF_0
#define SCL2 PF_1
#define SDA3 PC_9
#define SCL3 PA_8
/*********************************/
/*****ENDSTOPS**************/
#define ENDSTOP1 PC_3
#define ENDSTOP2 PB_15
#define ENDSTOP3 PD_11
#define ENDSTOP4 PC_2
/*********************************/
/******ANALOG SENSORS******/
#define CURRENT1 PC_1
#define CURRENT2 PC_0

#define POTENTIOMETER1 PF4  //84//PF4
#define POTENTIOMETER2 PF5    //PF5
/*********************************/
/*****ENCODERS SENSORS********/
#define ENC1 PG_3
#define ENC2 PG_2
#define ENC3 PG_5
#define ENC4 PG_4

#define ENC_INT1 PB_14
#define ENC_INT2 PB_12
#define ENC_INT3 PE_11
#define ENC_INT4 PE_12
/*********************************/
