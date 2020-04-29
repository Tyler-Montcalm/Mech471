//FINAL CONTROLLER CAN UPDATE. 
#include <Arduino.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define BIT(a) (1 << (a))

void PID(float wdesired);
void my_motor(int pwm);
void my_setup();
float adc_analog_read(int analog_port);
float v_a0,v_a1,v_a2;
float adc_analog_avg(int analog_port);
volatile long int motor_ticks = 3000;     //Global variable used to set OCR1B and TCNT1. Set to make motor stop (safe if no signal received or function call)
unsigned char SREG_BACKUP;                //Used to ensure I properly restore my interrupt state

// initial time of test
float t0;

void setup() 
{	
	// Serial startup sequence below ensures reliable / predictable startup /////
	char ch;
	
	Serial.begin(2000000);
	
   my_setup();
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


	t0 = micros()*1.0e-6; // initial time (s)


  while(1) PID(700); //Change this to change the desired control speed.

	delay(1000);
	exit(0); // not using loop()	
}

void PID(float wdesired){
  float y1 = -1, y2 = -1, y3 = -1;
  float wb, wf;
  static float u1 = 0;
  float pw1;
  float t;
  float errorpf = 0;
  float lasterrorf = 0;
  float errorif = 0;
  float errordf = 0;
  float errorpb = 0;
  float lasterrorb = 0;
  float errorib = 0;
  float errordb = 0;
  const int   PW_MAX = 2000, PW_0 = 1500, PW_MIN = 1000;
  const float wmax = 810.0; // maximum back wheel speed (rad/s)  
  const float V_bat = 12.0; // lipo battery voltage
  const float V_bat_inv = 1/V_bat;  
  static float lastt = t0;
  // read clock time (s)
  t = micros()*1.0e-6 - t0;
  
  
  y1 = adc_analog_avg(1);
  y2 = adc_analog_avg(3);
 
  
  // use the following scale for the output y1
  // w = 0  -> y1 = 2.5 V
  // w = wmax -> y1 = 5.0 V
  // y1 = 2.5 + w * wmax_inv * 2.5 ->
  // back wheel angular velocity (rad/s)
  wb = (y1 - 2.5) * 0.4 * wmax;

  // front wheel angular velocity (rad/s)
  wf = (y2 - 2.5) * 0.4 * wmax;
 if (t < 20){ //Calculates the errors for the PID control. 
  errorpf = wdesired-wf;
  errordf = (errorpf - lasterrorf)/(t-lastt);
  errorif += (lasterrorf)*(t-lastt) + (errorpf-lasterrorf)*(0.5)*(t-lastt);
  lasterrorf = errorpf;
  errorpb = wdesired-wb;
  errordb = (errorpb - lasterrorb)/(t-lastt);
  errorib += (lasterrorb)*(t-lastt) + (errorpb-lasterrorb)*(0.5)*(t-lastt);
  lasterrorb = errorpb;
  
  //This is the tuned PID controller values, it encorporates PID for the backwheel, aswell as an addition proportional factor for the front wheel. 
   u1 += errorpf*0.0005+errordf*0+errorif*0+errorpb*0.001+errordb*0.0005+errorib*0.001; 
 if (u1 > 12) u1 = 12; //Sets the voltage output to the max of 12 if controller brings it above that, this also allows more aggresive control values as they will be controlled by this in a sense.
 if (u1 < -12) u1 = -12; //Set the voltage output to the min of -12 if controller brings it below that.
  // calculate inputs
  lastt = t;
 }else{
  u1 = 0;
 }
 //Max and min does not need to be controller here for two reasons: max/min voltage is already set previously, and max/min pwm values are once again checked in the my_motor function.
 pw1 = u1 * V_bat_inv * (PW_MAX - PW_0) + PW_0; 

  
  // this sends a pwm signal from pin 7 equivalent to the value of pw1.
  my_motor(pw1);

  
  // print out for testing purposes - y3 not used, but left here in case you are using this format for your test.
  
  Serial.print(t);
  Serial.print(",");
  
  Serial.print(y1);
  Serial.print(",");
  
  Serial.print(y2);
  Serial.print(",");

  Serial.print(y3);
  Serial.print(",");  
  
  Serial.print(u1);
  Serial.print(",");  
  
  Serial.print(wb);
  Serial.print(",");    
  
  Serial.print(wf);
  Serial.print("\n"); 
  //Need this here or the analog read stops working properly
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
  
  delay(30);
}




