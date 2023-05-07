#pragma once
#include "project_config.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
	 void InitUART3(); //SparkFun module
	 void InitUART6(); //camera OpenMV
	 int GetUART3data();
	 int GetUART6data();

	 
 #ifdef __cplusplus
}
#endif