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
  static int data_length = 12;           //I expect to recieve 3 ints of 4 bytes each, therefore my pointers need to be setup as long ints.
  static long int *q;                    //Making a pointer to navigate my buffer
  q = (long int *)data_buffer;    //Setting pointer to start of buffer
  static int angle;                      //A number between -30 and 30 (-30 being full left, 30 being full right, 0 being straight). Represents angle in degrees. Does not go to 90deg
                                  //because the steering mechanism can't handle that.
  int power;                      //A number between -100 to 100 (100 being full forward, -100 being full reverse, 0 being no power). Represents the power demand in % power the motor can deliver.
  static int count = 0;           //To guard against radio loss
  bool found;                     //For syncing data
  int index;                      //For syncing data
  char search_char;               //For syncing data

  //found = false;
  //index = 0;
  //search_char = '1';              //Setting to something that isn't my start marker.
  
  //Read the data if available in buffer
  //if (radio.available())
  //{
    cli();
    radio.read(&data_buffer, data_length);

    //Syncing that data is necessary. This is done by sending the message twice over (see the visual studio code) as that guarantees
    //that the entire message will be found if the start marker is found.
    //To find the message, "search_char" checks each character of "data_buffer" until it finds the start marker (newline).
    //There is a small chance that the newline character is generated by the data itself (being an int read as a char), but testing shows this to be
    //so infrequent that it is not worth worrying about in this iteration of the code.
    //Once the start marker is found, my long int pointer is set to the next char.
    //while(!found)
     //{
      /*search_char = data_buffer[index];
      if(search_char != '\n')
      {
        index++;
        Serial.print("\n");
        Serial.print(index);
      }*/
     /* else
      {
        q = (long int*)&(data_buffer[index+1]);
        found = true;
        /*Serial.print("\n");
        Serial.print(q[0]);
        Serial.print("\t");
        Serial.print(q[2]);
      }*/
     //}
    
    //Determining the angle and motor power from my data buffer.
    //NOTE: this seems silly now, but I will be changing this value with corrections from my controllers
    angle = (int)(q[0]*0.09);
    power = (int)(q[2]*0.1);
    Serial.print("\n");
    Serial.print(q[0]);
    Serial.print("\t");
    Serial.print(q[2]);
    sei();

    delay(50);
    
    my_servo(angle);
    my_motor(power);

    //count = 0;
  /*}
  else if(count > 100)
  {
    my_motor(0);
    my_servo(0);
  }
  else
  {
    count++;
  }*/
  //}

}
