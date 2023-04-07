void setup() {
  Serial1.begin(115200);
}
int receive = -1;
void loop() {


  if (Serial1.available()) {
    receive = Serial1.read()-'0';
  }
  if (receive == 3) {
    digitalWrite(13, HIGH);
  }
}


