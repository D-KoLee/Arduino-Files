int light = A1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(light, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int Light = analogRead(light);
  int LIGHT = digitalRead(light);
  Serial.println(Light);
  Serial.println(LIGHT);
  Serial.println("");
  
}
