//Controls the servo and remotely receives a signal from the "Transmitter"


#include <Arduino.h>
#include <avr/io.h> 
#include "my_register_level_magic.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <math.h>

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  cli();
  Serial.begin(115200);
  my_setup();
  
  /*************RADIO SETUP*************/
  radio.begin();
  radio.openReadingPipe(0, address);  //set the address
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); //Set module as receiver
  
  sei();

  my_motor(0);
  my_servo(0);
  delay(5000);
}

void loop()
{
  static const int NMAX = 64;
  static char data_buffer[NMAX];  //Does "buffer" have a specific meaning?
  static int data_length = 12;    //I expect to recieve 3 ints of 4 bytes each, therefore my pointers need to be setup as long ints.
  static long int *q;             //Making a pointer to navigate my buffer
  q = (long int *)data_buffer;    //Setting pointer to start of buffer
  static int angle;
  static int angle1;              //A number between -30 and 30 (-30 being full left, 30 being full right, 0 being straight). Represents angle in degrees. Does not go to 90deg
                                  //because the steering mechanism can't handle that.
  static int angle2;
  static int power;
  static int power1;              //A number between -100 to 100 (100 being full forward, -100 being full reverse, 0 being no power). Represents the power demand in % power the motor can deliver.
  static int power2;

  static int count = 0;           //To guard against radio loss
  
    cli();
    radio.read(&data_buffer, data_length);    
    sei();

    angle1 = angle2;
   
    power1 = power2;


    if((abs(q[0])<1000)&&(abs(q[2])<1000))  //Noise cancelling. The noise is generally waaaaaaaay off the expected values.
    {
      //Determining the angle and motor power from my data buffer.
      //NOTE: this seems silly now, but I will be changing this value with corrections from my controllers
      angle2 = (int)(q[0]*0.03);
      power2 = (int)(q[2]*0.1);
    }

    if(abs(angle2-angle1)<10)
    {
      angle = angle2;
    }
    if(abs(power2-power1)<50)
    {
      power = power2;
    }
    
    
    /*Serial.print("\n");
    Serial.print(q[0]);
    Serial.print("\t");
    Serial.print(q[2]);*/   //Trouble-shooting code not needed.
    

    delay(20);
    
    my_servo(angle);
    my_motor(power);

}
