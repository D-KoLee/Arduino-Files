#include <Adafruit_NeoPixel.h>

#define LED 11

const int BLED = 5;

const int GLED = 6;

const int RLED = 7;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);


float I_shunt = 0.0;

float vout = 0.0;

float vin = 0.0;

float R1 = 30000.0;

float R2 = 7500.0;

float Amp = 0;

int G = 0;

int R = 0;

int B = 0;

int value = 0;

int snd = 0;

float mV;

void setup() {
  Serial.begin(9600);

  pinMode(BLED,OUTPUT);

  pinMode(GLED,OUTPUT);

  pinMode(RLED,OUTPUT);
  
}


void loop() {

  pixels.setPixelColor(0, 0, 255, 0);

  value = analogRead(A0);

  vout = (value * 5.0) / 1024.0;

  mV = vout*1000;

  Amp = mV/1.5;

  I_shunt = (vout / 0.000025);

  Serial.println(I_shunt);

  Serial.println(mV);

  Serial.println(Amp);

  if (Amp < 12) {
    digitalWrite(BLED, HIGH);

    digitalWrite(GLED, LOW);

    digitalWrite(RLED, LOW);
  }

  else if (Amp >= 12 && Amp <35) {
    
    digitalWrite(BLED, LOW);

    digitalWrite(GLED, HIGH);

    digitalWrite(RLED, LOW);
    
  }

  else if (Amp > 35) {

    digitalWrite(BLED, LOW);

    digitalWrite(GLED, LOW);

    digitalWrite(RLED, HIGH);
    
  }
  delay(200);
}
