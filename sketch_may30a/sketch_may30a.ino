#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

SoftwareSerial mySerial(10, 11);

void setup () {
  Serial.begin(9600);
  mySerial.begin(9600);
  mp3_set_serial(mySerial);
  delay(1);
  mp3_set_volume(15);
}

void loop() {
  bool play = digitalRead(3);
  if(play == HIGH) 
  mp3_next();
}
