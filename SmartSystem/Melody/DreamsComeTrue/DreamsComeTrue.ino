// Dreams Come True

#include "pitches.h" // 음정을 미리 정의해 놓은 헤더 파일

const int SPEAKER = 0; // 스피커는 9번 핀에
float bpm = 100; // 노래의 BPM
float shift = 60 / bpm * 250; // 한 음 재생 시간

int notes[] = {
  NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5,
  NOTE_F5, NOTE_A5, NOTE_D5, 0,
  NOTE_G5, 0, NOTE_G5, NOTE_G5, 0, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_F5,
  0, NOTE_F5, 0, NOTE_F5, NOTE_F5, 0, NOTE_F5, 0, NOTE_F5, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, 0,
  NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5,
  NOTE_F5, NOTE_A5, NOTE_D5, 0,
  NOTE_G5, 0, NOTE_G5, NOTE_G5, 0, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_F5,
  0, NOTE_F5, 0, NOTE_F5, NOTE_F5, 0, NOTE_F5, 0, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_C5  
};

float times[] = {
  shift, shift, 2*shift, 2*shift,
  4*shift, 4*shift, 4*shift, 4*shift,
  shift, shift, shift, shift, shift, shift, 2*shift, 2*shift, shift, 3*shift,
  2*shift, shift, shift, shift, 2*shift, shift, shift, shift, 2*shift, shift, 2*shift, 3*shift, 8*shift, 2*shift,
  shift, shift, 2*shift, 2*shift,
  4*shift, 4*shift, 4*shift, 4*shift,
  shift, shift, shift, shift, shift, shift, 2*shift, 2*shift, shift, 3*shift,
  2*shift, shift, shift, shift, 2*shift, shift, shift, shift, 2*shift, shift, 3*shift, shift, 6*shift
};

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 63; i++)
  {
    tone(SPEAKER, notes[i], times[i]);
    delay(times[i]);  
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
