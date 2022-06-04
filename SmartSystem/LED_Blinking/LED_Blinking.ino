const int LED_RED = 12;   //Second LED
const int LED_GREEN = 13; //First LED
/* 전자공학과 201901268 유승준 */
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  delay(1000);
  digitalWrite(LED_GREEN, LOW);
  delay(1000);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  delay(1000);
  digitalWrite(LED_GREEN, LOW);
  delay(1000);
}
