#include <Adafruit_NeoPixel.h>

#define LED 11

int button = 12;
int light = A1;
int sound_sensor = A2;
int IR = A3;
int Touch = A4;
int potentiometer = A5;
const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;

unsigned long cm = 0;
unsigned long pm = 0;

int colVal[8] = {0, 0, 4, 8, 16, 32, 64, 0};
int rowVal[8] = {255, 255, 255, 255, 255, 255, 255, 255};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

boolean previousSW = HIGH;
boolean currentSW = HIGH;
boolean push = true; //default on
boolean detect = false; //default not detected
boolean sound = false; //default no big sound
boolean TOUCH = digitalRead(Touch);
int state = 0;
int Red, Blue, Green;

void setup() {
  pinMode(light, INPUT);
  pinMode(sound_sensor, INPUT);
  pinMode(IR, INPUT);
  pinMode(Touch, INPUT);
  pinMode(potentiometer, INPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pixels.begin();
  pixels.show();
  Serial.begin(9600);
}

void loop() {
  cm = millis();

  if (analogRead(sound_sensor) > 500) {
    sound = 1;
    pm = millis();
  }
  if (!digitalRead(IR)) {
    detect = 1;
    pm = millis();
  }

  if (sound == 1) {
    if ((cm - pm) < 2000) {
      if ((cm - pm) % 100 < 50) {
        pixels.setPixelColor(0, 50, 0, 0);
        pixels.show();
      }
      else {
        pixels.setPixelColor(0, 0, 0, 0);
        pixels.show();
      }
    }
    else
      sound = !sound;
  }
  else if (detect == 1) {
    if ((cm - pm) < 2000) {
      if ((cm - pm) % 100 < 50) {
        pixels.setPixelColor(0, 0, 0, 50);
        pixels.show();
      }
      else {
        pixels.setPixelColor(0, 0, 0, 0);
        pixels.show();
      }
    }
    else
      detect = !detect;
  }
  else {
    currentSW = debounce(previousSW);
    if (previousSW == HIGH && currentSW == LOW) {
      push = !push;
    }
    previousSW = currentSW;

    if (digitalRead(Touch) != TOUCH) {
      ++state;
      if (state == 4) state = 0;
      TOUCH = digitalRead(Touch);
    }

    if (state == 0) {
      Red = map(analogRead(potentiometer), 0, 1023, 0, 255);
      Blue = Green = 0;
      setRow(0);
    }
    else if (state == 1) {
      Green = map(analogRead(potentiometer), 0, 1023, 0, 255);
      Red = Blue = 0;
      setRow(1);
    }
    else if (state == 2) {
      Blue = map(analogRead(potentiometer), 0, 1023, 0, 255);
      Red = Green = 0;
      setRow(2);
    }
    else if (state == 3) { 
      Red = analogRead(potentiometer) / 4;
      Green = 0;
      Blue = analogRead(potentiometer) / 8;
      setRow(3);
    }

    if (digitalRead(light) || push) {
      pixels.setPixelColor(0, Red, Green, Blue);
      pixels.show();
    }
    else {
      pixels.setPixelColor(0, 0, 0, 0);
      pixels.show();
      setRow(-1);
    }


  }
  if (push) {
    for (int colrow = 0; colrow < 8; colrow++) {
      DotMatrixWrite(rowVal[colrow], colVal[colrow]);
    }
  }
}

boolean debounce (boolean previous) {
  boolean current = digitalRead(button);
  if (previous != current) {
    delay(5);
    current = digitalRead(button);
  }
  return current;
}

void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW); //Pull latch LOW to send data
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal); //send the row data byte. Row data should be sent first
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}

void setRow(int color) {
  int i = 2;
  
  if (color == 0) { //R
    rowVal[i] = 1;
    rowVal[++i] = 111;
    rowVal[++i] = 111;
    rowVal[++i] = 111;
    rowVal[++i] = 145;
  }
  else if (color == 1) {//G
    rowVal[i] = 131;
    rowVal[++i] = 125;
    rowVal[++i] = 125;
    rowVal[++i] = 109;
    rowVal[++i] = 227;
  }
  else if (color == 2) {//B
    rowVal[i] = 1;
    rowVal[++i] = 109;
    rowVal[++i] = 109;
    rowVal[++i] = 109;
    rowVal[++i] = 147;
  }
  else if (color == 3) {//P
    rowVal[i] = 1;
    rowVal[++i] = 111;
    rowVal[++i] = 111;
    rowVal[++i] = 111;
    rowVal[++i] = 159;
  }
  else {
    rowVal[i] = 255; //모두 끄기
    rowVal[++i] = 255;
    rowVal[++i] = 255;
    rowVal[++i] = 255;
    rowVal[++i] = 255;
  }
}
