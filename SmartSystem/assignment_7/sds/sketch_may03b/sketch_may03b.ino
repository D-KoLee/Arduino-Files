#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

const int SW = 12;
const int light_pin = A1;
const int sound_pin = A2;
const int IR_pin = A3;
const int touch_pin = A4;
const int poten_pin = A5;
boolean previousSW = HIGH;
boolean currentSW = HIGH;
boolean previousTo = HIGH;
boolean Doton = false;
boolean soundon = false;
boolean IRon = false;
int state = 0;
const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;
int colVal[8] = {1, 2, 4, 8, 16, 32, 64, 128};
int rowVal[8] = {255, 255, 255, 255, 255, 255, 255, 255};
int color[24] = {
  B10000000, B10000000, B10110011, B10110001, B10000100, B11001110, //R
  B11000001, B10000000, B10111110, B10111010, B10011000, B11011001, //G
  B10000000, B10000000, B10110110, B10110110, B10000000, B11001001, //B
  B10000000, B10000000, B10110111, B10110111, B10000111, B11001111  //P
};
unsigned long current_time = 0;
unsigned long previous_time = 0;
int r = 0;
int g = 0;
int b = 0;

void setup() {
  Serial.begin(9600);
  pinMode(light_pin, INPUT);
  pinMode(sound_pin, INPUT);
  pinMode(IR_pin, INPUT);
  pinMode(touch_pin, INPUT);
  pinMode(poten_pin, INPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pixels.begin();
  pixels.show();
  pinMode(SW, INPUT);
  previousTo = digitalRead(touch_pin);
}

void loop() {
  current_time = millis();

  if (soundon) {
    if ((current_time - previous_time) < 2000) {
      if ((current_time - previous_time) % 100 < 50) {
        pixels.setPixelColor(0, 255, 0, 0);
        pixels.show();
      } else {
        pixels.setPixelColor(0, 0, 0, 0);
        pixels.show();
      }
    } else soundon = false;
  } else if (IRon) {
    if ((current_time - previous_time) < 2000) {
      if ((current_time - previous_time) % 100 < 50) {
        pixels.setPixelColor(0, 0, 0, 255);
        pixels.show();
      } else {
        pixels.setPixelColor(0, 0, 0, 0);
        pixels.show();
      }
    } else IRon = false;
  }
  else {
    currentSW = debounce(previousSW);
    if (previousSW == HIGH && currentSW == LOW) {
      Doton = !Doton;
    }
    previousSW = currentSW;

    if (digitalRead(touch_pin) != previousTo) {
      state++;
      if (state > 3) state = 0;
      previousTo = digitalRead(touch_pin);
    }

    switch (state) {
      case 0:
        r = analogRead(poten_pin) / 4;
        g = 0;
        b = 0;
        break;
      case 1:
        r = 0;
        g = analogRead(poten_pin) / 4;
        b = 0;
        break;
      case 2:
        r = 0;
        g = 0;
        b = analogRead(poten_pin) / 4;
        break;
      case 3:
        r = analogRead(poten_pin) / 4;
        g = analogRead(poten_pin) / 16;
        b = analogRead(poten_pin) / 12;
        break;
    }

    if (digitalRead(light_pin) || Doton) {
      pixels.setPixelColor(0, r, g, b);
      pixels.show();
    } else {
      pixels.setPixelColor(0, 0, 0, 0);
      pixels.show();
    }

    if (analogRead(sound_pin) > 500) {
      soundon = true;
      previous_time = millis();
    }
    if (!digitalRead(IR_pin)) {
      IRon = true;
      previous_time = millis();
    }
  }


  if (Doton) {
    for (int row = 1; row < 7; row++) rowVal[row] = color[6 * state + row - 1];
    for (int col = 0; col < 8; col++) DotMatrixWrite(rowVal[col], colVal[col]);
  } else DotMatrixWrite(0, 0);

}

boolean debounce(boolean previous) {
  boolean current = digitalRead(SW);
  if (previous != current) {
    delay(5);
    current = digitalRead(SW);
  }
  return current;
}

void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal);
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}
