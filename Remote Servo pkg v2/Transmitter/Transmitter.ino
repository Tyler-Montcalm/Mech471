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
  static char data_out[NMAX];
  int data_length = 26;           //I expect to recieve 2x 3 ints of 4 bytes each and 2 chars. My pointers need to be setup as long ints.
  int data_out_length = 12;
  char *q;
  long int *ip = (long int*)q;
  int i = 0;                      //A do-nothing variable. Forces blocking. Could be useful for determining an error since I send serially continuously.
  int n = 0;                      //A record of the number of bytes received
  char search_char = '1';
  bool found = false;
  int index = 0;

  found = false;
  index = 0;

  while(Serial.available() < data_length) i++; 
  
  n = Serial.readBytes(data_buffer, data_length); //Symultaneously reads bits from serial buffer to "buffer" AND returns #bytes in serial buffer

  while(!found)
  {
    search_char = data_buffer[index];
    if(search_char != '\n')
    {
      index++;
    }
    else
    {
      q = &(data_buffer[index+1]);
      found = true;
    }
  }

  for(int i=0; i<data_out_length; i++)
  {
    data_out[i] = q[i]; 
  }
  
  Serial.print(ip[0]);
  Serial.print("\t");
  Serial.print(ip[2]);
  Serial.print("\n");
  
  radio.write(&data_out, data_out_length);       //Sends my actual message
}
