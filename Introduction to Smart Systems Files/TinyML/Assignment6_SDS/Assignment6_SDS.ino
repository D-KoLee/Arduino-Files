#include <IRremote.h>
#include <Adafruit_NeoPixel.h>
#include "myann.h"
#include <stdio.h>
#include <stdarg.h>
#include <PrintEx.h>
PrintEx myPrint = Serial;

#define LED 11

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

unsigned long current_time = 0;
unsigned long previous_time = 0;
int i = 0;
int remoteSensor = 3;
int incomingByte = 0;
int state = 0;
int testV = 0;
int brightness = 50;
int color = 0;
boolean state_on = false;
boolean LedOn = false;
boolean Test_on = false;
const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;
double InVal[7];
double result[4];
int Test_output = 0;
int colVal[8] = {1, 2, 4, 8, 16, 32, 64, 128};
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
int rowVal[8] = {255, 1, 125, 1, 255, 1, 125, 1};


IRrecv irrecv (remoteSensor);

decode_results results;



const int PATTERN_COUNT = 10;
const int INPUT_NODES = 7;
const int HIDDEN_NODES = 4;
const int OUTPUT_NODES = 4;

double sumError = 0.02455;

double input[PATTERN_COUNT][INPUT_NODES] = {
  { 1, 1, 1, 1, 1, 1, 0},  // 0
  { 0, 1, 1, 0, 0, 0, 0},  // 1
  { 1, 1, 0, 1, 1, 0, 1},  // 2
  { 1, 1, 1, 1, 0, 0, 1},  // 3
  { 0, 1, 1, 0, 0, 1, 1},  // 4
  { 1, 0, 1, 1, 0, 1, 1},  // 5
  { 0, 0, 1, 1, 1, 1, 1},  // 6
  { 1, 1, 1, 0, 0, 0, 0},  // 7
  { 1, 1, 1, 1, 1, 1, 1},  // 8
  { 1, 1, 1, 0, 0, 1, 1}   // 9
};
double target[PATTERN_COUNT][OUTPUT_NODES] = {
  { 0, 0, 0, 0 },
  { 0, 0, 0, 1 },
  { 0, 0, 1, 0 },
  { 0, 0, 1, 1 },
  { 0, 1, 0, 0 },
  { 0, 1, 0, 1 },
  { 0, 1, 1, 0 },
  { 0, 1, 1, 1 },
  { 1, 0, 0, 0 },
  { 1, 0, 0, 1 }
};
double hidden[HIDDEN_NODES];
double output[PATTERN_COUNT][OUTPUT_NODES];

double weightH[INPUT_NODES][HIDDEN_NODES] = {
  { -2.91703, 5.41097, -5.26981, -4.69735},
  {0.90888, -1.66985, -2.51382, -1.72516},
  {0.09756, 0.92623, 4.96570, -2.96621},
  {4.05811, -1.53781, 2.23749, 1.29215},
  {0.99279, -4.71496, -3.62960, 2.94658},
  { -4.92638, -4.20094, -0.15394, -0.82885},
  { -2.10893, -0.74264, -4.51816, 3.71595}
};
double biasH[HIDDEN_NODES] = {
  1.53899,
  -0.04174,
  4.89612,
  0.39245,
};
double weightO[HIDDEN_NODES][OUTPUT_NODES] = {
  { -5.43054, -8.08971, 2.50232, 1.39708},
  { -0.46309, 4.22896, 9.48748, 14.40990},
  { -8.30426, 8.43746, -1.36867, 0.23920},
  { -3.25865, 4.17170, 10.13540, -3.03200}
};
double biasO[OUTPUT_NODES] = {
  4.15727,
  -5.35873,
  -6.67808,
  -3.24029,
};

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pixels.begin();
  pixels.show();
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
}

