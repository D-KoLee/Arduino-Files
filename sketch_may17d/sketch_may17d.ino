

#define Buzzer 6
int a[15] = {1174, 1174, 1174, 932, 932, 1174, 1174, 1318,1318, 1318, 1046, 1046,1046,1046,1046};
int i = 0;
void setup() {
  // put your setup code here, to run once:

  pinMode(Buzzer, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(Buzzer, a[i], 1000);
  i++;
  if( i == 15) i = 0;
  delay(100);
}
