//Code written by: Tyler Montcalm
#include "PID_traction.h"
#include "definitions.h"


float U_traction=0;
float percent_speed=50;
float my_motor_speed_traction=0;

void loop() 
{
  // Idea is to adjust the speed of the rear so that the front and back wheels are reading an equivalent speed i.e no slip
  my_motor_speed_traction=(10*percent_speed)+U_traction;
  my_motor(my_motor_speed_traction);
  U_traction=pid_traction_calculate(percent_speed); // this will return a U
  
}
