#include "my_register_level_magic.h"
#include <Arduino.h>
#include <avr/io.h> 
#define BIT(a) (1 << (a)) //Allows easy bit manipulation and comparison (i.e. I can now switch bits on and off).

void setup() {
  Serial.begin(115200);
  my_setup();
  my_motor(0);
  my_servo(0);
  delay(1000);
}

void loop() {
  const int NMAX = 64;
  static char data_buffer[NMAX];  //Does "buffer" have a specific meaning?
  int length = 26;                //I expect to recieve 2x 3 ints of 4 bytes each, plus 2 chars. My pointers need to be setup as long ints.
  long int *q;                    //Making a pointer to navigate my buffer
  //q = (long int *)data_buffer;  //Setting pointer to start of buffer
  char search_char = '1';
  bool found = false;
  int index = 0;

  int n = 0;                //Setting up an integer to record the number of bytes in my buffer
  int i = 0;                //A do-nothing variable. Is it really necessary?
  //long int power = 0;
  //long int angle = 0;

  found = false;
  index = 0;
  while(Serial.available() < length) i++; 
  cli();
  n = Serial.readBytes(data_buffer, length); //Symultaneously reads bits from serial buffer to "buffer" AND returns #bytes in serial buffer
  sei();

  while(!found/*(index < length)&&(found == false)*/)
  {
    search_char = data_buffer[index];
    if(search_char != '\n')
    {
      index++;
    }
    else
    {
      q = (long int*)&(data_buffer[index+1]);
      found = true;
    }
  }
    

  //angle = (int)(q[0]*0.09);
  //power = (int)(q[2]*0.1);
  
  my_motor(q[2]/10);
  Serial.print("\t\t");
  Serial.print(q[2]);
  Serial.print("\n");
  //my_servo(angle);
  //delay(500);

}
