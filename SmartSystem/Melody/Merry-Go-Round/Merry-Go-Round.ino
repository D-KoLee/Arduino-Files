// MERRY-GO-ROUND

#include "pitches.h" // 음정을 미리 정의해 놓은 헤더 파일

const int SPEAKER = 6; // 스피커는 9번 핀에
float bpm = 108; // 노래의 BPM
float shift = 60 / bpm * 250; // 한 음 재생 시간 (대개 4분의 4박자)

int notes[] = {
  NOTE_FS5, NOTE_E5, NOTE_DS5, NOTE_FS5, NOTE_DS5, NOTE_CS5, NOTE_DS5, NOTE_CS5, NOTE_B4, NOTE_CS5, NOTE_DS5, NOTE_CS5, 0,
  NOTE_B4, NOTE_DS5, NOTE_CS5, NOTE_B4, NOTE_GS4, NOTE_B4, NOTE_CS5, NOTE_DS5, NOTE_CS5, 0,
  NOTE_FS5, NOTE_E5, NOTE_DS5, NOTE_FS5, NOTE_DS5, NOTE_CS5, NOTE_DS5, NOTE_CS5, NOTE_B4, NOTE_CS5, NOTE_DS5, NOTE_AS5, NOTE_B5,
  NOTE_GS4, NOTE_DS5, NOTE_GS4, NOTE_FS4, NOTE_DS5, NOTE_AS4, NOTE_B4
};

float times[] = {
  shift*4, shift*4, shift*2, shift*2, shift*2, shift*4, shift*2, shift*2, shift*4, shift*2, shift*2, shift*6, shift*2,
  shift*2, shift*2, shift*2, shift*2, shift*2, shift*2, shift*2, shift*2, shift*6, shift*4,
  shift*4, shift*4, shift*2, shift*2, shift*2, shift*4, shift*2, shift*2, shift*4, shift*2, shift*2, shift*6, shift*4,
  shift*2, shift*4, shift*2, shift*2, shift*4, shift*2, shift*8
};

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 43; i++)
  {
    tone(SPEAKER, notes[i], times[i]);
    delay(times[i]);  
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
