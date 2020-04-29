//Written by: Thomas Savage (40016675)
//Written for the course MECH 471
//Last modified: 28th April 2020

//This receives commands from the computer serially, sorts it, and then sends it to the "Controller".


#include <Arduino.h>
#include <avr/io.h> 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define BIT(a) (1 << (a)) //Allows easy bit manipulation and comparison (i.e. I can now switch bits on and off).

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

int count = 0;

void setup() {
  Serial.begin(115200);
  radio.begin();
  
  //set the address
  radio.openWritingPipe(address);

  radio.setPALevel(RF24_PA_MIN);
  
  //Set module as transmitter
  radio.stopListening();
}

void loop() {
  const int NMAX = 64;
  static char data_in[NMAX];  //Buffer that receives the incomming serial message.
  int data_in_length = 26;    //I expect to recieve 2x 3 ints of 4 bytes each, plus 2 chars. My pointers need to be setup as long ints.
                              //(It's the message twice over, so I'm guaranteed that if I find the start character I'll have the whole message)
  int data_out_length = 12;   //I'm sending 3 ints of 4 bytes each.
  long int *q;                //Making a pointer to navigate my data_in buffer as a long int (used for confirning correct data was sent by printing in the computer terminal)
  char *data_out;             //A character pointer to be correctly positioned in my data_in buffer (will send message from that location in the buffer).
  char search_char = '1';     //A variable to read my data_in and find my start character (which is \n). Initialized to something other than \n
  bool found = false;         //Returns the state of my search for the start character.
  int index = 0;              //Returns the byte location of my message start

  int n = 0;                //Setting up an integer to record the number of bytes in my buffer
  int i = 0;                //A do-nothing variable. Is it really necessary? (Maybe not)


  found = false;
  index = 0;
  while(Serial.available() < data_in_length) i++; 

  //All of this is inside non interrupt code. Might only need to keep serial.readbytes() in this, but it works.
  cli();
  n = Serial.readBytes(data_in, data_in_length);  //Symultaneously reads bits from serial buffer to "buffer" AND returns #bytes in serial buffer
                                                  //Was going to make use of "n" to track whether I received my message correctly, but I didn't yet implement that.

  //The search function for finding the start character.
  //Will compare each character in my data_in (and update index) until it finds \n. Then it will start 
  //both my pointers (long int and char) on the next byte.
  //It was found that if the transmitting Arduino was reset, the data was received out of sync. This guards against that and has been proven to do so.
  while(!found)
  {
    search_char = data_in[index];
    if(search_char != '\n')
    {
      index++;
    }
    else
    {
      q = (long int*)&(data_in[index+1]);
      data_out = &(data_in[index+1]);
      found = true;
    }
  }

  radio.write(data_out, data_out_length);       //Sends my actual message. Does not need to have the same precaution as the serial.read()
                                                //due to the background checks by the nRF library (i.e. only a complete message is sent over the radio).

  //Prints to the terminal on the computer so the user can confirm the correct data is being received and sent.
  Serial.print(q[0]);   //Gets Left joystick sideways motion
  Serial.print("\t");
  Serial.print(q[2]);   //Gets LT and RT button input
  Serial.print("\n");
  
  sei();

}
