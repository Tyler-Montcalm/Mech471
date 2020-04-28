//FINAL CONTROLLER CAN UPDATE. (Y1 = Voltage from back wheel,
#include <Servo.h>


void acceleration(float ds);
Servo servo1, servo2;

float read_ADC_voltage(int channel, int n);

// initial time of test
float t0;

void setup() 
{	
	// Serial startup sequence below ensures reliable / predictable startup /////
	char ch;
	
	Serial.begin(2000000);
	
	// wait until Serial Monitor or save_serial program is started
	while( !Serial ) delay(1);
	
	// note: the following code is the best way to start an Arduino 
	// program in my opinion since you can control the start time exactly
	// and you can wait until you are ready to watch the experiment.
//	while(1) {
	////	if( Serial.available() > 0 ) { // is there incoming data
		//	ch = Serial.read(); // read one character / byte
		//	if( ch == 's' ) break;
	//	}
	//	delay(10); // leave some time for printing message above, etc.
	//}
	
	// give some time for user to look at robot
	//delay(3000);
	
	// NOTE: you can try using the following line here to make sure each line 
	// is completely output before continuing -- this might make the 
	// output more reliable
	// Serial.flush() // completely write serial output before continuing		
	
	// end of Serial startup sequence ///////////////////////////////////	

	// I recommend using pins 7 and 8 for the servo pins
	// -- you should avoid using pins 5, 6, and 11 which
	// are used by the car simulator board for analog output.
	
	// you should also avoid using pins 2 and 3 to avoid
	// potential conflict with the simulator board.
	
	servo1.attach(7); 
	servo2.attach(8);

	t0 = micros()*1.0e-6; // initial time (s)

	//while(1) task1();
  while(1) Acceleration(3);

	delay(1000);
	exit(0); // not using loop()	
}

void Acceleration(float ds){
  int n;
  float y1 = -1, y2 = -1, y3 = -1;
  float wb, wf;
  static float u1 = 1;
  int pw1, pw2;
  int w1, w2; 
  float t;
  float slipratio;
  float sliperror = 0;
  float lastsliperror = 0;
  float sliperrori = 0;
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
  
  // read car outputs ///////////////

  // note that y1 (drive motor), y2 (right front wheel), and 
  // y3 (left front wheel) are the outputs for the car system

  // *** I recommend using pins A1, A3, and A5 for analog inputs ***
  
  n = 200; // number of ADC samples in averaging filter
  
  y1 = read_ADC_voltage(A1,n);
  y2 = read_ADC_voltage(A3,n);
  //y3 = read_ADC_voltage(A5,n);
  slipratio = (y1-y2)/(abs(y2)+0.0003);
  sliperror = ds*0.01-slipratio;
  sliperrori += (lastsliperror)*(t-lastt) + (sliperror-lastsliperror)*(0.5)*(t-lastt);
  lastsliperror = sliperror;
  float controlinput = sliperror*2+sliperrori*7;
  if(controlinput > 0.1){
  u1+= controlinput;
  }else{
    //It appears that even if slip is overshot, it's better to let the front speed catch up rather than lower voltage to maximize acceleration. As such no change in voltage is applied if slip is too high.
    //Using a lower proportional value then for the positive slip error was also attempted, but was also worse than having no change in voltage. 
    //u1+= (ds*0.01-slipratio);
   //u1+= 20*sliperrori;
   u1 += 0.1+controlinput*1.5;  
  }
  lastt = t;
  
  // use the following scale for the output y1
  // w = 0  -> y1 = 2.5 V
  // w = wmax -> y1 = 5.0 V
  // y1 = 2.5 + w * wmax_inv * 2.5 ->
  // back wheel angular velocity (rad/s)
  wb = (y1 - 2.5) * 0.4 * wmax;

  // front wheel angular velocity (rad/s)
  wf = (y2 - 2.5) * 0.4 * wmax;
 if (t > 20){
  u1 = 0;
 }
 // u1 = errorp*4+errori*0.01+errord*0.5;
 // u1 = 6+errorp*.4+2*errori + 0.1*errord;
  

  // convert inputs to actuator values pw1, pw2

  // convert motor voltage to pulse width
  // u1 = (w1 - PW_0) * PW_R * V_bat ->
  if(u1 > 12) u1 = 12;
  if(u1 < -12) u1 = -12;
  w1 = u1 * V_bat_inv * (PW_MAX - PW_0) + PW_0; 
  
  // saturate input if out of range
  if(w1 > PW_MAX) w1 = PW_MAX;
  if(w1 < PW_MIN) w1 = PW_MIN;  
  
  pw1 = w1;
  
//  if(w2 > PW_MAX) w2 = PW_MAX;
//  if(w2 < PW_MIN) w2 = PW_MIN;

  // set pw2 for testing purposes
  pw2 = 1750;
  
  // set car actuators
  servo1.writeMicroseconds(pw1);
  servo2.writeMicroseconds(pw2);
  
  // print out for testing purposes
  
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
  
  delay(30);
}


float read_ADC_voltage(int channel, int n)
// average = digital low pass filter
// for some value of n it's very good (eg n=200)
{
	int i;
	float input, voltage;
	unsigned long int sum;
	const float ADC_to_V = 1.0/1023.0*5;
	
	sum = 0;
	for(i=0;i<n;i++) {
		sum += analogRead(channel);
	}
	
	input = (float)sum / n; // average analog input
	
	// note that the simple division of float below takes around 
	// 40 us compared to around 4 us for equivalent multiplication
	// -> avoid using float division if possible
	// -> use multiplication instead of division when possible
	// -> avoid putting float division in loops
//	voltage = input / 1023.0 * 5;
	voltage = input * ADC_to_V; // much faster than expression above
	
	return voltage;
}


void loop()
{
	// not used
}
