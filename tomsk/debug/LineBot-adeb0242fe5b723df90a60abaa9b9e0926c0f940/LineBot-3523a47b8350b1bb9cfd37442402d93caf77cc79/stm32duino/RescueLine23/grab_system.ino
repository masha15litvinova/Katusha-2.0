


float kp_sliders = 0.2;
float ki_sliders = 0.8; 
float kd_sliders = 0.0004;
float prevErr = 0;
float dt = 0.005;
float h1 = 0;
int h2 = 0;
int max_target_position = 60; //unit = mm


void sliders_movement(int target_position)
{
  h1 = analogRead(POTENTIOMETER2);
  if(target_position < 0)target_position=abs(target_position);
  if(target_position > max_target_position)target_position=max_target_position;
  float err_h1 = (h1 - target_position*68);
  static float integral = 0;
  integral = integral + (float)err_h1 * dt * ki_sliders;
  float D = (err_h1 - prevErr) / dt;
  prevErr = err_h1;
  float result = err_h1 * kp_sliders + integral + D * kd_sliders;
  if(abs(result)<60)result=0;
  sliders(-result,0);  
}
