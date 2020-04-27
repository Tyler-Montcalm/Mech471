
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
  static char data_in[NMAX];  //Does "buffer" have a specific meaning?
  int data_in_length = 26;    //I expect to recieve 2x 3 ints of 4 bytes each, plus 2 chars. My pointers need to be setup as long ints.
  int data_out_length = 12;
  long int *q;                //Making a pointer to navigate my buffer
  char *data_out;
  char search_char = '1';
  bool found = false;
  int index = 0;

  int n = 0;                //Setting up an integer to record the number of bytes in my buffer
  int i = 0;                //A do-nothing variable. Is it really necessary?


  found = false;
  index = 0;
  while(Serial.available() < data_in_length) i++; 
  cli();
  n = Serial.readBytes(data_in, data_in_length); //Symultaneously reads bits from serial buffer to "buffer" AND returns #bytes in serial buffer
  

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

  radio.write(data_out, data_out_length);       //Sends my actual message
  
  Serial.print(q[0]);
  Serial.print("\t");
  Serial.print(q[2]);
  Serial.print("\n");
  

  
  sei();
}
