#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
 
SoftwareSerial mySerial(10, 11); // RX, TX
 
void setup () {
  Serial.begin (9600);
  mySerial.begin (9600);
  mp3_set_serial (mySerial);    //set softwareSerial for DFPlayer-mini mp3 module 
  delay(1);                     // delay 1ms to set volume
  mp3_set_volume (15);          // value 0~30
}
void loop () {
  boolean play_state = digitalRead(3);// connect Pin3 to BUSY pin of player
  if(play_state == HIGH){
    mp3_next ();
  }
}


//출처: https://deneb21.tistory.com/418 [Do It Yourself!]
