#include <IRremote.h>
#include <Adafruit_NeoPixel.h>
#include "myann.h"
#include <stdio.h>
#include <stdarg.h>
#include <PrintEx.h>
PrintEx myPrint = Serial;
#define LED 11
#define Br 30

const int PATTERN_COUNT = 10;
const int INPUT_NODES = 7;
const int HIDDEN_NODES = 4;
const int OUTPUT_NODES = 4;
double sumError = 0;
unsigned long pm2 = 0;
unsigned long pm = 0; //previous_Millis
unsigned long cm = 0; //current_Millis

double input[PATTERN_COUNT][INPUT_NODES] = {
  { 1, 1, 1, 1, 1, 1, 0 }, // 0
  { 0, 1, 1, 0, 0, 0, 0 }, // 1
  { 1, 1, 0, 1, 1, 0, 1 }, // 2
  { 1, 1, 1, 1, 0, 0, 1 }, // 3
  { 0, 1, 1, 0, 0, 1, 1 },
  { 1, 0, 1, 1, 0, 1, 1 },
  { 0, 0, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 0, 0, 1, 1 }
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
  { -2.91703, 5.41097, -5.26981, -4.69735 },
  { 0.90888, -1.66985, -2.51382, -1.72516 },
  { 0.09756, 0.92623, 4.96570, -2.96621 },
  { 4.05811, -1.53781, 2.23749, 1.29215 },
  { 0.99279, -4.71496, -3.62960, 2.94658 },
  { -4.92638, -4.20094, -0.15394, -0.82885 },
  { -2.10893, - 0.74264, - 4.51816, 3.71595 }
};
double biasH[HIDDEN_NODES] = {
  1.53899,
  -0.04174,
  4.89612,
  0.39245
};
double weightO[HIDDEN_NODES][OUTPUT_NODES] = {
  { -5.43054, -8.08971, 2.50232, 1.39708 },
  { -0.46309, 4.22896, 9.48748, 14.40990 },
  { -8.30426, 8.43746, -1.36867, 0.23920 },
  { -3.25865, 4.17170, 10.13540, -3.03200 }
};
double biasO[OUTPUT_NODES] = {
  4.15727,
  -5.35873,
  -6.67808,
  -3.24029
};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

int colVal[8] = {0, 2, 4, 8, 0, 32, 64, 128};
int rowVal[8] = {255, 255, 255, 255, 255, 255, 255, 255};

int remoteSensor = 3;
int UpCount = 0;
int interval = 5;
const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;
int bright = 100;     //control RGB brightness
int bright2 = 0;
int color = 0;        //0 == R, 1 == G, 2 == B
boolean LED_On = true;  //LED(RGB) On and Off control
boolean cg = false;   //Change, detect the change of Serial or IR
int state = 0;        //check the number
int SerialRead = 0;
double IR[7]; //입력받은 값 저장
double result[4]; //입력받은 값에 따라 나온 결과값을 2진수로 변환
boolean started = 0; //test가 눌렸는지 안눌렸는지 판단
int test_result = 0; //result[4]를 10진수로 바꾸어 저장

IRrecv irrecv(remoteSensor);
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
}

