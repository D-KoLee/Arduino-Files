#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

unsigned long pm = 0; //previousMillis

void setup() {
  pixels.begin();
  pixels.show();
}

int state = 1;

void loop() { 
  unsigned long cm = millis(); //currentMillis
if(cm - pm >= 50) {
  pm = cm;
  for(int brightness = 0; brightness <= 50; brightness += 5) {
       pixels.setPixelColor(0, 0, brightness, 0);
       pixels.show();
       cm = millis();
       state = 0;
  } //2초동안 점점 초록색 빛이 255 밝기까지 밝아지는 반복문
}
  if(cm - pm >= 50) {
    pm = cm;
  for(int brightness = 50; brightness >= 0; brightness -= 5) {
    if(cm -pm >= 50){
      pm = cm;
    pixels.setPixelColor(0, 0, brightness, 0);
    pixels.show();
    }
  } //2초동안 점점 255에서 0으로 파란색 빛이 꺼지는 반복문 
  }
}
