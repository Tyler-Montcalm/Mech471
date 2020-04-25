//Code written by: Tyler Montcalm
#include "PID_speed.h"
#include "definitions.h"

float percent_speed=50;
float my_motor_speed=0;
float U_speed=0;
float t0_speed=0;

void setup()
{
 t0_speed=micros()*11.0e-6;
}

void loop() 
{
 // First we write an approximately close motor speed, 80 percent would be 80 percent of 1000, then we iterate PID until we hit desired wheel speed
 // this allows for the PID to work on any surface or incline it essentially adds speed to the ESC until the approximate motor voltage is read
 my_motor_speed=(10*percent_speed)+U_speed;
// my_motor(my_motor_speed);
 U_speed=pid_speed_calculate(percent_speed,t0_speed); // this will return a U
  
}
