#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftware(0,1);

DFRobotDFPlayerMini player;

long cm = 0;
long pm = 0;

void setup() {
  // put your setup code here, to run once:
  mySoftware.begin(9600);
  player.begin(mySoftware);
  player.reset();
  player.volume(20);
  delay(1);
}

void loop() {
  delay(1);
  player.play(1);
  delay(3000);
  player.play(3);
  delay(3000);
  player.play(4);
  delay(3000);
  player.play(5);
  delay(3000);
  player.play(6);
  delay(3000);
  player.play(7);
  delay(3000);
  player.play(8);
  delay(3000);
  player.play(9);
  delay(3000);
  player.play(10);
  delay(3000);
}
