#include <Servo.h>

int servo_Ultra = 2;
int servo_Trig = A1;
int servo_Barrel = 3;

Servo Servo_Ultra;
Servo Servo_Trig;
Servo Servo_Barrel;

void setup() {
  // put your setup code here, to run once:
  Servo_Ultra.attach(servo_Ultra);
  Servo_Trig.attach(servo_Trig);
  Servo_Barrel.attach(servo_Barrel);
  Servo_Ultra.write(90);
  Servo_Trig.write(0);
  Servo_Barrel.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
