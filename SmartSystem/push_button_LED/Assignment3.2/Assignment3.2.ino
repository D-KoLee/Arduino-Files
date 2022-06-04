#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);
/*millis를 사용할 때 이전 시간을 저장하기 위한 변수*/
unsigned long pm = 0; //previousMillis

const int SW = 12;

boolean ledOn = LOW;
boolean previousSW = HIGH;
boolean currentSW = HIGH;

int state = 0;    //RGBW0을 구분하기 위한 변수
int br = 0;       //0~255 사이로 값이 변하며 밝기를 나타내는 변수
int interval = 5; //bright가 5씩 커지게 하는 변수

void setup() {
  pixels.begin();
  pixels.show();
  pinMode(SW, INPUT);
}

void loop() {
  unsigned long cm = millis(); //currentMillis
  
  currentSW = debounce(previousSW);  
  if(previousSW == HIGH && currentSW == LOW) {
    state++;//state를 증가시켜 다음 색상을 띄우도록 함
  }
  previousSW = currentSW;
  
  if(state == 0) {
    if(cm - pm >= 9.8) { 
      //1000ms 동안 102(fade in 51, fadeout 51)회 작동하므로 1000/102 = 9.8
      pm = cm;
      pixels.setPixelColor(0, br, 0, 0);
      pixels.show();
      br = br + interval; //5씩 증감

     if(br <= 0 || br >= 255) {
      interval = -interval; //255가 넘어가면 줄어들어야 하므로 음수로 변경
      }
    }
  }
  
  else if(state == 1) {
    if(cm - pm >= 9.8) {
      pm = cm;
      pixels.setPixelColor(0, 0, br, 0);
      pixels.show();
      br = br + interval;

     if(br <= 0 || br >= 255) {
      interval = -interval;     
      }
    }
  }
  
  else if(state == 2) {
    if(cm - pm >= 9.8) {
      pm = cm;
      pixels.setPixelColor(0, 0, 0, br);
      pixels.show();
      br = br + interval;

     if(br <= 0 || br >= 255) {
      interval = -interval;     
      }
    }
  }
  
  else if(state == 3) {
    if(cm - pm >= 9.8) {
      pm = cm;
      pixels.setPixelColor(0, br, br, br);
      pixels.show();
      br = br + interval;

     if(br <= 0 || br >= 255) {
      interval = -interval;     
      }
    }
  }
  
  else if(state == 4) {
    pixels.setPixelColor(0, 0, 0, 0);
    pixels.show();
    state = -1;
    }
  }


boolean debounce (boolean previous) {
  boolean current = digitalRead(SW);
  if(previous != current) {
    delay(5);
    current = digitalRead(SW);
  }
  return current;
}
