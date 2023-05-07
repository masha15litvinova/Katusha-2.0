#include "project_config.h"
#include "stm32f4xx.h"
#include "pinsStr.h"
#include "pins.h"
#include "vl53l0x.h"
#include "mlx906014.h"
#include "motor.h"
#include "servo.h"
#include "gyro.h"
#include "camera.h"
#include "base_movment.h"
#include "map_operations.h"
#include "softI2C.h"
#include "main_func.h"
#include "display.h"
#include "pos_sens.h"
#include "posSensReader.h"
#include "analog_input.h"
#include "math.h"

#define useDebug


static const uint8_t distFrontAdress = 20;
static const uint8_t distRightAdress = 16;
static const uint8_t distLeftAdress = 28;
static const uint8_t distBackAdress = 18;
static const uint8_t distRightFrontAdress = 12;
static const uint8_t distLeftFrontAdress = 10;

int main()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    time_service::init();
    time_service::startTime();
    
    //Enable pins init
    pins::initFunc::pinInit(otherPins::LeftSensEn);
    GPIO_SetPin(otherPins::LeftSensEn);
    pins::initFunc::pinInit(vlxPins::enFr);
    GPIO_SetPin(vlxPins::enFr);
    pins::initFunc::pinInit(vlxPins::enBo);
    GPIO_SetPin(vlxPins::enBo);
    pins::initFunc::pinInit(vlxPins::enLeFr);
    GPIO_SetPin(vlxPins::enLeFr);
    pins::initFunc::pinInit(vlxPins::enRiFr);
    GPIO_SetPin(vlxPins::enRiFr);
    pins::initFunc::pinInit(otherPins::LeftSensEn);
    GPIO_SetPin(otherPins::LeftSensEn);
    pins::initFunc::pinInit(otherPins::RightSensEn);
    GPIO_SetPin(otherPins::RightSensEn);
    pins::initFunc::pinInit(FrRiMotPins::en);
    GPIO_SetPin(FrRiMotPins::en);
    pins::initFunc::pinInit(FrLeMotPins::en);
    GPIO_SetPin(FrLeMotPins::en);
    pins::initFunc::pinInit(BoRiMotPins::en);
    GPIO_SetPin(BoRiMotPins::en);
    pins::initFunc::pinInit(BoLeMotPins::en);
    GPIO_SetPin(BoLeMotPins::en);
    
    
    
    //подтягивания xshut дальномеров к земле
    pins::initFunc::pinInit(vlxPins::xshutFr);
    GPIO_ResetPin(vlxPins::xshutFr);
    pins::initFunc::pinInit(vlxPins::xshutBo);
    GPIO_ResetPin(vlxPins::xshutBo);
    pins::initFunc::pinInit(vlxPins::xshutLeFr);
    GPIO_ResetPin(vlxPins::xshutLeFr);
    pins::initFunc::pinInit(vlxPins::xshutRiFr);
    GPIO_ResetPin(vlxPins::xshutRiFr);
    pins::initFunc::pinInit(vlxPins::xshutRi);
    GPIO_ResetPin(vlxPins::xshutRi);
    pins::initFunc::pinInit(vlxPins::xshutLe);
    GPIO_ResetPin(vlxPins::xshutLe);
    
   

    static Display disp;
    static OLED_GFX work_disp = OLED_GFX();
    work_disp.Device_Init();
    work_disp.Set_Color(RED);
    work_disp.Clear_Screen();
   
    work_disp.print_String(0, 20, "Periph init", FONT_5X8);
    
   
    
    //инициализация дальномеров
    static SoftI2C softVl(vlxPins::sclPinVL,vlxPins::sdaPinVL);
    static vl53l0x distFront(vlxPins::xshutFr, vlxPins::enFr, distFrontAdress ,&softVl, 0);
    static vl53l0x distRight(vlxPins::xshutRi, otherPins::RightSensEn, distRightAdress ,&softVl, 1);
    static vl53l0x distLeft(vlxPins::xshutLe, otherPins::LeftSensEn,distLeftAdress ,&softVl, 2);
    static vl53l0x distBack(vlxPins::xshutBo, vlxPins::enBo, distBackAdress ,&softVl, 3);
    static vl53l0x distRightFront(vlxPins::xshutRiFr, vlxPins::enRiFr, distRightFrontAdress ,&softVl, 4);
    static vl53l0x distLeftFront(vlxPins::xshutLeFr, vlxPins::enLeFr, distLeftFrontAdress ,&softVl, 5);
    work_disp.print_String(0, 30, "Dist init complete", FONT_5X8);
    
   
    
   
    
    
    //static uint32_t time;
    
    //static uint32_t dt;
    
    __enable_irq();
    work_disp.Clear_Screen();
   
       
    volatile uint16_t light;
    volatile static uint8_t readbyte;
   
    work_disp.print_String(0, 20, "start", FONT_5X8);
    time_service::delay_ms(500);
    work_disp.Clear_Screen();
    volatile int32_t fl = 0, bl = 0, fr = 0, br = 200;
