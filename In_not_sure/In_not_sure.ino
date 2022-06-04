#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

const int SW = 12;
const long interval = 50;
int count = 0;

unsigned long pm = 0; //previousMillis

boolean ledOn = LOW;
boolean previousSW = HIGH;
boolean currentSW = HIGH;

void setup() {
  pixels.begin();
  pixels.show();
  pinMode(SW, INPUT);
}


void loop() {
  unsigned long cm = millis(); //currentMillis
  currentSW = debounce(previousSW);

  if(previousSW == HIGH && currentSW == LOW) {
    ledOn = !ledOn;
  }
  previousSW = currentSW;
  
  if(ledOn == HIGH && count == 0) {
    for(int i = 0; i < 50; i += 5) {
      if((cm - pm) >= interval){
        pm = cm;
        pixels.setPixelColor(0,i,0,0);
        pixels.show();
      }
    }
    for(int i = 50; i > 0; i -= 5) {
      if(cm - pm >= interval){
        pm = cm;
        pixels.setPixelColor(0,i,0,0);
        pixels.show();
        }
      
    }
    if(!debounce(digitalRead(SW))) { ++count; ledOn = !ledOn;}
  }
  
  else if(ledOn == HIGH && count == 1) {
    for(int i = 0; i < 250; i += 2) {
      pixels.setPixelColor(0,0,i,0);
      pixels.show();
      delay(4);
    }
    for(int i = 250; i > 0; i -= 2) {
      pixels.setPixelColor(0,0,i,0);
      pixels.show();
      delay(4);
    }
    if(!debounce(digitalRead(SW))) { ++count; ledOn = !ledOn;}
  }
  
  else if(ledOn == HIGH && count == 2) {
    for(int i = 0; i < 250; i += 2) {
      pixels.setPixelColor(0,0,0,i);
      pixels.show();
      delay(4);
    }
    for(int i = 250; i > 0; i -= 2) {
      pixels.setPixelColor(0,0,0,i);
      pixels.show();
      delay(4);
    }
    if(!debounce(digitalRead(SW))) { ++count; ledOn = !ledOn;}
  }
  
  else if(ledOn == HIGH && count == 3) {
    for(int i = 0; i < 250; i += 2) {
      pixels.setPixelColor(0,i,i,i);
      pixels.show();
      delay(4);
    }
    for(int i = 250; i > 0; i -= 2) {
      pixels.setPixelColor(0,i,i,i);
      pixels.show();
      delay(4);
    }
    if(!debounce(digitalRead(SW))) { ++count; ledOn = !ledOn;}
  }
  else if(ledOn == HIGH && count == 4) {
    pixels.setPixelColor(0,0,0,0);
    pixels.show();
    delay(1000);
    if(!debounce(digitalRead(SW))) { count = 0; ledOn != ledOn;}
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
