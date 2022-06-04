#include <Servo.h>

#define TRIG 6 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 5 //ECHO 핀 설정 (초음파 받는 핀)
int servo_Ultra = 2;
int servo_Trig = 4;
int servo_Barrel = 3;
int pir = A0;
Servo Servo_Ultra;
Servo Servo_Trig;
Servo Servo_Barrel;
int angle = 90; // servo position in degrees
int ti = 0;
long duration, distance;
int TrigAngle[5] = {180, 120, 80, 20, 0};

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(pir, INPUT);
  Servo_Ultra.attach(servo_Ultra);
  Servo_Trig.attach(servo_Trig);
  Servo_Barrel.attach(servo_Barrel);
  Servo_Ultra.write(90);
  Servo_Ultra.write(90);
  Servo_Ultra.write(90);
}

void loop()
{

  for (angle = 0; angle < 180; angle++)
  {
    if (distance < 30) shoot();
    Servo_Ultra.write(angle);
    UltraSonic();
  }
  // now scan back from 180 to 0 degrees
  for (angle = 180; angle > 0; angle--)
  {
    if (distance < 30) {
      Servo_Barrel.write(angle);

    }
    Servo_Ultra.write(angle);
    UltraSonic();
  }
}

void UltraSonic() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn (ECHO, HIGH);
  distance = duration * 17 / 1000;
}

int shoot() {
  while (1) {
    Servo_Barrel.write(angle);
    if (digitalRead(pir) == 1) {
      Servo_Trig.write(TrigAngle[ti++]);
      if (ti == 5) reload();
      return 1;
    }
  }
}

int reload() {
  ti = 0;
}
