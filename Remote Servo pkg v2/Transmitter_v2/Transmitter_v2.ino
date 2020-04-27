
#include <Arduino.h>
#include <avr/io.h> 
#define BIT(a) (1 << (a)) //Allows easy bit manipulation and comparison (i.e. I can now switch bits on and off).

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  const int NMAX = 64;
  static char data_in[NMAX];  //Does "buffer" have a specific meaning?
  int data_in_length = 26;    //I expect to recieve 2x 3 ints of 4 bytes each, plus 2 chars. My pointers need to be setup as long ints.
  long int *q;                //Making a pointer to navigate my buffer
  //q = (long int *)data_in;  //Setting pointer to start of buffer
  char search_char = '1';
  bool found = false;
  int index = 0;

  int n = 0;                //Setting up an integer to record the number of bytes in my buffer
  int i = 0;                //A do-nothing variable. Is it really necessary?
  //long int power = 0;
  //long int angle = 0;

  found = false;
  index = 0;
  while(Serial.available() < data_in_length) i++; 
  cli();
  n = Serial.readBytes(data_in, data_in_length); //Symultaneously reads bits from serial buffer to "buffer" AND returns #bytes in serial buffer
  sei();

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
      found = true;
    }
  }

  Serial.print(q[0]);
  Serial.print("\t");
  Serial.print(q[2]);
  Serial.print("\n");


}
