//Code based on Code provided by Dr. Brandon Gordon
#include <Arduino.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define BIT(a) (1 << (a))

void setup() 
{
  Serial.begin(115200);
  cli(); // stop all interrupt functions

  //Select ADC Channel by uncommenting.
  //uncomment as needed for A0 to A5
  ADMUX =0;    //A0 still dont know yet if this ones right
  //ADMUX |= BIT(MUX0); //A1
  //ADMUX |= BIT(MUX1); //A2
  //ADMUX |= BIT(MUX0) | BIT(MUX1); //A3
  //ADMUX |= BIT(MUX2); //A4
  //ADMUX |= BIT(MUX0) | BIT(MUX2); //A5
  ADMUX |= BIT(REFS0); // sets your ADC reference to 5V cause default is Aref pin
  ADCSRA=0; // sets your ADC Control and Status Register A to zero
  ADCSRA |=Bit(ADEN); //ADC enable on status register A
  //use a 128 prescaler (slowest)
  ADCSRA |= BIT(ADPS0) | BIT(ADPS1) | BIT(ADPS2);
  ADCSRA |= BIT(ADSC); // start ADC conversion
}

void loop() 
{

}
