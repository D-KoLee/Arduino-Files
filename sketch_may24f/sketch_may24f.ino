int fire = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(fire, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(fire));
}
