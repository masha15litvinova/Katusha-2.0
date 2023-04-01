class Robot {
public:
  Robot();
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



  long int timeGyro = 0;
  long int timeCamera = 0;
  long int timeDist2 = 0;  //передний дальномер снизу (опрашивается, когда робот следует по линии)
  long int timeMotors = 0;
  long int timeLED = 0;
  long int timeLineSens = 0;
  long int timeCurrSens = 0;
};

Robot::Robot(){};
