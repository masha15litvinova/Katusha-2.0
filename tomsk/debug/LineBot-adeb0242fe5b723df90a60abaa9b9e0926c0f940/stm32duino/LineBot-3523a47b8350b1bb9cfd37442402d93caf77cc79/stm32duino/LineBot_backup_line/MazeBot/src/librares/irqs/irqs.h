#pragma once
#include "project_config.h"

#ifdef __cplusplus
 extern "C" {
#endif
void Init_Encoders(void);

int Encoder1();
int Encoder2();
int Button1();
int Button2();
int Button3();
int Button4();
void initEndstops();
void initNVIC();
void initEXTI();	 
void resetButton1();
void resetButton2();
void resetButton3();
void resetButton4();
 #ifdef __cplusplus
}
#endif