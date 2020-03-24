//This is the transmitter in the remote servo package

//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  Serial.begin(115200);
  
  radio.begin();
  
  //set the address
  radio.openWritingPipe(address);

  radio.setPALevel(RF24_PA_MIN);
  
  //Set module as transmitter
  radio.stopListening();
}
void loop()
{
  const int NMAX = 64;
  static char data_buffer[NMAX];  //Does "buffer" have a specific meaning?
  int data_length = 4;            //I expect to recieve 1 ints of 4 bytes each, therefore my pointers need to be setup as long ints.
  int i = 0;                      //A do-nothing variable. Forces blocking
  int n = 0;                      //A record of the number of bytes received

  while(Serial.available() < data_length) i++;  //Forces blocking

  n = Serial.readBytes(data_buffer, data_length);

  if(n < data_length)                                //Makes sure all information is read. I expect 2 bytes, so I want to receive 2 bytes, else something is wrong.
  {
    Serial.print("Error: Not enough data, n = ");
    Serial.println(n);                          //Serial.println() prints a new line after the stuff in the brackets
  }
  
  radio.write(&data_buffer, data_length);       //Sends my actual message
}
