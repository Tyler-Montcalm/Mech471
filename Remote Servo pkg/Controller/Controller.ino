//Controls the servo and remotely receives a signal from the "Transmitter"

#include <Arduino.h>
#include <avr/io.h> 
#define BIT(a) (1 << (a)) //Allows easy bit manipulation and comparison (i.e. I can now switch bits on and off).

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

volatile long int servo_ticks = 3000;   //Global variable used to set OCR1A
volatile long int motor_ticks = servo_ticks + 3000;   //Global variable used to set OCR1B and TCNT1

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

void setup() {
  cli();

  /*************DIGITAL PIN SETUP*************/
  
  //Setting up my pinmodes. Pin 9 and 10 are output.
  DDRB |= BIT(1) | BIT(2);

  //Setting the pins low to start
  PORTB &= ~(BIT(1));
  PORTB &= ~(BIT(2));

  /*************RADIO SETUP*************/

  radio.begin();
  
  //set the address
  radio.openReadingPipe(0, address);

  radio.setPALevel(RF24_PA_MAX);
  
  //Set module as receiver
  radio.startListening();
  
  /*************TIMER SETUP*************/

  //Setting my control registers to default value
  TCCR1A = 0;
  TCCR1B = 0;

  //Setting my prescalar to 8
  TCCR1B |= BIT(CS11);

  //Enabling my compare match for A and B and overflow
  TIMSK1 = 0;
  TIMSK1 |= BIT(OCIE1B) | BIT(OCIE1A) | BIT(TOIE1);
  
  //Setting my compare match value for B to 40000 ticks (20ms)
  OCR1B = 40000;

  //Setting my compare match value for A arbitratily to 3000 (1.5ms)
  OCR1A = 3000;

  //Clearing interrupt flags
  TIFR1 |= BIT(OCF1B) | BIT(OCF1A) | BIT(TOV1);

  //Setting my timer to 0
  TCNT1 = 0;
  
  sei();
}

void loop() {
  const int NMAX = 64;
  static char data_buffer[NMAX]; //Does "buffer" have a specific meaning?
  int data_length = 12;          //I expect to recieve 3 ints of 4 bytes each, therefore my pointers need to be setup as long ints.
  long int *q;                   //Making a pointer to navigate my buffer
  q = (long int *)data_buffer;   //Setting pointer to start of buffer
  
  //Read the data if available in buffer
  if (radio.available())
  {
    radio.read(&data_buffer, data_length);
  }

  //Setting my global variable to read the number of clock ticks until my next compare match
  servo_ticks = (2*q[0]) + 3000; //May want to move inside if{}. ATOMIC ACCESS!
  motor_ticks = servo_ticks + (3000 + q[2]);

}

ISR(TIMER1_COMPA_vect)
{
  //PORTB 1 = pin D9 (servo)
  //PORTB 2 = pin D10 (motor)
  PORTB &= ~(BIT(1)); //Ending my servo signal
  PORTB |= BIT(2);    //Starting my motor signal
}

ISR(TIMER1_COMPB_vect)
{
  PORTB &= ~(BIT(2));               //Ending my motor signal
  TCNT1 = 65535 - (40000 - OCR1B);  //Ensuring my overflow interrupt occurs at 20ms after the servo pulse
}

ISR(TIMER1_OVF_vect)
{
  PORTB |= BIT(1);      //Starting my signal to the servo
  OCR1A = servo_ticks;  //Setting my compare timers according to new commands. May want to do better
  OCR1B = motor_ticks;
}
