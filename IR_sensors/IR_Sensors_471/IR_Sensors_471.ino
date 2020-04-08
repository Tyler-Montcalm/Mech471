//3 sides obstacle detection code for mech 471 
//written by: Tyler Montcalm

///////////INCLUDES////////////
#include <Servo.h>
//////////PIN CALLOUTS////////////
int ir_1=4;
int ir_2=5;
int ir_3=6;
int st_pin=3;
int slow_back=10;
int slow_forward=10;
int left=-30;
int right=+30;
int middle=0;

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
void ir_auto();


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
 ir_auto();
 //st_sweep();
 delay(500);
}

void ir_auto()
{
  //scenarios:
  // 1: 000 // stuck in a dead end.. reverse a bunch and turn right DONE
  // 2: 001 // reverse and turn right DONE
  // 3: 010 // stuck in a tunnel.. reverse a bunch and turn right DONE
  // 4: 011 // reverse out and go right DONE
  // 5: 100 // reverse and turn left DONE
  // 6: 101 // reverse and turn left DONE
  // 7: 110 // reverse out and go left DONE
  // 8: 111 // continue as normal DONE
  
   //scenario 1 stuck in a dead end.. reverse a bunch and turn right
  if(sensorVal1==0 && sensorVal2==0 && sensorVal3==0) 
  {
    Serial.println("scenario 1");
    my_servo(middle); 
    my_motor(slow_back);
    delay(1000);
    my_servo(right);
    my_motor(slow_forward);
    delay(500);
    my_servo(middle);
  }
  
  //scenario 2 reverse and turn right
  if(sensorVal1==0 && sensorVal2==0 && sensorVal3==1) 
  {
    Serial.println("scenario 2");
    my_servo(left); 
    my_motor(slow_back);
    delay(500);
    my_servo(right);
    my_motor(slow_forward);
    delay(500);
    my_servo(middle);
  }
  //scenario 3 stuck in a tunnel.. reverse a bunch and turn right
  if(sensorVal1==0 && sensorVal2==1 && sensorVal3==0) 
  {
    Serial.println("scenario 3");
    my_servo(middle); 
    my_motor(slow_back);
    delay(1000);
    my_servo(right);
    my_motor(slow_forward);
    delay(500);
    my_servo(middle);
  }
  //scenario 4 just left sensor
  if(sensorVal1==0 && sensorVal2==1 && sensorVal3==1) 
  {
    Serial.println("scenario 4");
    my_servo(right); 
    my_motor(slow_back);
    delay(500);
    my_servo(middle);
    my_motor(slow_forward);
    delay(500);
    my_servo(left);
    my_motor(slow_forward);
    delay(500);
    my_servo(middle);
  }
 //scenario 5 reverse and turn left
  if(sensorVal1==1 && sensorVal2==0 && sensorVal3==0) 
  {
    Serial.println("scenario 5");
    my_servo(right); 
    my_motor(slow_back);
    delay(500);
    my_servo(left);
    my_motor(slow_forward);
    delay(500);
    my_servo(middle);
  }
  //scenario 6 reverse and turn right
  if(sensorVal1==1 && sensorVal2==0 && sensorVal3==1) 
  {
    Serial.println("scenario 6");
    my_servo(left); 
    my_motor(slow_back);
    delay(500);
    my_servo(right);
    my_motor(slow_forward);
    delay(500);
    my_servo(middle);
  }
  //scenario 7 just right sensor
  if(sensorVal1==1 && sensorVal2==1 && sensorVal3==0) 
  {
    Serial.println("scenario 7");
    my_servo(left); 
    my_motor(slow_back);
    delay(500);
    my_servo(middle);
    my_motor(slow_forward);
    delay(500);
    my_servo(right);
    my_motor(slow_forward);d
    delay(500);
    my_servo(middle);
  }
  //scenario 8 continue as normal
if(sensorVal1==0 && sensorVal2==0 && sensorVal3==0) 
  {
    Serial.println("scenario 8");
    return;
  }
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
