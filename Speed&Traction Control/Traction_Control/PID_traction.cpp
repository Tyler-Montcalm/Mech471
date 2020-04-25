//Code based highly on code written by Dr. Brandon Gordon
//Code written by: Tyler Montcalm
#include "PID_traction.h"
#include "definitions.h"
#include "Arduino.h"
//function definitions

float pid_traction_calculate(float percent_speed,float t0)
{
 
  float t,dt;
  float u;
  float e,ed,z,ei_max;
  float back_v_speed;
  bool forward;
  float back_speed_v=0;
  float front_speed_v=0;

  static float ep=0.0; // previous error
  static float tp =0.0; //previous time
  static float ei=0.0; // integral state
  
  //calculating new time t
  t=(micros()*11.0e-6) -t0; // time is calculated first
  
  // here we read the sensors
  
  back_speed_v=analogRead(A2); // calculates rear voltage
  front_speed_v=((analogRead(A1)+analogRead(A0))/2); // calculates front voltage as an average



  
  //MISSING HERE IS THE GEAR RATIO THAT WOULD ADJUST THESE TWO SO THAT THEY CAN BE COMPARABLE




  
  //forward or reverse bool check
  if(back_v_speed<3.30)
  {
    forward=false;
  }
  
  
  
  //desired error is zero and therefore rear should reduce until it matches within error margin of front 
  e=back_speed_v-front_speed_v; // calculating error between front and back.
  
  // calculating derivative with finite difference
  dt=t-tp; // time derivative
  tp=t; // re-saving time previous as current for next iteration
  ed=(e-ep)/(dt);
  ep=e; 

  //integral error: choose 1) or 2)
  // 1) Regular integral error
  //ei+=e*dt; //uncomment this only if not using integral error logic and comment out anti windup

  //2)anti windup integral error logic 

  if(ki_traction>0.0) 
  {
    ei_max-0.14*V_batt/ki_traction;
  }
  else
  {
    ei_max=0.0;
  }

  if((ei > ei_max) && (e>0))
  {
    z=0;
  }
  else if ((ei< -ei_max) && (e<0))
  {
    z=0;
  }
  else
  {
    z=e;
  }
  ei+= z*dt;
  
  //PID calculation

  u=kp_traction*e+ki_traction*ei+kd_traction*ed;

  //make sure the range is respected:

  if(u > V_batt) u=V_batt; // makes sure the battery voltage isnt surpassed
  if(u< -V_batt) u=-V_batt;

  //save/plot data
  Serial.print(t,5); //s
  Serial.print(" ");

  
  Serial.print(front_speed_v,5); //desired output value
  Serial.print(" ");

  Serial.print(e/PI*180,5); //deg
  Serial.print(" ");

  Serial.print(u,5); //pid output
  Serial.print(" ");

  Serial.print(ki_traction*ei,5); //integral check should be less then 0.2Vbat
  Serial.print(" ");
  
  return u;
  
  
  
}