void loop() {
  if (started == false) {
    cm = millis();
    if (irrecv.decode(&results)) {
      if (results.decode_type == NEC && (cm - pm > 100)) {
        switch (results.value) {
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
          case 0x00FFA25D: //power
            state = 10;
            break;
          case 0x00FF22DD: //test
            state = 11; break;
          default:
            break;
        }
        pm = millis();
        cg = !cg;
      }
      irrecv.resume();
    }

    if (Serial.available() > 0) {
      SerialRead = Serial.read();
      if (SerialRead >= 48) {
        state = SerialRead - 48;
        cg = !cg;
      }
    }

    if (cg) {
      switch (state) {
        case 0:
          Serial.println("Press '0'");
          setRow(0, 10);
          setRow(0, 1);
          LED_On = !LED_On;
          break;

        case 1: //1
          Serial.println("Press '1'");
          color = 0;
          setRow(0, 10);
          setRow(1, 1);
          break;

        case 2: //2
          Serial.println("Press '2'");
          color = 1;
          setRow(0, 10);
          setRow(2, 1);
          break;

        case 3: //3
          Serial.println("Press '3'");
          color = 2;
          setRow(0, 10);
          setRow(3, 1);
          break;

        case 4: //4
          Serial.println("Press '4'");
          setRow(0, 10);
          setRow(4, 1);
          break;

        case 5: //5
          Serial.println("Press '5'");
          if ((bright += Br) > 255) bright = 255;
          setRow(0, 10);
          setRow(5, 1);
          break;

        case 6: //6
          Serial.println("Press '6'");
          setRow(0, 10);
          setRow(6, 1);
          break;

        case 7: //7
          Serial.println("Press '7'");
          setRow(0, 10);
          setRow(7, 1);
          break;

        case 8: //8
          Serial.println("Press '8'");
          if ((bright -= Br) < 0) bright = 0;
          setRow(0, 10);
          setRow(8, 1);
          break;

        case 9: //9
          Serial.println("Press '9'");
          setRow(0, 10);
          setRow(9, 1);
          break;

        case 10: //power
          Serial.println("Press POWER");
          // if (PW == 1) {
          setRow(-1, 10);
          setRow(-1, 1);
          break;

        case 11:
          started = true;
          setRow(-1,10);
          setRow(-1,1);
          break;

        default:
          break;
      }
      cg = !cg;
      Serial.println();
    }
    if (LED_On) {
      if (color == 0) {
        pixels.setPixelColor(0, bright, 0, 0);
        pixels.show();
      }
      else if (color == 1) {
        pixels.setPixelColor(0, 0, bright, 0);
        pixels.show();
      }
      else if (color == 2) {
        pixels.setPixelColor(0, 0, 0, bright);
        pixels.show();
      }
    }
    else {
      pixels.setPixelColor(0, 0, 0, 0);
      pixels.show();
    }

    for (int colrow = 0; colrow < 8; colrow++) {
      DotMatrixWrite(rowVal[colrow], colVal[colrow]);
    }
  }
  else if (started == true) {
    if (cm - pm2 >= 4.9) {
      
      pixels.setPixelColor(0, bright2, 0, 0);
      pixels.show();
      pm2 = cm;
      bright2 += interval;
      if(bright2 <=0||bright2>=255) interval = -interval;
    }
    cm = millis();
    if (UpCount <= 6) {
      if (irrecv.decode(&results)) {
        if (results.decode_type == NEC && (cm - pm > 100)) {
          switch (results.value) {
            case 0x00FF6897:
              state = 0; break;
            case 0x00FF30CF:
              state = 1; break;
          }
          pm = millis();
          IR[UpCount] = state;
          Serial.print("Input : "); Serial.println(IR[UpCount]);
          UpCount++;
        }
        irrecv.resume();
      }
    }
    if(UpCount == 7) {
      test(IR);
      UpCount = 0;
      started = false;
      state = test_result;
      cg = !cg;
    }
  }
}

void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW); //Pull latch LOW to send data
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal); //send the row data byte. Row data should be sent first
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}

void setRow(int digit, int Unit) {
  int i = 0;

  if (Unit == 10) i = 1; //10의 자릿수를 1~3row에 표현하기 위함
  else i = 5;            // 1의 자릿수를 5~7row에 표현하기 위함

  if (digit == 0) {
    rowVal[i] = 1;
    rowVal[++i] = 125;
    rowVal[++i] = 1;
  }
  else if (digit == 1) {
    rowVal[i] = 189;
    rowVal[++i] = 1;
    rowVal[++i] = 253;
  }
  else if (digit == 2) {
    rowVal[i] = 97;
    rowVal[++i] = 109;
    rowVal[++i] = 13;
  }
  else if (digit == 3) {
    rowVal[i] = 109;
    rowVal[++i] = 109;
    rowVal[++i] = 1;
  }
  else if (digit == 4) {
    rowVal[i] = 15;
    rowVal[++i] = 239;
    rowVal[++i] = 1;
  }
  else if (digit == 5) {
    rowVal[i] = 13;
    rowVal[++i] = 109;
    rowVal[++i] = 97;
  }
  else if (digit == 6) {
    rowVal[i] = 1;
    rowVal[++i] = 109;
    rowVal[++i] = 97;
  }
  else if (digit == 7) {
    rowVal[i] = 15;
    rowVal[++i] = 127;
    rowVal[++i] = 1;
  }
  else if (digit == 8) {
    rowVal[i] = 1;
    rowVal[++i] = 109;
    rowVal[++i] = 1;
  }
  else if (digit == 9) {
    rowVal[i] = 13;
    rowVal[++i] = 109;
    rowVal[++i] = 1;
  }
  else {
    rowVal[1] = 255; //모두 끄기
    rowVal[2] = 255;
    rowVal[3] = 255;
    rowVal[5] = 255;
    rowVal[6] = 255;
    rowVal[7] = 255;
  }
}

void test(double IR[]) {
  int times = 1;
  test_result = 0;

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
  for(int i = 0; i < 10; i ++) {
    int counting = 0;
    for(int j = 0; j < 7; j++) {
      if(IR[j] ==  input[i][j]) counting++;
      if(counting == 7) {
        Serial.println("");Serial.println("TinyML has been complete!");
        for(int k = 3; k >= 0; k--) {
          result[k] = output[i][k];
          if(result[k] >= 0.5) test_result += times;
          times *= 2;
        }
        Serial.println("");Serial.print("The result state is ");Serial.println(test_result);
        Serial.print("And it ");
      }
    }
  }
}
