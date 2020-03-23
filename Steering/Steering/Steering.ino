#include <Servo.h>

Servo Servo_st;
int angle=0;
void setup()
{
  Serial.begin(9600);
  Servo_st.attach(3);
  Servo_st.write(angle);
}

void loop()
{
  for(angle = 10; angle < 180; angle++)  
  {                                  
    Servo_st.write(angle);               
    delay(15);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 10; angle--)    
  {                                
    Servo_st.write(angle);           
    delay(15);       
  } 
}
