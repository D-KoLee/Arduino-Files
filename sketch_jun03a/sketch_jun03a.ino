

int pir = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pir, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("PIR anal: "); Serial.println(analogRead(pir));
  Serial.print("PIR digi: "); Serial.println(digitalRead(pir));
  Serial.println("");

}
