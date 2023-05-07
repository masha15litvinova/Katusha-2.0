#include "stm32f10x.h"
#include "encoders.h"

#define SPEED_COEFF 100
#define ENC_COEFF 0.065


#ifdef __cplusplus
 extern "C" {
#endif
int vel1(int speed, long int enc1);
int vel2(int speed, long int enc2);
int vel3(int speed, long int enc3);
int vel4(int speed, long int enc4);

#ifdef __cplusplus
}
#endif