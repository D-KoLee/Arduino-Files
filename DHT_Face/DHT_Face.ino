#include <DHT.h>
#define DHTPIN 10
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const int datapin = 10;
const int latchpin = 8;
const int clockpin = 7;
int row;
int col;

int colval[8] = {1, 2, 4, 8, 16, 32, 64, 128};
int smile[8] = {B11000011, B10111101, B01010110, B01111010, B01111010, B01010110, B10111101, B11000011};
int angry[8] = {B11000011, B10111101, B01011010, B01110110, B01110110, B01011010, B10111101, B11000011};
int heart[8] = {B10011111, B01100111, B10111001, B11011110, B11011110, B10111001, B01100111, B10011111};

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(datapin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);
}

void loop() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int di = t - 0.55 * (1 - 0.01 * h) * (t - 14.5);
  Serial.print("discomportIndex : ");
  Serial.println(di);

  if (di < 21) {
    for (row = 0, col = 0; row < 8, col < 8; row++, ++col) {
      DotMatrixWrite(heart[row], colval[col]);
      delay(1);
    }
  }

  else if (21 < di && di < 24) {
    for (row = 0, col = 0; row < 8, col < 8; row++, ++col) {
      DotMatrixWrite(smile[row], colval[col]);
      delay(1);
    }
  }

  else if (di > 24) {
    for (row = 0, col = 0; row < 8, col < 8; row++, ++col) {
      DotMatrixWrite(angry[row], colval[col]);
      delay(1);
    }
  }
}

void DotMatrixWrite(int rowval, int colval) {
  digitalWrite(latchpin, LOW);
  shiftOut(datapin, clockpin, MSBFIRST, rowval);
  shiftOut(datapin, clockpin, MSBFIRST, colval);
  digitalWrite(latchpin, HIGH);
}
