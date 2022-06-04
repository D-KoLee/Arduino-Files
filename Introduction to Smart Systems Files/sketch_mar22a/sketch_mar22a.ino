#include <Adafruit_NeoPixel.h>
#include <rp2040_pio.h>
int led = 9l

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  for(int brightness = 0; brightness <= 255; brightness += 5) {
    analogWrite(led, brightness);
    delay(30);
  }
  for(int brightness = 255; brightness >= 0; brightness -= 5) {
    analogWrite(led, brightness);
    delay(30);
  }
  
}
