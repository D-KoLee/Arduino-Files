//int X = A0;
//int Y = A1;
//int Z = 3;
//int A = A3;
//
//void setup() {
//  pinMode(X, INPUT);
//  pinMode(Y, INPUT);
//  pinMode(Z, INPUT);
//  pinMode(A, INPUT);
//  Serial.begin(9600);
//}
//
//void loop() {
////  int x,y,z;
////  x=analogRead(X);
////  y=analogRead(Y);
////  z=digitalRead(Z);
////  Serial.print(x);Serial.print(", ");Serial.print(y);
////  Serial.print(", ");Serial.println(z*500); delay(50);
//Serial.println(analogRead(A));
//  
//}

const int piezoPin = 6;

int freq = 150;

boolean freqFlag = true;

 

void setup() {
  pinMode(piezoPin, OUTPUT);
}

 

void loop() {
  tone(piezoPin, freq, 10);

 

  if(freqFlag == true) freq += 2;

  if(freq >= 1800) freqFlag = false;

  if(freqFlag == false) freq -= 2;

  if(freq <= 150) freqFlag = true;
}
