#pragma once 
#include "project_config.h"
#include "motor.h"
#include "mlx906014.h"
#include "vl53l0x.h"
#include "camera.h"
#include "servo.h"
#include "gyro.h"

class Base_movement{
    public:
        Base_movement(Motor& motorFrRi,
                      Motor& motorFrLe,
                      Motor& motorBoRi,
                      Motor& motorBoLe,
                      mlx906014& heatRight,
                      mlx906014& heatLeft,
                      vl53l0x& FrontDist,
                      vl53l0x& RightDist,
                      vl53l0x& LeftDist,
                      vl53l0x& BotDist,
                      Camera& Cam,
                      Servo& vidServ,
                      Gyro& Gyros);
        bool movement(bool loop);
        void hillMovement();
        bool goBack(bool loop);
        bool rotate(uint8_t side,bool loop);
        void fullStop();
        void setYaw();
    private:
        STRONG_ENUM( movement_state,
            MOVING,
            LEVELING_FR,
            LEVELING_BA,
            TOUCH_ROTATE,
            TOUCH_BACK_MOVEMENT,
            TOUCH_FRONT_MOVEMENT,
            TOUCH_RI,
            TOUCH_LE
        );
        STRONG_ENUM( rotate_state,
            ROTATE,
            LEVELING_FR,
            LEVELING_BA
        );
        uint32_t m_backEnc;
    
        bool m_isSecondRot;
    
        movement_state::Type m_stateMov;
        movement_state::Type m_concSide;
        rotate_state::Type m_stateRot;
    
        uint16_t m_startYaw;
        uint32_t m_encStart;
        uint32_t m_getEnc();
        int16_t m_calcEncErrLR();
        int16_t m_calcEncErrFB();
        int16_t m_calcDistErr();
        
        uint32_t m_checkPointRast;
        
        uint32_t m_leftCount;
        uint32_t m_rightCount;
        uint32_t m_startRotateTime;
        
        int32_t errSumm;
        int32_t errPrev;
        uint32_t timePrev;
        uint32_t m_engSc;
        
        uint16_t m_yawSt;
        int32_t m_yawErrSumm;
        
        int16_t m_yawAim;
        uint32_t m_rotateCount;
        int32_t m_errOrSumm;
        int16_t m_oldErrOr;
        uint32_t m_timeStartRotate;
        
        uint32_t m_frontScet;
    
        Motor & m_motorFrRi;
        Motor & m_motorFrLe;
        Motor & m_motorBoRi;
        Motor & m_motorBoLe;
        mlx906014 & m_heatRight;
        mlx906014 & m_heatLeft;
        vl53l0x & m_FrontDist;
        vl53l0x & m_RightDist;
        vl53l0x & m_LeftDist;
        vl53l0x & m_BotDist;
        Camera & m_Cam;
        Servo & m_vidServ;
        Gyro & m_Gyros;
        uint32_t startTime;
        uint32_t startEnc;
        uint32_t m_timeStartConc;
};