void loop() {
  if (!Test_on) {
    current_time = millis();
    if (irrecv.decode(&results)) {
      if (results.decode_type == NEC && (current_time - previous_time > 100)) {
        switch (results.value) {
          case 0x00FF6897:
            state = 0; break;
          case 0x00FF30CF:
            state = 1; break;
          case 0x00FF18E7:
            state = 2; break;
          case 0x00FF7A85:
            state = 3; break;
          case 0x00FF10EF:
            state = 4; break;
          case 0x00FF38C7:
            state = 5; break;
          case 0x00FF5AA5:
            state = 6; break;
          case 0x00FF42BD:
            state = 7; break;
          case 0x00FF4AB5:
            state = 8; break;
          case 0x00FF52AD:
            state = 9; break;
          case 0x00FFA25D:
            state = 10; break;
          case 0x00FF22DD:
            state = 11; break;
          default:
            break;
        }
        state_on = true;
        previous_time = millis();
      }
      irrecv.resume();
    }

    if (Serial.available() > 0) {
      incomingByte = Serial.read();
      if (incomingByte >= 48) {
        state = incomingByte - 48;
        state_on = true;
      }
    }

    if (state_on) {
      switch (state) {
        case 0:
          LedOn = !LedOn; break;
        case 1:
          color = 0; break;
        case 2:
          color = 1; break;
        case 3:
          color = 2; break;
        case 5:
          if (brightness < 255) brightness += 5; break;
        case 8:
          if (brightness > 0) brightness -= 5; break;
        case 11:
          Test_on = true; break;
        default:
          break;
      }

      state_on = false;
    }

    if (LedOn) {
      if (color == 0) {
        pixels.setPixelColor(0, brightness, 0, 0);
        pixels.show();
      } else if (color == 1) {
        pixels.setPixelColor(0, 0, brightness, 0);
        pixels.show();
      } else if (color == 2) {
        pixels.setPixelColor(0, 0, 0, brightness);
        pixels.show();
      }
    } else {
      pixels.setPixelColor(0, 0, 0, 0);
      pixels.show();
    }

    if (state <= 9 ) {
      for (int row = 5; row < 8; row++) rowVal[row] = num[3 * state + row - 5];
      for (int col = 0; col < 8; col++) DotMatrixWrite(rowVal[col], colVal[col]);
    } else DotMatrixWrite(0, 0);
  }

  else if (Test_on = true)
  {
    current_time = millis();
    if (i <= 6) {
      if (irrecv.decode(&results)) {
        if (results.decode_type == NEC && (current_time - previous_time > 100)) {
          switch (results.value) {
            case 0x00FF6897:
              state = 0; break;
            case 0x00FF30CF:
              state = 1; break;
          }
          previous_time = millis();
          InVal[i] = state;
          Serial.println(InVal[i]);
          i++;
        }
        irrecv.resume();
      }
    }
    if (i == 7) {
      Test(InVal);
      i = 0;
      Test_on = false;
      state = Test_output;
    }
  }
}


void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal);
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}



void Test(double InVal[]) {
  int a = 1;
  Test_output = 0;
   
  for (int pc = 0; pc < PATTERN_COUNT; pc++) {

    feed_forward(input[pc],
                 (const double *)weightH,
                 biasH,
                 hidden,
                 INPUT_NODES,
                 HIDDEN_NODES,
                 SIGMOID);

    feed_forward(hidden,
                 (const double *)weightO,
                 biasO,
                 output[pc],
                 HIDDEN_NODES,
                 OUTPUT_NODES,
                 SIGMOID);

    double Error =
      get_error(target[pc], output[pc],
                OUTPUT_NODES, MSE);

    sumError += Error;
  }
  for (int i = 0; i < 10; i++) {
    int m = 0;
    for (int j = 0; j < 7; j++) {
      if (InVal[j] == input[i][j]) m++;
      if (m == 7) {
        Serial.println("find");
        for (int on = OUTPUT_NODES - 1; on >= 0; on--) {
          result[on] = output[i][on];
          if (result[on] >= 0.5) Test_output += a;
          a *= 2;
        }
        Serial.println(Test_output);
      }
    }
  }
}
