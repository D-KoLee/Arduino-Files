#include <IRremote.h>
#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1,LED,NEO_GRB + NEO_KHZ800);


int remoteSensor = 6;
int incomingByte = 0;
int state = 0;
int brightness = 50;
int color = 0;
boolean sc = false;
boolean LedOn = false;
boolean DotOn = false;
int number = 0;
const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;
int colVal[8] = {1,2,4,8,16,32,64,128};
const int num[30] = {
  1, 125, 1,
  189, 1, 253,
  97, 109, 13,
  109, 109, 1,
  15, 239, 1,
  13, 109, 97,
  1, 109, 97,
  127, 127, 1,
  1, 109, 1,
  13, 109, 1,
};
int rowVal[8] = {255,255,255,255,255,255,255,255};


IRrecv irrecv (remoteSensor);

decode_results results;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pixels.begin();
  pixels.show();
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  for(int row = 1; row <4; row++) rowVal[row] = num[row-1];
  for(int row = 5; row <8; row++) rowVal[row] = num[row-5];
}

void loop() {
  if(irrecv.decode(&results)) {
    if(results.decode_type == NEC) {
      delay(100);
      
      switch(results.value) {
        case 0x00FF6897:
          state = 0;
        break;
        case 0x00FF30CF:
          state = 1;
        break;
        case 0x00FF18E7:
          state = 2;
        break;
        case 0x00FF7A85:
          state = 3;
        break;
        case 0x00FF10EF:
          state = 4;
        break;
        case 0x00FF38C7:
          state = 5;
        break;
        case 0x00FF5AA5:
          state = 6;
        break;
        case 0x00FF42BD:
          state = 7;
        break;
        case 0x00FF4AB5:
          state = 8;
        break;
        case 0x00FF52AD:
          state = 9;
        break;
        case 0x00FFA25D:
          state = 10;
        break;
        default:
        break;
      }
      sc = !sc;
    }
    irrecv.resume();
  }

  if(Serial.available() > 0) {
    incomingByte = Serial.read();
    if(incomingByte >= 48){
      state = incomingByte-48;
      sc = !sc;
    }
  }

  if(sc) {

  switch(state) {
    case 0:
      Serial.println("press '0'");
      LedOn = !LedOn;
      number = 0;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 1:
      Serial.println("press '1'");
      color = 0;
      number = 1;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 2:
      Serial.println("press '2'");
      color = 1;
      number = 2;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 3:
      Serial.println("press '3'");
      color = 2;
      number = 3;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 4:
      Serial.println("press '4'");
      number = 4;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 5:
      Serial.println("press '5'");
      if(brightness <= 255) brightness += 5;
      number = 5;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 6:
      Serial.println("press '6'");
      number = 6;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 7:
      Serial.println("press '7'");
      number = 7;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 8:
      Serial.println("press '8'");
      if(brightness >= 0) brightness -= 5;
      number = 8;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 9:
      Serial.println("press '9'");
      number = 9;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 10:
      Serial.println("press 'Power'");
      DotOn = !DotOn;
    break;
    default:
    break;
  }
  sc = !sc;
  }

  if(LedOn) {
    if(color == 0){
      pixels.setPixelColor(0,brightness,0,0);
      pixels.show();
    }else if(color == 1){
      pixels.setPixelColor(0,0,brightness,0);
      pixels.show();
    }else if(color == 2){
      pixels.setPixelColor(0,0,0,brightness);
      pixels.show();
    }
  } else {
    pixels.setPixelColor(0,0,0,0);
    pixels.show();
  }
  if(DotOn) for(int col = 0; col <8; col++) DotMatrixWrite(rowVal[col],colVal[col]);
  else DotMatrixWrite(255,255);
  
}


void DotMatrixWrite(int rowVal, int colVal){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal);
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}#include <IRremote.h>
#include <Adafruit_NeoPixel.h>

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1,LED,NEO_GRB + NEO_KHZ800);


int remoteSensor = 6;
int incomingByte = 0;
int state = 0;
int brightness = 50;
int color = 0;
boolean sc = false;
boolean LedOn = false;
boolean DotOn = false;
int number = 0;
const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;
int colVal[8] = {1,2,4,8,16,32,64,128};
const int num[30] = {
  1, 125, 1,
  189, 1, 253,
  97, 109, 13,
  109, 109, 1,
  15, 239, 1,
  13, 109, 97,
  1, 109, 97,
  127, 127, 1,
  1, 109, 1,
  13, 109, 1,
};
int rowVal[8] = {255,255,255,255,255,255,255,255};


IRrecv irrecv (remoteSensor);

decode_results results;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pixels.begin();
  pixels.show();
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  for(int row = 1; row <4; row++) rowVal[row] = num[row-1];
  for(int row = 5; row <8; row++) rowVal[row] = num[row-5];
}

void loop() {
  if(irrecv.decode(&results)) {
    if(results.decode_type == NEC) {
      delay(100);
      
      switch(results.value) {
        case 0x00FF6897:
          state = 0;
        break;
        case 0x00FF30CF:
          state = 1;
        break;
        case 0x00FF18E7:
          state = 2;
        break;
        case 0x00FF7A85:
          state = 3;
        break;
        case 0x00FF10EF:
          state = 4;
        break;
        case 0x00FF38C7:
          state = 5;
        break;
        case 0x00FF5AA5:
          state = 6;
        break;
        case 0x00FF42BD:
          state = 7;
        break;
        case 0x00FF4AB5:
          state = 8;
        break;
        case 0x00FF52AD:
          state = 9;
        break;
        case 0x00FFA25D:
          state = 10;
        break;
        default:
        break;
      }
      sc = !sc;
    }
    irrecv.resume();
  }

  if(Serial.available() > 0) {
    incomingByte = Serial.read();
    if(incomingByte >= 48){
      state = incomingByte-48;
      sc = !sc;
    }
  }

  if(sc) {

  switch(state) {
    case 0:
      Serial.println("press '0'");
      LedOn = !LedOn;
      number = 0;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 1:
      Serial.println("press '1'");
      color = 0;
      number = 1;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 2:
      Serial.println("press '2'");
      color = 1;
      number = 2;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 3:
      Serial.println("press '3'");
      color = 2;
      number = 3;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 4:
      Serial.println("press '4'");
      number = 4;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 5:
      Serial.println("press '5'");
      if(brightness <= 255) brightness += 5;
      number = 5;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 6:
      Serial.println("press '6'");
      number = 6;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 7:
      Serial.println("press '7'");
      number = 7;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 8:
      Serial.println("press '8'");
      if(brightness >= 0) brightness -= 5;
      number = 8;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 9:
      Serial.println("press '9'");
      number = 9;
      for(int row = 5; row <8; row++) rowVal[row] = num[3*number+row-5];
    break;
    case 10:
      Serial.println("press 'Power'");
      DotOn = !DotOn;
    break;
    default:
    break;
  }
  sc = !sc;
  }

  if(LedOn) {
    if(color == 0){
      pixels.setPixelColor(0,brightness,0,0);
      pixels.show();
    }else if(color == 1){
      pixels.setPixelColor(0,0,brightness,0);
      pixels.show();
    }else if(color == 2){
      pixels.setPixelColor(0,0,0,brightness);
      pixels.show();
    }
  } else {
    pixels.setPixelColor(0,0,0,0);
    pixels.show();
  }
  if(DotOn) for(int col = 0; col <8; col++) DotMatrixWrite(rowVal[col],colVal[col]);
  else DotMatrixWrite(255,255);
  
}


void DotMatrixWrite(int rowVal, int colVal){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal);
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}
