float t1,t2,dt;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  t1=micros();
  analogRead(A0);
  t2=micros();
  dt=t2-21;
  Serial.print("Time to print analog read = ");
  Serial.println(dt);
  delay(100);
}
