#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

unsigned long pm = 0; //previousMillis
int br = 0;

const int SW = 12;
int count = 0;

boolean ledOn = LOW;
boolean previousSW = HIGH;
boolean currentSW = HIGH;

void setup() {
  pixels.begin();
  pixels.show();
}

int state = 0;

void loop() {
 currentSW = debounce(previousSW);

  if(previousSW == HIGH && currentSW == LOW) {
    ledOn = !ledOn;
  }
  previousSW = currentSW;

  if(cm - pm >= 10 && br <= 250 && state == 0){
    pm = cm;
    if(ledOn == HIGH){
      switch(count){
        count 0:
         pixels.setPixelColor(0, br, 0, 0);
         pixels.show();
         br += 5;
         if(br <= 0) { br = 0; state = 0; }
         if(cm - pm >= 10 && br > 0){
          pm = cm;
          pixels.setPixelColor(0, 0, br, 0);
          pixels.show();
          br -= 5;
          if(br <= 0) { br = 0; state = 0; }
          }
      } 

  if(ledOn == HIGH && count == 0) {
    for(int i = 0; i < 250; i += 2) {
        pixels.setPixelColor(0,i,0,0);
        pixels.show();
        delay(4);
    }
    for(int i = 250; i > 0; i -= 2) {
        pixels.setPixelColor(0,i,0,0);
        pixels.show();
        delay(4);
      
    }
    if(currentSW != debounce(previousSW)) ++count;
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
    if(currentSW != debounce(previousSW)) ++count;
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
    if(currentSW != debounce(previousSW)) ++count;
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
    if(currentSW != debounce(previousSW)) ++count;
  }
  else if(ledOn == HIGH && count == 4) {
    pixels.setPixelColor(0,0,0,0);
    pixels.show();
    delay(1000);
    if(currentSW != debounce(previousSW)) count = 0;
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





/*
void loop() { 
  unsigned long cm = millis(); //currentMillis
  
  if(cm - pm >= 10 && br <= 250 && state == 0){
    pm = cm;
    pixels.setPixelColor(0, 0, br, 0);
    pixels.show();
    br += 5;
    if(br >= 250) state = 1;
  }
  else if(cm - pm >= 10 && br > 0){
    pm = cm;
    pixels.setPixelColor(0, 0, br, 0);
    pixels.show();
    br -= 5;
    if(br <= 0) { br = 0; state = 0; }
  } 
}
*/
