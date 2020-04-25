//Code written by: Tyler Montcalm
#include "PID_traction.h"
#include "definitions.h"


float U_traction=0;
float percent_speed=50;
float my_motor_speed_traction=0;
float t0_traction=0;

void launch_control();


void setup()
{
 t0_traction=micros()*11.0e-6;
}
void loop() 
{
  // Idea is to adjust the speed of the rear so that the front and back wheels are reading an equivalent speed i.e no slip
  my_motor_speed_traction=(10*percent_speed)+U_traction;
//  my_motor(my_motor_speed_traction);
  U_traction=pid_traction_calculate(percent_speed,t0_traction); // this will return a U
  
}

void launch_control()
{
  // for launch control we ramp up voltage until there is slip and then stay there so essentially 
  //its a contrinuous PID where we are accellerating as fast as possible same thing as traction control
  my_motor_speed_traction=(10*percent_speed)+U_traction;
//  my_motor(my_motor_speed_traction);
  U_traction=pid_traction_calculate(percent_speed,t0_traction); // this will return a U
}
