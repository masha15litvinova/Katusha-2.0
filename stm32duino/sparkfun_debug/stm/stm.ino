#define RX3 PD_9  //GYRO
#define TX3 PD_8


HardwareSerial GyroUART(RX3, TX3);

void setup() {

/*GyroUART.setRx(RX3);
GyroUART.setTx(TX3);*/

GyroUART.begin(115200);


pinMode(13, OUTPUT);
}

void loop() {
 GyroUART.print('3');
}
