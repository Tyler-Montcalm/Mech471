#ifndef my_register_level_magic_h
#define my_register_level_magic_h

#include <Arduino.h>
#include <avr/io.h>

void my_setup();
void my_motor(int power);
void my_servo(int angle);
ISR(TIMER1_COMPA_vect);
ISR(TIMER1_COMPB_vect);
ISR(TIMER1_OVF_vect);

#endif
