//Controls the servo and remotely receives a signal from the "Transmitter"

#include <Arduino.h>
#include <avr/io.h> 
#define BIT(a) (1 << (a)) //Allows easy bit manipulation and comparison (i.e. I can now switch bits on and off).

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

volatile long int ticks = 3000;  //Global variable used to set OCR1A

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

void setup() {
  cli();

  /*************DIGITAL PIN SETUP*************/
  
  //Setting up my pinmodes. Pin 9 is output.
  DDRB |= BIT(1);

  //Setting the pin low to start
  PORTB &= ~(BIT(1));

  /*************RADIO SETUP*************/

  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);

  radio.setPALevel(RF24_PA_MIN);
  
  //Set module as receiver
  radio.startListening();
  
  /*************TIMER SETUP*************/

  //Setting my control registers to default value
  TCCR1A = 0;
  TCCR1B = 0;

  //Setting my prescalar to 8
  TCCR1B |= BIT(CS11);

  //Enabling my compare match for A and B
  TIMSK1 = 0;
  TIMSK1 |= BIT(OCIE1B) | BIT(OCIE1A);
  
  //Setting my compare match value for B to 40000 ticks (20ms)
  OCR1B = 40000;

  //Setting my compare match value for A arbitratily to 3000 (1.5ms)
  OCR1A = 3000;

  //Clearing interrupt flags
  TIFR1 |= BIT(OCF1B) | BIT(OCF1A);

  //Setting my timer to 0
  TCNT1 = 0;
  
  sei();
}

void loop() {
  const int NMAX = 64;
  static char data_buffer[NMAX]; //Does "buffer" have a specific meaning?
  int data_length = 4;           //I expect to recieve 1 ints of 4 bytes each, therefore my pointers need to be setup as long ints.
  long int *q;              //Making a pointer to navigate my buffer
  q = (long int *)data_buffer;   //Setting pointer to start of buffer
  
  //Read the data if available in buffer
  if (radio.available())
  {
    radio.read(&data_buffer, data_length);
  }

  //Setting my global variable to read the number of clock ticks until my next compare match
  ticks = (2*q[0]) + 3000; //May want to move inside if{}. ATOMIC ACCESS!

}

ISR(TIMER1_COMPA_vect)
{
  //Here I need to set my pin to 0. This is the end of my pulse.
  PORTB &= ~(BIT(1));
}

ISR(TIMER1_COMPB_vect)
{
  //Here I end my signal and start my pulse.
  TCNT1 = 0;
  PORTB |= BIT(1);
  
  //Resetting OCR1A value. Moved this from ISR COMPA because changing OCR1A significantly would mean
  //ISR COMPA could run a second time before ISR COMPB (i.e. TCNT1 could equal OCR1A again after the 1st instance of the interrupt).
  OCR1A = ticks;
}
