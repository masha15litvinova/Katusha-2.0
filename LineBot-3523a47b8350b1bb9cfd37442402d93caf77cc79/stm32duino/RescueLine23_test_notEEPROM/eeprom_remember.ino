/*white0[0] white0[1] white0[2] black0[3] black0[4] black0[5] --сотни/десятки/тысячи
  white1[6] white1[7] white1[8] black1[9] black1[10] black1[11]
  white2[12] white2[13] white2[14] black2[15] black2[16] black2[17]
  white3[18] white3[19] white3[20] black3[21] black3[22] black3[23]
  white4[24] white4[25] white4[26] black4[27] black4[28] black4[29]
  white5[30] white5[31] white5[32] black5[33] black5[34] black5[35]
  yaw0[36] yaw0[37] yaw0[38]
  */

void eeprom_write_line() {
  EEPROM.write(0, (white[0]/100));
  EEPROM.write(1, ((white[0]/10)%10));
  EEPROM.write(2, (white[0]%10));
  EEPROM.write(3, (black[0]/100));
  EEPROM.write(4, ((black[0]/10)%10));
  EEPROM.write(5, (black[0]%10));

  EEPROM.write(6, (white[1]/100));
  EEPROM.write(7, ((white[1]/10)%10));
  EEPROM.write(8, (white[1]%10));
  EEPROM.write(9, (black[1]/100));
  EEPROM.write(10, ((black[1]/10)%10));
  EEPROM.write(11, (black[1]%10));

  EEPROM.write(12, (white[2]/100));
  EEPROM.write(13, ((white[2]/10)%10));
  EEPROM.write(14, (white[2]%10));
  EEPROM.write(15, (black[2]/100));
  EEPROM.write(16, ((black[2]/10)%10));
  EEPROM.write(17, (black[2]%10));

  EEPROM.write(18, (white[3]/100));
  EEPROM.write(19, ((white[3]/10)%10));
  EEPROM.write(20, (white[3]%10));
  EEPROM.write(21, (black[3]/100));
  EEPROM.write(22, ((black[3]/10)%10));
  EEPROM.write(23, (black[3]%10));

  EEPROM.write(24, (white[4]/100));
  EEPROM.write(25, ((white[4]/10)%10));
  EEPROM.write(26, (white[4]%10));
  EEPROM.write(27, (black[4]/100));
  EEPROM.write(28, ((black[4]/10)%10));
  EEPROM.write(29, (black[4]%10));

  EEPROM.write(30, (white[5]/100));
  EEPROM.write(31, ((white[5]/10)%10));
  EEPROM.write(32, (white[5]%10));
  EEPROM.write(33, (black[5]/100));
  EEPROM.write(34, ((black[5]/10)%10));
  EEPROM.write(35, (black[5]%10));
}
void eeprom_read_line() {
  white[0] = 100 * EEPROM.read(0) + 10 * EEPROM.read(1) + EEPROM.read(2);
  black[0] = 100 * EEPROM.read(3) + 10 * EEPROM.read(4) + EEPROM.read(5);

  white[1] = 100 * EEPROM.read(6) + 10 * EEPROM.read(7) + EEPROM.read(8);
  black[1] = 100 * EEPROM.read(9) + 10 * EEPROM.read(10) + EEPROM.read(11);

  white[2] = 100 * EEPROM.read(12) + 10 * EEPROM.read(13) + EEPROM.read(14);
  black[2] = 100 * EEPROM.read(15) + 10 * EEPROM.read(16) + EEPROM.read(17);

  white[3] = 100 * EEPROM.read(18) + 10 * EEPROM.read(19) + EEPROM.read(20);
  black[3] = 100 * EEPROM.read(21) + 10 * EEPROM.read(22) + EEPROM.read(23);

  white[4] = 100 * EEPROM.read(24) + 10 * EEPROM.read(25) + EEPROM.read(26);
  black[4] = 100 * EEPROM.read(27) + 10 * EEPROM.read(28) + EEPROM.read(29);

  white[5] = 100 * EEPROM.read(30) + 10 * EEPROM.read(31) + EEPROM.read(32);
  black[5] = 100 * EEPROM.read(33) + 10 * EEPROM.read(34) + EEPROM.read(35);
}

void eeprom_write_gyro() {
  EEPROM.write(36, (robot.start_angle_y / 100));
  EEPROM.write(37, ((robot.start_angle_y / 10) % 10));
  EEPROM.write(38, (robot.start_angle_y % 10));
}
void eeprom_read_gyro() {
  robot.start_angle_y = 100 * EEPROM.read(36) + 10 * EEPROM.read(37) + EEPROM.read(38);
}
