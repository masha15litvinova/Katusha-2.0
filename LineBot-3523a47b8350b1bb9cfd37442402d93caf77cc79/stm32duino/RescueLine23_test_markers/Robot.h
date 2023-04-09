class Robot {
public:
  Robot();
  int gorka = 0;  //переменная, означающая, что робот въехал на горку
  
  //0 - пол внизу
  //1 - горка вверх
  //2 - пол наверху
  //3 - горка вниз
  int v = 0;
  int v1_target = 0;  //требуемая скорость
  int v2_target = 0;
  long int encoder_remember = 0;  //1 motor
  long int time_remember = 0;
  int motor1 = 0;  //подаваемый шим по факту
  int motor2 = 0;

  int sensors[6] = { 0, 0, 0, 0, 0, 0 };
  float sensors_analog[6] = { 0, 0, 0, 0, 0, 0 };
  long int turnBegin = 0;
  int startPosition = 10000;
  int targetAngle = 0;
  int errAngle = 0;
  int angle = 0;
  int angle_p = 0;
  int angle_r = 0;
  int start_angle_p = 0;
  int start_angle_y = 0;
  int angle_uart;
  int angle_yaw = 0;
  int angle_pitch = 0;
  int angle_roll = 0;
  byte gyroStarted = 0;

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
  int dataCam[2] = { 0, 0 };
  int camDir = 3;
  int camLineAngle = 0;
  int camLineDev = 0;
  int camLineAngleOld = 0;
  int up_cam = 0;
  int ui_cam = 0;
  int ud_cam = 0;
  bool turn_detected = false;
  float p_cam = 0;       //-0.5;//-1.0;             //-2.0;//-1.8;//0.1;//-0.5;//-1.7;
  float p_cube_cam = 0;  //-0.0001;  //-0.00007;//-0.000002;  //-0.0002;
  float p_cam_line = 0;  //0.2;//0.5;//0.5;      //0.03;//-0.03;//0.02;
  float i_cam = 0;       //-0.001;             //-0.0002;//-0.0001;
  float d_cam = 0;       //-0.5;//-0.05;


  float kp_line = 120;     //1000;//300;//1.2;       //2.2
  float kcube_line = 0.12;  //3;//0.000001;//0.0045;  //0.04; //0.028
  float kd_line = 0.0015;//0.0015;   //0.01;     //1.0;//-1.5;
  float ki_line = 2.7;     //0.1;//0.5;//0.00001;//0.003;//0.009;     //0.03;//2.0;//0.5;//0.012;//0.0005;//0.009;

  long int timeGyro = 0;
  long int timeCamera = 0;
  long int timeDist2 = 0;  //передний дальномер снизу (опрашивается, когда робот следует по линии)
  long int timeMotors = 0;
  long int timeLED = 0;
  long int timeLineSens = 0;
  long int timeCurrSens = 0;
  long int timeMotor1 = 0;
  long int timeMotor2 = 0;
  long int timeColors = 0;

  int colorDist1 = 1000;
  int colorDist2 = 1000;
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

  long int online = millis();
};

Robot::Robot(){};
