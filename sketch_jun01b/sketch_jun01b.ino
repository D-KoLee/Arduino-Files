int vib = A1;

void setup() {
  // put your setup code here, to run once:
  pinMode(vib,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(analogRead(vib) >=1)
  Serial.println(analogRead(vib));
}
