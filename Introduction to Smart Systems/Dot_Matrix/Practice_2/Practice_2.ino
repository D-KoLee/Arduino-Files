const int dataPin = 10; //The Serial Data Pinto the Shift Register
const int latchPin = 8; //The Latch Pin to the Shift Register
const int clockPin = 7; //The Clock Pin to the Shift Register

int colVal[8] = {1, 2, 4, 8, 16, 32, 64, 128};
//Same to {B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000};
int rowVal[8] = {255, 255, 255, 255, 255, 222, 128, 254};

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  for(int col = 0; col < 8; col++) {
    DotMatrixWrite(rowVal[col], colVal[col]);
  }
}

void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW); //Pull latch LOW to send data
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal); //send the row data byte. Row data should be sent first
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}
