#include "my_register_level_magic.h"
#include "Arduino.h"
#include "avr/io.h"
#define BIT(a) (1 << (a)) //Allows easy bit manipulation and comparison (i.e. I can now switch bits on and off).

volatile long int servo_ticks = 3000;   //Global variable used to set OCR1A
volatile long int motor_ticks = 6000;   //Global variable used to set OCR1B and TCNT1
unsigned char SREG_BACKUP;

void my_setup()
{
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

  //Setting my timer to 0
  TCNT1 = 0;

  sei();

}

void my_motor(int power)
{
  SREG_BACKUP = SREG;
  cli();  
  motor_ticks = (power*20)+3000;  //power is an integer, so there should be no issues with non-integer types appearing
  SREG = SREG_BACKUP;
  //Designed to ensure 100% is actually 50% max power from motor
}

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

  SREG_BACKUP = SREG;
  cli();
  servo_ticks = (angle*22) + 3000;
  SREG = SREG_BACKUP;
}

ISR(TIMER1_COMPA_vect)
{
  PORTB &= ~(BIT(1)); //Ending my servo signal
  PORTB |= BIT(2);    //Starting my motor signal
}

ISR(TIMER1_COMPB_vect)
{
  PORTB &= ~(BIT(2));                     //Ending my motor signal
  
  SREG_BACKUP = SREG;
  cli();
  TCNT1 = 65535 - (40000 - motor_ticks);  //Ensuring my overflow interrupt occurs at 20ms after the servo pulse
  SREG = SREG_BACKUP;
}

ISR(TIMER1_OVF_vect)
{
  PORTB |= BIT(1);      //Starting my signal to the servo
  
  SREG_BACKUP = SREG;
  cli();
  OCR1A = servo_ticks;  //Setting my compare timers according to new commands. May want to do better
  OCR1B = motor_ticks;  //ATOMIC ACCESS
  SREG = SREG_BACKUP;
}
