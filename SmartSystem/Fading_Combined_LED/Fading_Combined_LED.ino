#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  pixels.begin();
  pixels.show();
  Serial.begin(9600);
}

void loop() { 
  Serial.println("------------------------------------------");
  long mil = 0; 
  long a1, a2 = 0;
  mil = micros();Serial.print("At Start : ");Serial.print(mil/1000000);Serial.print(".");Serial.println(mil%100000);
  a1 = a2; a2 = (mil/1000000) + (mil%1000000);Serial.print("Minus : ");Serial.println(a2-a1);
  for(int time = 0; time < 20; ++time) {
    pixels.setPixelColor(0, 255, 0, 0);
    pixels.show();
    delay(50);
    pixels.setPixelColor(0, 0, 0, 0);
    pixels.show();
    delay(50);
  } //2초동안 빨간불이 100ms 주기로 점등되는 반복문
  mil = micros();Serial.print("After Red blink : ");Serial.print(mil/1000000);Serial.print(".");Serial.println(mil%100000);
  a1 = a2; a2 = (mil/1000000) + (mil%1000000);Serial.print("Minus : ");Serial.println(a2-a1);
  for(int brightness = 0; brightness <= 255; brightness += 5) {
    pixels.setPixelColor(0, 0, brightness, 0);
    pixels.show();
    delay(39);//1989ms로 요구한 2000ms보다 11ms 짧습니다.
    //delayMicroseconds(39215); //2000ms보다 0.035ms 짧습니다. 그러나 delayMicroseconds는 16383μs보다 긴 시간에서는 정확하게 작동할지 알 수 없습니다.
  } //2초동안 점점 초록색 빛이 255 밝기까지 밝아지는 반복문
  mil = micros();Serial.print("After GREEN : ");Serial.print(mil/1000000);Serial.print(".");Serial.println(mil%100000);
  a1 = a2; a2 = (mil/1000000) + (mil%1000000);Serial.print("Minus : ");Serial.println(a2-a1);
  for(int brightness = 255; brightness >= 0; brightness -= 5) {
    pixels.setPixelColor(0, 0, 0, brightness);
    pixels.show();
    delay(39);//1989ms로 요구한 2000ms보다 11ms 짧습니다.
    //delayMicroseconds(39215);
  } //2초동안 점점 파란색 빛이 꺼지는 반복문 
  mil = micros();Serial.print("After BLUE : ");Serial.print(mil/1000000);Serial.print(".");Serial.println(mil%1000000);
}
