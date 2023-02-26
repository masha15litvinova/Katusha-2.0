class Robot {
  public:
    Robot();
    int v = 0;
    int v1_target = 0;  //требуемая скорость
    int v2_target = 0;

    int motor1 = 0;  //подаваемый шим по факту
    int motor2 = 0;

    int sensors[6] = { 0, 0, 0, 0, 0, 0 };
    long int turnBegin = 0;
    int startPosition = 10000;
    int targetAngle = 0;
    int errAngle = 0;
    int angle = 0;
    int angle_p = 0;
    int start_angle_p = 0;
    int angle_uart;

    float u_gyro = 0;
    float up_gyro = 0;
    float ui_gyro = 0;

    bool turnCompleted = true;

    int u_line = 0;
    int up_line = 0;
    int ui_line = 0;
    int ud_line = 0;

    int current1 = 0;
    int current2 = 0;

    int potentiometer1 = 0;
    int potentiometer2 = 0;

    int ball = 0;

    int countCam = 0;
    int dataCam[2] = {0, 0};

    long int timeGyro = 0;
    long int timeCamera = 0;
    long int timeDist2 = 0;  //передний дальномер снизу (опрашивается, когда робот следует по линии)
    long int timeMotors = 0;
    long int timeLED = 0;
    long int timeLineSens = 0;
    long int timeCurrSens = 0;


    float up1 = 0;
    float ud1 = 0;
    float ui1 = 0;

    float u1 = 0;

    float up2 = 0;
    float ud2 = 0;
    float ui2 = 0;

    float u2 = 0;

    float err1 = 0;
    float err1_old = 0;
    float err2 = 0;
float err2_old = 0;

long int prev_enc1 = 0;
long int prev_enc2 = 0;

float current_speed1 = 0;

float current_speed2 = 0;


};

Robot::Robot() {};
