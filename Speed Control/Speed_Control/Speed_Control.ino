#include "PID_speed.h"
#include "definitions.h"
float percent_speed=50;

void loop() 
{
 // here we want to use the PID_speed function in order to 
 //iterate through motor write and checking to see if its good as fast as possible
 // U is in terms of voltage to write we must first map this to the motor controller as well
 pid_speed_calculate(percent_speed);
 
}
