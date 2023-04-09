#include "speed_control.h"

int vel1(int speed, long int enc1)
{
	float speed_k = SPEED_COEFF;
	float enc_const = ENC_COEFF;
	
	float err = speed*enc_const + (Encoder1()-enc1);
	int u = (err * speed_k);
	return u;
}
int vel2(int speed, long int enc2)
{
	float speed_k = SPEED_COEFF;
	float enc_const = ENC_COEFF;
	
	float err = speed*enc_const + (Encoder2()-enc2);
	int u = (err * speed_k);
	return u;
}
int vel3(int speed, long int enc3)
{
	float speed_k = SPEED_COEFF;
	float enc_const = ENC_COEFF;
	
	float err = speed*enc_const + (Encoder3()-enc3);
	int u = (err * speed_k);
	return u;
}
int vel4(int speed, long int enc4)
{
	float speed_k = SPEED_COEFF;
	float enc_const = ENC_COEFF;
	
	float err = speed*enc_const + (Encoder4()-enc4);
	int u = (err * speed_k);
	return u;
}
