#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

const int SW = 12;

boolean ledOn = LOW;
boolean previousSW = HIGH;
boolean currentSW = HIGH;

void setup() {
  pixels.begin();
  pixels.show();
  pinMode(SW, INPUT);
}


void loop() {
  currentSW = debounce(previousSW);

  if(previousSW == HIGH && currentSW == LOW) {
    ledOn = !ledOn;
  }
  previousSW = currentSW;
  if(ledOn == HIGH) {
    pixels.setPixelColor(0,50,50,50);
    pixels.show();
  }
  else {
    pixels.setPixelColor(0,0,0,0);
    pixels.show();
  }
}

boolean debounce(boolean previous) {
  boolean current = digitalRead(SW);
  if(previous != current) {
    delay(5);
    current = digitalRead(SW);
  }
  return current;
}
