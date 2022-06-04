#include <Adafruit_NeoPixel.h>
#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);
const int pot_pin = A5;
int pot_output;

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.show();
}


void loop() {
  pot_output = analogRead(pot_pin);
  int mapped_pot_output = map(pot_output, 0, 1023, 0, 255);
  Serial.println(mapped_pot_output);
  if (Serial.available() > 0) {
    char state = Serial.read();
    if (state == '1') {
      pixels.setPixelColor(0, 50, 50, 50);
      pixels.show();
    }
    if (state == '0') {
      pixels.setPixelColor(0, 0, 0, 0);
      pixels.show();
    }
    delay(50);
  }
}
