#define RX3 PD_9  //GYRO
#define TX3 PD_8


HardwareSerial GyroUART(RX3, TX3);

void setup() {

/*GyroUART.setRx(RX3);
GyroUART.setTx(TX3);*/

GyroUART.begin(9600);


pinMode(13, OUTPUT);
}

void loop() {
  for(int i = 0; i<10; i++)
  {
 GyroUART.print('3');
 delay(20);
  }
  for(int i = 0; i<10; i++)
  {
 GyroUART.print('2');
 delay(20);
  }
  for(int i = 0; i<10; i++)
  {
 GyroUART.print('1');
 delay(20);
  }
  for(int i = 0; i<10; i++)
  {
 GyroUART.print('0');
 delay(20);
  }
}
