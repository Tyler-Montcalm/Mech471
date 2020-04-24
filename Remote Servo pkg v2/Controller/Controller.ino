//Controls the servo and remotely receives a signal from the "Transmitter"


#include <Arduino.h>
#include <avr/io.h> 
#include "my_register_level_magic.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  cli();

  my_setup();
  
  /*************RADIO SETUP*************/
  radio.begin();
  radio.openReadingPipe(0, address);  //set the address
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); //Set module as receiver
  
  sei();
}

void loop()
{
  const int NMAX = 64;
  static char data_buffer[NMAX]; //Does "buffer" have a specific meaning?
  int data_length = 12;          //I expect to recieve 3 ints of 4 bytes each, therefore my pointers need to be setup as long ints.
  long int *q;                   //Making a pointer to navigate my buffer
  q = (long int *)data_buffer;   //Setting pointer to start of buffer
  int angle;
  int power;
  
  //Read the data if available in buffer
  if (radio.available())
  {
    radio.read(&data_buffer, data_length);
  }

  //Determining the angle and motor power from my data buffer.
  //NOTE: this seems silly now, but I will be changing this value with corrections from my controllers
  angle = (int)(q[0]*0.09);
  power = (int)(q[2]*0.1);
  
  //Setting my global variable to read the number of clock ticks until my next compare match 
  my_servo(angle);
  my_motor(power);

}
