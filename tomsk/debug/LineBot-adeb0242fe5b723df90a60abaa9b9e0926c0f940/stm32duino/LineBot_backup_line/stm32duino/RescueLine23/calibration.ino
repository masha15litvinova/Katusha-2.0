



void calibration() {
  
long int calibr_begin = millis();
  motors(V_CALIBR, -V_CALIBR);
  
  while ((millis() - calibr_begin)<DELAY_CALIBR_4/4) {
    int s1 = analogRead(SENSOR1);
    int s2 = analogRead(SENSOR2);
    int s3 = analogRead(SENSOR3);
    int s4 = analogRead(SENSOR4);
    int s5 = analogRead(SENSOR5);
    int s6 = analogRead(SENSOR6);

    if(s1>black[0]) black[0] = s1;
    if(s2>black[1]) black[1] = s2;
    if(s3>black[2]) black[2] = s3;
    if(s4>black[3]) black[3] = s4;
    if(s5>black[4]) black[4] = s5;
    if(s6>black[5]) black[5] = s6;

    if(s1<white[0]) white[0] = s1;
    if(s2<white[1]) white[1] = s2;
    if(s3<white[2]) white[2] = s3;
    if(s4<white[3]) white[3] = s4;
    if(s5<white[4]) white[4] = s5;
    if(s6<white[5]) white[5] = s6;


  }
  motors(-V_CALIBR, V_CALIBR);
  
  while ((millis() - calibr_begin)<DELAY_CALIBR_4/2) {
    int s1 = analogRead(SENSOR1);
    int s2 = analogRead(SENSOR2);
    int s3 = analogRead(SENSOR3);
    int s4 = analogRead(SENSOR4);
    int s5 = analogRead(SENSOR5);
    int s6 = analogRead(SENSOR6);

    if(s1>black[0]) black[0] = s1;
    if(s2>black[1]) black[1] = s2;
    if(s3>black[2]) black[2] = s3;
    if(s4>black[3]) black[3] = s4;
    if(s5>black[4]) black[4] = s5;
    if(s6>black[5]) black[5] = s6;

    if(s1<white[0]) white[0] = s1;
    if(s2<white[1]) white[1] = s2;
    if(s3<white[2]) white[2] = s3;
    if(s4<white[3]) white[3] = s4;
    if(s5<white[4]) white[4] = s5;
    if(s6<white[5]) white[5] = s6;


  }
  motors(-V_CALIBR, V_CALIBR);
  
  while ((millis() - calibr_begin)<DELAY_CALIBR_4*3/4) {
    int s1 = analogRead(SENSOR1);
    int s2 = analogRead(SENSOR2);
    int s3 = analogRead(SENSOR3);
    int s4 = analogRead(SENSOR4);
    int s5 = analogRead(SENSOR5);
    int s6 = analogRead(SENSOR6);

    if(s1>black[0]) black[0] = s1;
    if(s2>black[1]) black[1] = s2;
    if(s3>black[2]) black[2] = s3;
    if(s4>black[3]) black[3] = s4;
    if(s5>black[4]) black[4] = s5;
    if(s6>black[5]) black[5] = s6;

    if(s1<white[0]) white[0] = s1;
    if(s2<white[1]) white[1] = s2;
    if(s3<white[2]) white[2] = s3;
    if(s4<white[3]) white[3] = s4;
    if(s5<white[4]) white[4] = s5;
    if(s6<white[5]) white[5] = s6;


  }
  motors(V_CALIBR, -V_CALIBR);
  
  while ((millis() - calibr_begin)<DELAY_CALIBR_4) {
    int s1 = analogRead(SENSOR1);
    int s2 = analogRead(SENSOR2);
    int s3 = analogRead(SENSOR3);
    int s4 = analogRead(SENSOR4);
    int s5 = analogRead(SENSOR5);
    int s6 = analogRead(SENSOR6);

    if(s1>black[0]) black[0] = s1;
    if(s2>black[1]) black[1] = s2;
    if(s3>black[2]) black[2] = s3;
    if(s4>black[3]) black[3] = s4;
    if(s5>black[4]) black[4] = s5;
    if(s6>black[5]) black[5] = s6;

    if(s1<white[0]) white[0] = s1;
    if(s2<white[1]) white[1] = s2;
    if(s3<white[2]) white[2] = s3;
    if(s4<white[3]) white[3] = s4;
    if(s5<white[4]) white[4] = s5;
    if(s6<white[5]) white[5] = s6;


  }
  motors(0,0);
  for(int i=0; i<6; i++)
  {
    grey[i]=(white[i]+black[i])/2;
    grey[i] = map(grey[i], white[i], black[i], 0, 100);//по идее должно быть 50(+-1) у всех датчиков -среднее в диспазоне черный-белый
  }

}