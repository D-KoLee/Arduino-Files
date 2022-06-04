#include <Adafruit_NeoPixel.h>
#define LED 11

const int dataPin = 10; //The Serial Data Pinto the Shift Register
const int latchPin = 8; //The Latch Pin to the Shift Register
const int clockPin = 7; //The Clock Pin to the Shift Register
const int SW = 12;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

boolean previousSW = HIGH;
boolean currentSW = HIGH;
int digit = 1;

int colVal[8] = {0, 2, 4, 8, 0, 32, 64, 128};
int rowVal[8] = {255, 255, 255, 255, 255, 255, 255, 255}; //아무것도 켜지지 않도록

void setup() {
  pixels.begin();
  pixels.show();
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(SW, INPUT);
}

void loop() {
  currentSW = debounce(previousSW);  
  if(previousSW == HIGH && currentSW == LOW) {
    digit++;
  }
  previousSW = currentSW;

  int ten = 0;
  int one = 0;
  
  ten = digit / 10; //10의 자릿수 추출
  one = digit % 10; // 1의 자릿수 추출

  if(digit >= 1 && digit <= 99) {
    setRow(ten, 10);
    setRow(one, 1);
    pixels.setPixelColor(0,0,100,0);
    pixels.show();
  }
  else {
    pixels.setPixelColor(0,0,0,0);
    pixels.show();
    setRow(-1, 10); //else문으로 빠지도록 -1 전달
    setRow(-1, 1);
    digit = 0;
  }
  
  for(int colrow = 0; colrow < 8; colrow++) {
    DotMatrixWrite(rowVal[colrow], colVal[colrow]);
  }
}

void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW); //Pull latch LOW to send data
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal); //send the row data byte. Row data should be sent first
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}

void setRow(int digit, int Unit) {
  int i = 0;
  
  if(Unit == 10) i = 1; //10의 자릿수를 1~3row에 표현하기 위함
  else i = 5;           // 1의 자릿수를 5~7row에 표현하기 위함
   
  if(digit == 0) {
    rowVal[i] = 1;
    rowVal[++i] = 125;
    rowVal[++i] = 1;
  }
  else if(digit == 1) {
    rowVal[i] = 189;
    rowVal[++i] = 1;
    rowVal[++i] = 253;
  }
  else if(digit == 2) {
    rowVal[i] = 97;
    rowVal[++i] = 109;
    rowVal[++i] = 13;
  }
  else if(digit == 3) {
    rowVal[i] = 109;
    rowVal[++i] = 109;
    rowVal[++i] = 1;
  }
  else if(digit == 4) {
    rowVal[i] = 15;
    rowVal[++i] = 239;
    rowVal[++i] = 1;
  }
  else if(digit == 5) {
    rowVal[i] = 13;
    rowVal[++i] = 109;
    rowVal[++i] = 97;
  }
  else if(digit == 6) {
    rowVal[i] = 1;
    rowVal[++i] = 109;
    rowVal[++i] = 97;
  }
  else if(digit == 7) {
    rowVal[i] = 15;
    rowVal[++i] = 127;
    rowVal[++i] = 1;
  }
  else if(digit == 8) {
    rowVal[i] = 1;
    rowVal[++i] = 109;
    rowVal[++i] = 1;
  }
  else if(digit == 9) {
    rowVal[i] = 13;
    rowVal[++i] = 109;
    rowVal[++i] = 1;
  }
  else {
    rowVal[1] = 255; //모두 끄기
    rowVal[2] = 255;
    rowVal[3] = 255;
    rowVal[5] = 255;
    rowVal[6] = 255;
    rowVal[7] = 255;
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