/*#ifdef useDebug
    bool isInterupted = 0;
    bool variant = 1;
#endif
    while(1)
    {        
        //time = time_service::getCurTime();
#ifdef useDebug
        if(isInterupted)
        {
            func.loop(0);
            char str[15];
            if(GPIO_Read(otherPins::button2))
            {
                motorFrontLeft.setPower(500);
                motorFrontRight.setPower(500);
                motorBackLeft.setPower(500);
                motorBackRight.setPower(500);
            }
            else
            {
                movement.fullStop();
            }
            if(variant)
            {
                sprintf(str, "distFr = %i   ", distFront.takeSens());
                work_disp.print_String(0, 0, str, FONT_5X8);            
                sprintf(str, "distRi = %i   ", distRight.takeSens());
                work_disp.print_String(0, 10, str, FONT_5X8);
                sprintf(str, "distLe = %i   ", distLeft.takeSens());
                work_disp.print_String(0, 20, str, FONT_5X8);
                sprintf(str, "distBa = %i   ", distBack.takeSens());
                work_disp.print_String(0, 30, str, FONT_5X8);
                sprintf(str, "distFrRi = %i   ", distRightFront.takeSens());
                work_disp.print_String(0, 40, str, FONT_5X8);
                sprintf(str, "distFrLe = %i   ", distLeftFront.takeSens());
                work_disp.print_String(0, 50, str, FONT_5X8);
                sprintf(str, "yaw = %i   ", gyro.getYaw());
                work_disp.print_String(0, 60, str, FONT_5X8);
                sprintf(str, "pitch = %i   ", gyro.getRoll());
                work_disp.print_String(0, 70, str, FONT_5X8);
                sprintf(str, "accel = %i   ", gyro.getAccel());
                work_disp.print_String(0, 80, str, FONT_5X8);
                sprintf(str, "rightH = %i     ", (int)heatRi.takeTemp());
                work_disp.print_String(0, 90, str, FONT_5X8);
                sprintf(str, "leftH = %i   ", (int)heatLe.takeTemp());
                work_disp.print_String(0, 100, str, FONT_5X8);
                sprintf(str, "light = %i   ", analog_sensors.getAdc(analog_sensors.lightSensor));
                work_disp.print_String(0, 110, str, FONT_5X8);
                variant = 1;
            }
            else
            {
                sprintf(str, "motorFrRi = %i   ", motorFrontRight.getEnc());
                work_disp.print_String(0, 0, str, FONT_5X8);            
                sprintf(str, "motorFrLe = %i   ", motorFrontLeft.getEnc());
                work_disp.print_String(0, 10, str, FONT_5X8);
                sprintf(str, "motorBaRi = %i   ", motorBackRight.getEnc());
                work_disp.print_String(0, 20, str, FONT_5X8);
                sprintf(str, "motorBaLe = %i   ", motorBackLeft.getEnc());
                work_disp.print_String(0, 30, str, FONT_5X8);
                sprintf(str, "rast = %f   ", 800 / cos(float(gyro.getRoll() / (180 / 3.1415))));
                work_disp.print_String(0, 40, str, FONT_5X8);
                sprintf(str, "concR = %i   ", gyro.ConcR());
                work_disp.print_String(0, 50, str, FONT_5X8);
                sprintf(str, "concL = %i   ", gyro.ConcL());
                work_disp.print_String(0, 60, str, FONT_5X8);
                variant = 0;
            }
            isInterupted = 1;
        }
        else
            func.loop(1);
        
        if(GPIO_Read(otherPins::button1))
        {
            work_disp.Clear_Screen();
            isInterupted = 1;
            variant = !variant;
        }
#else
        func.loop(1);
        work_disp.print_String(10, 10, "SCREEN", FONT_8X16);
        work_disp.print_String(0, 40, "Working...", FONT_5X8);
#endif
        //fl = posSensors::period[0];
        //fr = posSensors::period[1];
        //bl = posSensors::period[2];
        //br = posSensors::period[3];
        //gyro.worker();
        /*motorFrontLeft.worker();
        motorBackLeft.worker();
        motorFrontRight.worker();
        motorBackRight.worker();
        motorFrontLeft.setPower(fl);
        motorFrontRight.setPower(fr);
        motorBackLeft.setPower(bl);
        motorBackRight.setPower(br);*/
        //dt = time_service::getCurTime() - time;
        //char str[15];
        //sprintf(str, "yaw = %i___", gyro.getYaw());
        //work_disp.print_String(0, 0, str, FONT_5X8);
        //cam.takeSens();*/
		char str[4];
		
        while(1)
		{
			distFront.worker();
			distRight.worker();
			time_service::delay_ms(25);
			sprintf(str, "distFr = %i   ", distFront.takeSens());
            work_disp.print_String(0, 0, str, FONT_5X8); 
			sprintf(str, "distRi = %i   ", distRight.takeSens());
            work_disp.print_String(0, 10, str, FONT_5X8); 
		}
}
