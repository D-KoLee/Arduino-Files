#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1,LED,NEO_GRB + NEO_KHZ800);

const int SW = 12;
boolean previousSW = HIGH;
boolean currentSW = HIGH;
int state = 0;

const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;

int colVal[8] = {1,2,4,8,16,32,64,128};
int rowVal[30] = {
  B00000001, B01111101, B00000001,
  B10111101, B00000001, B11111101,
  B01100001, B01101101, B00001101,
  B01101101, B01101101, B00000001,
  B00001111, B11101111, B00000001,
  B00001101, B01101101, B01100001,
  B00000001, B01101101, B01100001,
  B01111111, B01111111, B00000001,
  B00000001, B01101101, B00000001,
  B00001101, B01101101, B00000001,
};
int row = 0;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pixels.begin();
  pixels.show();
  pinMode(SW, INPUT);
}

void loop() {
   currentSW = debounce(previousSW);
  if(previousSW == HIGH && currentSW == LOW) {
    state++;
  }
  previousSW = currentSW;
  
  if(state != 0) {
    
    pixels.setPixelColor(0,0,100,0);
    pixels.show();
    DotMatrixWrite(B11111111, colVal[0]);
    row = state / 10;
    for(int col = 1; col <4; col++) {
      DotMatrixWrite(rowVal[3*row+col-1], colVal[col]);
    }
    
    DotMatrixWrite(B11111111, colVal[4]);
    row = state % 10;
    for(int col = 5; col <8; col++) {
      DotMatrixWrite(rowVal[3*row+col-5], colVal[col]);
    }
    if(state == 100){
      state = 0;
    }
  } else {
    DotMatrixWrite(B11111111, B11111111);
    pixels.setPixelColor(0,0,0,0);
    pixels.show();
  } 
}

boolean debounce(boolean previous) {
  boolean current = digitalRead(SW);
  if (previous != current) {
    delay(5);
    current = digitalRead(SW);
  }
  return current;
}

void DotMatrixWrite(int rowVal, int colVal){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal);
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}
