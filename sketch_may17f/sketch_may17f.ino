
#include "DHT.h"

#define dht11_pin 5

DHT dht11(dht11_pin, DHT11);

void setup() {
  // put your setup code here, to run once:
  dht11.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  dht11_read();
  delay(2000);
}
void dht11_read() {
  int h = dht11.readHumidity();
  int t = dht11.readTemperature();

  Serial.print("H: ");Serial.println(h);
  Serial.print("T: ");Serial.println(t);

  Serial.println("");
}
