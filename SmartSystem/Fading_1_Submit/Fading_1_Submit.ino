#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.show();
}

void loop() { 
  for(int time = 0; time < 20; ++time) {
    pixels.setPixelColor(0, 255, 0, 0);
    pixels.show();
    delay(50);
    
    pixels.setPixelColor(0, 0, 0, 0);
    pixels.show();
    delay(50);
  } //2초동안 빨간불이 100ms 주기로 점등되는 반복문
  
  for(int brightness = 0; brightness <= 255; brightness += 5) {
    pixels.setPixelColor(0, 0, brightness, 0);
    pixels.show();
    delay(39);//1989ms로 요구한 2000ms보다 11ms 짧습니다.
  } //2초동안 점점 초록색 빛이 255 밝기까지 밝아지는 반복문
  
  for(int brightness = 255; brightness >= 0; brightness -= 5) {
    pixels.setPixelColor(0, 0, 0, brightness);
    pixels.show();
    delay(39);//1989ms로 요구한 2000ms보다 11ms 짧습니다.
  } //2초동안 점점 255에서 0으로 파란색 빛이 꺼지는 반복문 
}
