#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

unsigned long previousMillis = 0;
const int SW = 12;
int C = 0;
int fadeamount = 5;
boolean ledOn = LOW;
boolean previousSW = HIGH;
boolean currentSW = HIGH;
int brightness = 0;

void setup() {
  pixels.begin();
  pixels.show();
  pinMode(SW, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  currentSW = debounce(previousSW);  
  if(previousSW == HIGH && currentSW == LOW){
    C++;
  }
  previousSW = currentSW;
 
  
  if(C == 1){
    if(currentMillis - previousMillis >= 9.8){
      previousMillis = currentMillis;
    
      pixels.setPixelColor(0,brightness,0,0);
      pixels.show();
      brightness = brightness + fadeamount;

     if(brightness <= 0 || brightness >= 255){
      fadeamount = -fadeamount;     
      }
    }
  }
    else if(C == 2){
    if(currentMillis - previousMillis >= 9.8){
      previousMillis = currentMillis;
    
      pixels.setPixelColor(0,0,brightness,0);
      pixels.show();
      brightness = brightness + fadeamount;

     if(brightness <= 0 || brightness >= 255){
      fadeamount = -fadeamount;     
      }
    }
  }
    else if(C == 3){
    if(currentMillis - previousMillis >= 9.8){
      previousMillis = currentMillis;
    
      pixels.setPixelColor(0,0,0,brightness);
      pixels.show();
      brightness = brightness + fadeamount;

     if(brightness <= 0 || brightness >= 255){
      fadeamount = -fadeamount;     
      }
    }
  }
    else if(C == 4){
    if(currentMillis - previousMillis >= 9.8){
      previousMillis = currentMillis;
    
      pixels.setPixelColor(0,brightness,brightness,brightness);
      pixels.show();
      brightness = brightness + fadeamount;

     if(brightness <= 0 || brightness >= 255){
      fadeamount = -fadeamount;     
      }
    }
  }
    else if(C == 5){
      pixels.setPixelColor(0,0,0,0);
      pixels.show();
      C = 0;
    }
  }


boolean debounce (boolean previous){
  boolean current = digitalRead(SW);
  if(previous != current){
    delay(5);
    current = digitalRead(SW);
  }
  return current;
}
