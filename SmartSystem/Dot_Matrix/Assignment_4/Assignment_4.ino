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
//Same to {B00000000, B00000010, B00000100, B00001000, B00000000, B00100000, B01000000, B10000000};
int rowVal[8] = {255, 255, 255, 255, 255, 255, 255, 255};

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
  
  ten = digit / 10;
  one = digit % 10; 

  setRow_ten(ten);
  setRow_one(one);
  
  for(int col = 0; col < 8; col++) {
    DotMatrixWrite(rowVal[col], colVal[col]);
  }

  if(digit <= 99) {
    pixels.setPixelColor(0,0,100,0);
    pixels.show();
  }
  else {
    pixels.setPixelColor(0,0,0,0);
    digit = 1;
  }
}

void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW); //Pull latch LOW to send data
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal); //send the row data byte. Row data should be sent first
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}

void setRow_ten(int ten) {
  if(ten == 0) {
    rowVal[1] = 1;
    rowVal[2] = 125;
    rowVal[3] = 1;
  }
  else if(ten == 1) {
    rowVal[1] = 189;
    rowVal[2] = 1;
    rowVal[3] = 253;
  }
  else if(ten == 2) {
    rowVal[1] = 97;
    rowVal[2] = 109;
    rowVal[3] = 13;
  }
  else if(ten == 3) {
    rowVal[1] = 109;
    rowVal[2] = 109;
    rowVal[3] = 1;
  }
  else if(ten == 4) {
    rowVal[1] = 15;
    rowVal[2] = 239;
    rowVal[3] = 1;
  }
  else if(ten == 5) {
    rowVal[1] = 13;
    rowVal[2] = 109;
    rowVal[3] = 97;
  }
  else if(ten == 6) {
    rowVal[1] = 1;
    rowVal[2] = 109;
    rowVal[3] = 97;
  }
  else if(ten == 7) {
    rowVal[1] = 15;
    rowVal[2] = 127;
    rowVal[3] = 1;
  }
  else if(ten == 8) {
    rowVal[1] = 1;
    rowVal[2] = 109;
    rowVal[3] = 1;
  }
  else if(ten == 9) {
    rowVal[1] = 13;
    rowVal[2] = 109;
    rowVal[3] = 1;
  }
  else {   //?를 표시
    rowVal[1] = 191;
    rowVal[2] = 98;
    rowVal[3] = 159;
  }
}

void setRow_one(int one) {
  if(one == 0) {
    rowVal[5] = 1;
    rowVal[6] = 125;
    rowVal[7] = 1;
  }
  else if(one == 1) {
    rowVal[5] = 189;
    rowVal[6] = 1;
    rowVal[7] = 253;
  }
  else if(one == 2) {
    rowVal[5] = 97;
    rowVal[6] = 109;
    rowVal[7] = 13;
  }
  else if(one == 3) {
    rowVal[5] = 109;
    rowVal[6] = 109;
    rowVal[7] = 1;
  }
  else if(one == 4) {
    rowVal[5] = 15;
    rowVal[6] = 239;
    rowVal[7] = 1;
  }
  else if(one == 5) {
    rowVal[5] = 13;
    rowVal[6] = 109;
    rowVal[7] = 97;
  }
  else if(one == 6) {
    rowVal[5] = 1;
    rowVal[6] = 109;
    rowVal[7] = 97;
  }
  else if(one == 7) {
    rowVal[5] = 15;
    rowVal[6] = 127;
    rowVal[7] = 1;
  }
  else if(one == 8) {
    rowVal[5] = 1;
    rowVal[6] = 109;
    rowVal[7] = 1;
  }
  else if(one == 9) {
    rowVal[5] = 13;
    rowVal[6] = 109;
    rowVal[7] = 1;
  }
  else {   //?를 표시
    rowVal[5] = 191;
    rowVal[6] = 98;
    rowVal[7] = 159;
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
