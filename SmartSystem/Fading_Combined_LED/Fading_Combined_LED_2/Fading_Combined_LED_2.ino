#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

long rand_N1 = 0;
long rand_N2 = 0;
long rand_N3 = 0;
기
void setup() {
  // put your setup code here, to run once:
  pixels.begin();
  pixels.show();
  randomSeed(analogRead(0));
  Serial.begin (9600);
}

void loop() {  
  
  rand_N1 = random(255);
  rand_N2 = random(255);
  rand_N3 = random(255);
  Serial.print("randomNumber 1: ");Serial.println( rand_N1 );
  Serial.print("randomNumber 2: ");Serial.println ( rand_N2 );
  Serial.print("randomNumber 3: ");Serial.println ( rand_N3 );
  Serial.println("");
  
  for(int time = 0; time < 20; ++time) {
    pixels.setPixelColor(0, rand_N1, rand_N2, rand_N3);
    pixels.show();
    delay(50);
    pixels.setPixelColor(0, 0, 0, 0);
    pixels.show();
    delay(50);
  } //2초동안 무작위 색상의 불빛이 100ms 주기로 점등되는 반복문
  
  for(int brightness = 0; brightness <= 255; brightness += 5) {
    pixels.setPixelColor(0, 0, brightness, 0);
    pixels.show();
    delay(39); //1989ms로 요구한 2000ms보다 11ms 짧습니다.
  } //2초동안 점점 초록색 빛이 255 밝기까지 밝아지는 반복문 
  
  for(int brightness = 255; brightness >= 0; brightness -= 5) {
    pixels.setPixelColor(0, 0, 0, brightness);
    pixels.show();
    delay(39);//1989ms로 요구한 2000ms보다 11ms 짧습니다.
  } //2초동안 점점 파란색 빛이 꺼지는 반복문 
}