float adc_analog_read(int analog_port)
{
  const float ADC_to_V = 1.0/1023.0*5;
  volatile unsigned int i=0;
  float Vin;
  volatile float adc1;
 
  //only set up to be used for pins 1 and 3, can include the rest of the pins but this reduces clutter in the code. 
  if(analog_port==1)
  {
  
  ADMUX |= BIT(MUX0);
  ADCSRA |= BIT(ADSC);
  while( ADCSRA & BIT(ADSC) ) i++;
  adc1 = ADC;
  Vin  = adc1*ADC_to_V;
  return Vin;
  }
  if(analog_port==3)
  {
  ADMUX |= B00000011; 
  ADCSRA |= BIT(ADSC);
  while( ADCSRA & BIT(ADSC) ) i++;
  adc1 = ADC;
  Vin  = adc1*ADC_to_V;
  return Vin;
  }
 
}

float adc_analog_avg(int analog_port)
{
  for(int i=0; i<200; i++)
  {
    v_a0+=adc_analog_read(analog_port);
  }
  v_a0=v_a0/200;
 // Serial.print("reading = ");
   //Serial.println(v_a0);
   return v_a0;
}


void loop()
{
	// not used
}

void my_setup() //Thomas wrote this, Ryan made it take a pwm input instead of a power input. 
{
  SREG_BACKUP = SREG; //Checking my current state of interrupt enable/disable
  cli();

  /*************DIGITAL PIN SETUP*************/
  
  //Setting up my pinmodes. Pin 7 for servo signal. 
  DDRD |= BIT(7);


  /*************TIMER SETUP*************/

  //Setting my control registers to default value
  TCCR1A = 0;
  TCCR1B = 0;

  //Setting my prescalar to 8
  TCCR1B |= BIT(CS11);

  //Enabling my compare match for A and B and overflow
  TIMSK1 = 0;
  TIMSK1 |= BIT(OCIE1B) | BIT(OCIE1A);
  
  //Setting my compare match value for B to servo_ticks (initially 3000 ticks or 1.5ms)
  OCR1A = motor_ticks;

  //Setting my compare match value for A arbitratily
  OCR1B = 40000;

  //Clearing interrupt flags
  TIFR1 |= BIT(OCF1B) | BIT(OCF1A);

  //Setting my timer to 0 so interrupts don't start too soon.
  TCNT1 = 0;

  SREG = SREG_BACKUP; //Resets my interrupt enable/disable to what it was in my main program.

}


//This receives the command for the pwm value to send. 
void my_motor(int pwm)
{
  //Making sure motor doesn't go over 100% forward or reverse.
  if(pwm > 2000)
  {
    pwm = 2000;
  }
  else if(pwm < 1000)
  {
    pwm = 1000;
  }
  //Provides atomic access since a 2 byte value.
  SREG_BACKUP = SREG;
  cli();  
  motor_ticks = (int)(pwm-1500)*2+3000;  //moter_ticks is an integer, so I need to ensure that this stays an integer.
  SREG = SREG_BACKUP;
  //Designed so that 100% is max the ESC will allow (which isn't 100% total motor power).
}

//For details on how this works, please see the report.
ISR(TIMER1_COMPA_vect)
{
 PORTD &= !BIT(7);
}

ISR(TIMER1_COMPB_vect)
{
  PORTD |= BIT(7);
  TCNT1 = 0;
  OCR1A = motor_ticks;
}
