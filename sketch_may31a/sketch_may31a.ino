#include <Servo.h>

int X = A5;
int Y = A4;
int Z = A3;
int servo_Barrel = 3;
Servo Servo_Barrel;

void setup() {
  // put your setup code here, to run once:
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  pinMode(Z, INPUT);
  Servo_Barrel.attach(servo_Barrel);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int degree = map(analogRead(X), 0, 1023, 0, 180);
  Serial.print("X : ");Serial.println(analogRead(X));
  Serial.print("degree : ");Serial.println(degree);
  Servo_Barrel.write(degree);
}
