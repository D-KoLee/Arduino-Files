
static int PIR = A1;

void setup() {
  pinMode(PIR, INPUT);
  Serial.begin(9600);
}

void loop() {
  int PIR_INPUT = analogRead(PIR);
  int PIR_in = digitalRead(PIR);

  Serial.print(PIR_INPUT);Serial.print(", ");
  
  Serial.println(PIR_in);
  
}
