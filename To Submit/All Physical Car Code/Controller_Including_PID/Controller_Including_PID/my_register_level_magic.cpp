//Written by: Thomas Savage (40016675)
//Written for the course MECH 471
//Data of final modification: 28th April 2020

//Does the behind-the-scenes register-level work to command my motor and servo.

//Pin 9 goes to Servo
//Pin 10 goes to Motor

#include "my_register_level_magic.h"
#include "Arduino.h"
#include "avr/io.h"
#define BIT(a) (1 << (a)) //Allows easy bit manipulation and comparison (i.e. I can now switch bits on and off).

volatile long int servo_ticks = 3000;     //Global variable used to set OCR1A. Set to make servo stop (safe if no signal received or function call)
volatile long int motor_ticks = 3000;     //Global variable used to set OCR1B and TCNT1. Set to make motor stop (safe if no signal received or function call)
volatile int TCNT1_change_value = 31535;  //Adjusts my timer to ensure one signal (pulse and off period) measure 20ms
unsigned char SREG_BACKUP;                //Used to ensure I properly restore my interrupt state

//This sets up the timer registers, output pins, and interrupts.
void my_setup()
{
  SREG_BACKUP = SREG; //Checking my current state of interrupt enable/disable
  cli();

  /*************DIGITAL PIN SETUP*************/
  
  //Setting up my pinmodes. Pin 9 and 10 are output.
  DDRB |= BIT(1) | BIT(2);

  //Setting the pins low to start
  PORTB &= ~(BIT(1));
  PORTB &= ~(BIT(2));

  /*************TIMER SETUP*************/

  //Setting my control registers to default value
  TCCR1A = 0;
  TCCR1B = 0;

  //Setting my prescalar to 8
  TCCR1B |= BIT(CS11);

  //Enabling my compare match for A and B and overflow
  TIMSK1 = 0;
  TIMSK1 |= BIT(OCIE1B) | BIT(OCIE1A) | BIT(TOIE1);
  
  //Setting my compare match value for B to servo_ticks (initially 3000 ticks or 1.5ms)
  OCR1A = servo_ticks;

  //Setting my compare match value for A arbitratily
  OCR1B = motor_ticks;

  //Clearing interrupt flags
  TIFR1 |= BIT(OCF1B) | BIT(OCF1A) | BIT(TOV1);

  //Setting my timer to 0 so interrupts don't start too soon.
  TCNT1 = 0;

  SREG = SREG_BACKUP; //Resets my interrupt enable/disable to what it was in my main program.

}


//This receives the command for how much power must be delivered to my motor.
void my_motor(int power)
{
  //Making sure motor doesn't go over 100% forward or reverse.
  if(power > 100)
  {
    power = 100;
  }
  else if(power < -100)
  {
    power = -100;
  }
  //Provides atomic access since a 2 byte value.
  SREG_BACKUP = SREG;
  cli();  
  motor_ticks = (int)(power*2.6)+3000;  //moter_ticks is an integer, so I need to ensure that this stays an integer.
  SREG = SREG_BACKUP;
  //Designed so that 100% is max the ESC will allow (which isn't 100% total motor power).
}

//Similar to my_motor()
void my_servo(int angle)
{
  //Making sure servo doesn't turn beyond 30 deg either way (mechanism can't handle it)
  if(angle > 30)
  {
    angle = 30;
  }
  else if(angle < -30)
  {
    angle = -30;
  }
  //Provides atomic access since a 2 byte value
  SREG_BACKUP = SREG;
  cli();
  servo_ticks = (1*angle*22) + 3000;
  SREG = SREG_BACKUP;
}


//For details on how this works, please see the report.
ISR(TIMER1_COMPA_vect)
{
  PORTB &= ~(BIT(1)); //Ending my servo signal
  PORTB |= BIT(2);    //Starting my motor signal
}

ISR(TIMER1_COMPB_vect)
{
  PORTB &= ~(BIT(2)); //Ending my motor signal
  
  SREG_BACKUP = SREG;
  cli();
  TCNT1 = TCNT1_change_value;  //Ensuring my overflow interrupt occurs at 20ms after the servo pulse
  SREG = SREG_BACKUP;
}

ISR(TIMER1_OVF_vect)
{
  PORTB |= BIT(1);      //Starting my signal to the servo
  OCR1A = servo_ticks;
  OCR1B = servo_ticks + motor_ticks;
  TCNT1_change_value = 65535 - (40000 - OCR1B);
}
