#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

const int SW = 12;
int color = 0;

void setup() {
  pixels.begin();
  pixels.show();
  pinMode(SW, INPUT);
}


void loop() {
  int R, G, B;
  
  if(digitalRead(SW) == LOW) {
    R = (color == 0 || color == 3) * 255;
    //color가 0 또는 3일 경우에 1을, 이외엔 0을 반환하게 되고, 255를 곱함
    
    G = (color == 1 || color == 3) * 255;
    //color가 1 또는 3일 경우에 1을, 이외엔 0을 반환하게 되고, 255를 곱함
    
    B = (color == 2 || color == 3) * 255;
    //color가 2 또는 3일 경우에 1을, 이외엔 0을 반환하게 되고, 255를 곱함
    
    pixels.setPixelColor(0,R,G,B); //R, G, B의 값으로 설정
    pixels.show();
    delay(1000);
    color = (color+1) % 4; //color가 0~3으로 반복하게 함
  }
  
  else {
    pixels.setPixelColor(0,0,0,0);
    pixels.show();
  }
}
