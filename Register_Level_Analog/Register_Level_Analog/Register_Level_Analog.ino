//Code based on Code provided by Dr. Brandon Gordon
//Code written by: Tyler Montcalm
#include <Arduino.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define BIT(a) (1 << (a))
float adc_analog_read(int analog_port);
float v_a0,v_a1,v_a2;
float adc_analog_avg(int analog_port);
void setup_reccuring();
void setup() 
{
  Serial.begin(115200);
  cli(); // stop all interrupt functions

  //Select ADC Channel by uncommenting. be careful about changing channel during conversion process
  //uncomment as needed for A0 to A5
  ADMUX =0;    //Initially set mux register to zero A0
  //ADMUX |= BIT(MUX0); //A1
  //ADMUX |= BIT(MUX1); //A2
  //ADMUX |= BIT(MUX0) | BIT(MUX1); //A3
  //ADMUX |= BIT(MUX2); //A4
  //ADMUX |= BIT(MUX0) | BIT(MUX2); //A5
  ADMUX |= BIT(REFS0); // sets your ADC reference to 5V cause default is Aref pin
  ADCSRA=0; // sets your ADC Control and Status Register A to zero
  ADCSRA |= BIT(ADEN); //ADC enable on status register A
  //use a 128 prescaler (slowest)
  ADCSRA |= BIT(ADPS0) | BIT(ADPS1) | BIT(ADPS2);
  ADCSRA |= BIT(ADSC); // start ADC conversion
  //BIT(ADSC) will be 1 whenever a conversion is happening and be 0 when complete to read a bit you and it
  sei();

}

void loop()
{ 
   Serial.println(adc_analog_read(1));
   delay(300);
   Serial.println(adc_analog_read(0));
   delay(300);
   setup_reccuring();
}
float adc_analog_read(int analog_port)
{
  volatile unsigned int i=0;
  static float dt=0.0,t1,t2,Vin,Vref;
  volatile int adc1;
  t1=micros(); // start time for the conversion
  if(analog_port==0)
  {  
  ADCSRA |= BIT(ADSC);
  while( ADCSRA & BIT(ADSC) ) i++;
  adc1 = ADC;
  Vref = 5.0;
  Vin  = adc1/1023.0*Vref;
  return Vin;
  }
  if(analog_port==1)
  {
  ADMUX |= BIT(MUX0);
  ADCSRA |= BIT(ADSC);
  while( ADCSRA & BIT(ADSC) ) i++;
  adc1 = ADC;
  Vref = 5.0;
  Vin  = adc1/1023.0*Vref;
  return Vin;
  }
  if(analog_port==2)
  {
  ADMUX |= BIT(MUX1);
  ADCSRA |= BIT(ADSC);
  while( ADCSRA & BIT(ADSC) ) i++;
  adc1 = ADC;
  Vref = 5.0;
  Vin  = adc1/1023.0*Vref;
  return Vin;
  }
  if(analog_port==3)
  {
  ADMUX |= BIT(MUX0) | BIT(MUX1); 
  ADCSRA |= BIT(ADSC);
  while( ADCSRA & BIT(ADSC) ) i++;
  adc1 = ADC;
  Vref = 5.0;
  Vin  = adc1/1023.0*Vref;
  return Vin;
  }
  if(analog_port==4)
  {
  ADMUX |= BIT(MUX2);
  ADCSRA |= BIT(ADSC);
  while( ADCSRA & BIT(ADSC) ) i++;
  adc1 = ADC;
  Vref = 5.0;
  Vin  = adc1/1023.0*Vref;
  return Vin;
  }
  if(analog_port==5)
  {
  ADMUX |= BIT(MUX0) | BIT(MUX2);
  ADCSRA |= BIT(ADSC);
  while( ADCSRA & BIT(ADSC) ) i++;
  adc1 = ADC;
  Vref = 5.0;
  Vin  = adc1/1023.0*Vref;
  return Vin;
  }
 
}
void setup_reccuring()
{
  cli(); // stop all interrupt functions

  //Select ADC Channel by uncommenting. be careful about changing channel during conversion process
  //uncomment as needed for A0 to A5
  ADMUX =0;    //Initially set mux register to zero A0
  //ADMUX |= BIT(MUX0); //A1
  //ADMUX |= BIT(MUX1); //A2
  //ADMUX |= BIT(MUX0) | BIT(MUX1); //A3
  //ADMUX |= BIT(MUX2); //A4
  //ADMUX |= BIT(MUX0) | BIT(MUX2); //A5
  ADMUX |= BIT(REFS0); // sets your ADC reference to 5V cause default is Aref pin
  ADCSRA=0; // sets your ADC Control and Status Register A to zero
  ADCSRA |= BIT(ADEN); //ADC enable on status register A
  //use a 128 prescaler (slowest)
  ADCSRA |= BIT(ADPS0) | BIT(ADPS1) | BIT(ADPS2);
  ADCSRA |= BIT(ADSC); // start ADC conversion
  //BIT(ADSC) will be 1 whenever a conversion is happening and be 0 when complete to read a bit you and it
  sei();
}
float adc_analog_avg(int analog_port)
{
  for(int i=0; i<50; i++)
  {
    v_a0+=adc_analog_read(analog_port);
  }
  v_a0=v_a0/50;
   return v_a0;
}
