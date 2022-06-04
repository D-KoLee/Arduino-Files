// Night Falls

#include <Tone.h>
#include "pitches.h"

Tone tone1;
Tone tone2;
float bpm = 170; // 노래의 BPM
float shift = 60 / bpm * 250; // 한 음 재생 시간

void setup() 
{
  tone1.begin(6);
  tone2.begin(7);
}
 

void loop() 
{
  tone1.play(NOTE_F4);
  delay(shift*4);
  tone1.play(NOTE_G4);
  delay(shift*4);
  tone1.play(NOTE_A4);
  tone2.play(NOTE_D4);
  delay(shift*8);
  tone2.play(NOTE_AS3);
  delay(shift*4);
  tone1.play(NOTE_A4);
  tone2.play(NOTE_D4);
  delay(shift*4);
  tone1.play(NOTE_G4);
  tone2.play(NOTE_C4);
  delay(shift*12);
  tone1.stop();
  tone2.play(NOTE_C4);
  delay(shift*4);
  tone1.play(NOTE_G4);
  tone2.play(NOTE_C4);
  delay(shift*8);
  tone1.play(NOTE_F4);
  tone2.stop();
  delay(shift*3);
  tone1.play(NOTE_E4);
  delay(shift*3);
  tone1.play(NOTE_C4);
  delay(shift*2);
  tone1.play(NOTE_D4);
  tone2.play(NOTE_AS3);
  delay(shift*12);
  tone1.play(NOTE_G3);
  tone2.play(NOTE_A3);
  delay(shift*4);
  tone1.play(NOTE_AS3);
  tone2.play(NOTE_G3);
  delay(shift*8);
  tone1.play(NOTE_D4);
  tone2.play(NOTE_AS3);
  delay(shift*8);
  tone1.play(NOTE_CS4);
  tone2.play(NOTE_A3);
  delay(shift*16);
  tone1.play(NOTE_D4);
  tone2.play(NOTE_A3);
  delay(shift*8);
  tone2.play(NOTE_A3);
  delay(shift*3);
  tone2.play(NOTE_GS3);
  delay(shift*3);
  tone2.play(NOTE_G3);
  delay(shift*2);
  tone2.play(NOTE_F3);
  delay(shift*7);

  tone1.stop();
  tone2.stop();

  delay(15000);

}
