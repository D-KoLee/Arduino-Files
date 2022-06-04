const int dataPin = 10; //The Serial Data Pinto the Shift Register
const int latchPin = 8; //The Latch Pin to the Shift Register
const int clockPin = 7; //The Clock Pin to the Shift Register
int state = 0;

void setup() {
  pinMode(dataPin, OUTPUT); //Configure Digital Pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  if( state == 0) {
    int rowVal = B00000000;
    int colVal = B11111111;
    DotMatrixWrite(rowVal, colVal);
    delay(500);
    state = 1;
  }
  else {
    int rowVal = B00000000;
    int colVal = B11111111;
    DotMatrixWrite(rowVal, ~colVal);
    delay(500);
    state = 0;
  }
}

void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW); //Pull latch LOW to send data
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal); //send the row data byte. Row data should be sent first
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}
