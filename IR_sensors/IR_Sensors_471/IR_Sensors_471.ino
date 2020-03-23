//3 sides obstacle detection code for mech 471 
//written by: Tyler Montcalm


//////////PIN CALLOUTS////////////
int ir_1=4;
int ir_2=5;
int ir_3=6;

////////GLOBAL VARIABLES IR////////
int sensorVal1;
int sensorVal2;
int sensorVal3; 

/////////FUNCTIONS IR Detection////////////
void ir_setup();
void ir_read();
void ir_print();

void setup()
{
  Serial.begin(9600);
  ir_setup();
}

void loop() 
{
 ir_read();
 ir_print();
 delay(500);
}

void ir_setup()
{
  pinMode(ir_1, INPUT_PULLUP);
  pinMode(ir_2, INPUT_PULLUP);
  pinMode(ir_3, INPUT_PULLUP);
}

void ir_read()
{
  sensorVal1 = digitalRead(ir_1);
  sensorVal2 = digitalRead(ir_2);
  sensorVal3 = digitalRead(ir_3);
}

void ir_print()
{
 Serial.print("sensorVal1 =");
 Serial.println(sensorVal1);
 Serial.print("sensorVal2 =");
 Serial.println(sensorVal2);
 Serial.print("sensorVal3 =");
 Serial.println(sensorVal3);
}
