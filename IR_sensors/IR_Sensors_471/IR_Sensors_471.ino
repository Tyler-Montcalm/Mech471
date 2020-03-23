//3 sides obstacle detection code for mech 471 
//written by: Tyler Montcalm

///////////INCLUDES////////////
#include <Servo.h>
//////////PIN CALLOUTS////////////
int ir_1=4;
int ir_2=5;
int ir_3=6;
int st_pin=3;

////////GLOBAL VARIABLES IR////////
int sensorVal1;
int sensorVal2;
int sensorVal3; 

///////GLOBAL VARIABLES STEERING//////
Servo Servo_st;
int angle=0;
/////////FUNCTIONS////////////
void ir_setup();
void ir_read();
void ir_print();
void steering_setup();
void st_sweep();



void setup()
{
  Serial.begin(9600);
  ir_setup();
  steering_setup();
}

void loop() 
{
 ir_read();
 ir_print();
 //st_sweep();
 delay(500);
}

void ir_setup()
{
  pinMode(ir_1, INPUT_PULLUP);
  pinMode(ir_2, INPUT_PULLUP);
  pinMode(ir_3, INPUT_PULLUP);
}

void ir_read()
{
  sensorVal1 = digitalRead(ir_1);
  sensorVal2 = digitalRead(ir_2);
  sensorVal3 = digitalRead(ir_3);
}

void ir_print()
{
 Serial.print("sensorVal1 =");
 Serial.println(sensorVal1);
 Serial.print("sensorVal2 =");
 Serial.println(sensorVal2);
 Serial.print("sensorVal3 =");
 Serial.println(sensorVal3);
}

void steering_setup()
{
  Servo_st.attach(st_pin);
  Servo_st.write(angle);
}

void st_sweep()
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
