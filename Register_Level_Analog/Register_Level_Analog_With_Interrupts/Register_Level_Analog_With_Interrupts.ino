//Code based on code provided by Dr. Brandon Gordon
//Code written by: Tyler Montcalm

#include <Arduino.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define BIT(a) (1<<(a))

float V_a0=0; //global variables for voltages of A0-A2 that will be changed by ISR and can be accessed any time 
float V_a1=0;
float V_a2=0;

void setup()
{
 Serial.begin(115200); // set serial communications at 115200 baud
 ADMUX =0; // set admux initially to a channel in this case A0 uncomment for others
 // when needing to change to another mid code add the line below corresponding to pin and then set initiate new adc
 //ADMUX |= BIT(MUX0); //A1
 //ADMUX |= BIT(MUX1); //A2
 //ADMUX |= BIT(MUX0) | BIT(MUX1); //A3
 //ADMUX |= BIT(MUX2); //A4
 //ADMUX |= BIT(MUX0) | BIT(MUX2); //A5
 ADMUX |=BIT(REFS0); // initially set adc reference (max) to 5v since Aref is base level
 ADCSRA=0;
 ADCSRA |= BIT(ADEN); //ADC enable
 ADCSRA |= BIT(ADIE); //ADC interrupt enable
 ADCSRA |= BIT(ADPS0) | BIT(ADPS1) |BIT(ADPS2); // sets prescaler to 128 
 ADCSRA |= BIT(ADSC); //initial adc conversion to get it going
 sei();
}

void loop() 
{
 
delay(500);
}

ISR(ADC_vect)
{
 static count1;
 static int i=0;
 int adc1;
 static float V_a0_avg;
 ADCSRA |= BIT(ADSC); 
 i++;
 adc1=ADC;
 V_a0_avg+=adc1;
 count++;
 if(count++>100)
 {
  V_a0=V_a0_avg/100;
 }
}
