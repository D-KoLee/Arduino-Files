#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

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
}

void loop() {
  pixels.setPixelColor(0, 0, 255, 0);
  value = analogRead(A0);
  vout = (value * 5.0) / 1024.0;
  mV = vout*1000;
  Amp = mV/1.25;
  Serial.println(mV);
  Serial.print(',');
  Serial.println(Amp);
  if(Amp <= 180) { 
    G = (255/180) * Amp;
    B = 255 - G;
    pixels.setPixelColor(0, 0, G, B);
  } //전류값이 180 이하일 때 RGB값이 파~초로 변화, 180일때 가장 Stable이어서 150이 아닌 180으로 중간 설정
  else if(300 >= Amp && Amp > 180) {
    G = (255/120) * (300 - Amp);
    R = 255 - G;
    pixels.setPixelColor(0, R, G, 0);
  } //When the value of current is from 180 to 300, the colour changes green to red.
  else if(Amp > 300)
    pixels.setPixelColor(0, 255, 0, 0); //if the current is over 300, show only red light.
  delay(10);
}#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

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
}

void loop() {
  pixels.setPixelColor(0, 0, 255, 0);
  value = analogRead(A0);
  vout = (value * 5.0) / 1024.0;
  mV = vout*1000;
  Amp = mV/1.25;
  Serial.println(mV);
  Serial.print(',');
  Serial.println(Amp);
  if(Amp <= 180) { 
    G = (255/180) * Amp;
    B = 255 - G;
    pixels.setPixelColor(0, 0, G, B);
  } //전류값이 180 이하일 때 RGB값이 파~초로 변화, 180일때 가장 Stable이어서 150이 아닌 180으로 중간 설정
  else if(300 >= Amp && Amp > 180) {
    G = (255/120) * (300 - Amp);
    R = 255 - G;
    pixels.setPixelColor(0, R, G, 0);
  } //When the value of current is from 180 to 300, the colour changes green to red.
  else if(Amp > 300)
    pixels.setPixelColor(0, 255, 0, 0); //if the current is over 300, show only red light.
  delay(10);
}
