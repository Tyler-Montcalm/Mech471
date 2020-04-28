//Written by: Thomas Savage (40016675)
//Written for the course MECH 471
//Data of final modification: 28th April 2020

//Receives a signal from "Transmitter" and translates that into commands to the motor and servo.
//See the library .cpp code for comments on how it works.

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
  cli();                    //Ensures that nothing in the setup phase is interrupted
  //Serial.begin(115200);   //Used for de-bugging. Would send information serially to the computer to
                            //check what the car was receiving.
  my_setup();               //A function from my own library.
  
  /*************RADIO SETUP*************/
  radio.begin();                      //Starting the radio process
  radio.openReadingPipe(0, address);  //set the address
  radio.setPALevel(RF24_PA_MIN);      //Setting the power level to minimum. May want to tinker with when running outside.
  radio.startListening();             //Set module as receiver.
  
  sei();              //Shuts off interrupts now that the setup is done.

  my_motor(0);        //Ensures that the ESC receives a command for 0 throttle. The delay ensures this is long enough for the ESC to arm.
  my_servo(0);        //Sets the servo to straight until a command is heard.
  delay(5000);
}

void loop()
{
  static const int NMAX = 64;
  static char data_buffer[NMAX];  //This is where I will store my incomming data
  static int data_length = 12;    //I expect to recieve 3 ints of 4 bytes each. My pointers need to be setup as long ints since they originate on a PC.
  static long int *q;             //Making a pointer to navigate my buffer and read it as a long int number.
  q = (long int *)data_buffer;    //Setting pointer to start of buffer
  static int angle;               //This is the actual number that will command my servo. This is a number between -30 and 30 (-30 being full left, 30 being full right, 0 being straight). 
                                  //Represents angle in degrees. Does not go to 90deg because the steering mechanism can't handle that.
  static int angle1;              //Used to store the previous value of angle so that I can prevent noise from making sudden changes.
  static int angle2;              //Used to store the current value of angle. This is what is compared to angle1.
  static int power;               //A number between -100 to 100 (100 being full forward, -100 being full reverse, 0 being no power). Represents the power demand in % power the motor can deliver.
  static int power1;              //Similar to angle1
  static int power2;              //Similar to angle2

  //radio.read() is put in a non-interrupt block so that the no corruption can happen. This was found to be necessary.
  cli();
  radio.read(&data_buffer, data_length);  //Reads incomming data and puts it into "data_buffer".
  sei();

  angle1 = angle2;  //Setting previous angle command to angle1.
  power1 = power2;  //Setting previous power command to power2.


  //Noise cancelling. "Transmitter" seems to generate noise due to the radio.write() command.
  //This noise tends to be WAAAYYYYYY outside the expected values (-1000 to 1000), so this cancells most of the erroneous messages.
  if((abs(q[0])<1000)&&(abs(q[2])<1000))  //Testing to see if either raw value is above the max possible values. Ignores values outside the max.
  {
    //Determining the angle and motor power from my data buffer.
    //NOTE: this seems silly now, but I will be changing this value with corrections from my controllers
    angle2 = (int)(q[0]*0.03);
    power2 = (int)(q[2]*0.1);
  }

  //Noise cancelling. Some of the noise can be within the expected bounds, but this noise only happens for one signal.
  //Thus, this prevents sudden changes. If I do actually mean to jump from ex: 10% to 80%, the first pass will not submit this command,
  //but the second pass will have recorded this 80%. Because I will demand 80% power for longer than one signal, comparing angle1 = 80%
  //and angle2 = 80% will allow the program to submit this command.
  if(abs(angle2-angle1)<10)
  {
    angle = angle2;
  }
  if(abs(power2-power1)<50)
  {
    power = power2;
  }
  
  
  /*Serial.print("\n");   //Trouble-shooting code not needed.
  Serial.print(q[0]);
  Serial.print("\t");
  Serial.print(q[2]);*/   
  

  delay(20);    //It was found that this delay allowed radio.read() to function properly. Without it nothing was received.
  
  my_servo(angle);  //Actually submitting my final, filtered commands
  my_motor(power);  //Actually submitting my final, filtered commands

}
