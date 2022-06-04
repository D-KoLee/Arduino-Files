#include <IRremote.h>
#include <Adafruit_NeoPixel.h>

#define LED 11
#define Br 30

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

/* //NOTE
  #define B0 0x00FF6897
  #define B1 0x00FF30CF
  #define B2 0x00FF18E7
  #define B3 0x00FF7A85
  #define B4 0x00FF10EF
  #define B5 0x00FF38C7
  #define B6 0x00FF5AA5
  #define B7 0x00FF42BD
  #define B8 0x00FF4AB5
  #define B9 0x00FF52AD
  #define PWR 0x00FFA25D
  #define ADD 0x00FF02FD
  #define SUB 0x00FF9867
  #define CLR 0x00FFB04F
  #define PREV 0x00FFE01F
  #define NEXT 0x00FF906F
  #define PLAY 0x00FFA857
  #define TEST 0x00FF22DD
  #define RTRN 0x00FFC23D
  #define MENU 0x00FFE21D
*/

int colVal[8] = {0, 2, 4, 8, 0, 32, 64, 128};
int rowVal[8] = {255, 255, 255, 255, 255, 255, 255, 255};

int remoteSensor = 3;
const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;
int bright = 100;     //control RGB brightness
int color = 0;        //0 == R, 1 == G, 2 == B
boolean LED_On = true;  //LED(RGB) On and Off control
boolean cg = false;   //Change, detect the change of Serial or IR
//boolean PW = true;    //control power on and off. But in this case, I use only OFF
int state = 0;        //check the number
int SerialRead = 0;

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
  if (irrecv.decode(&results)) {
    if (results.decode_type == NEC) {
      delay(100);
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
        default:
          break;
      }
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
      /*    PW = !PW; //If want to implement On and Off, delete annotation.
        }
              else {
                setRow(0, 10);
                setRow(10, 1);
                PW = !PW;
              } */

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
  } else {
    pixels.setPixelColor(0, 0, 0, 0);
    pixels.show();
  }

  for (int colrow = 0; colrow < 8; colrow++) {
    DotMatrixWrite(rowVal[colrow], colVal[colrow]);
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
  else i = 5;           // 1의 자릿수를 5~7row에 표현하기 위함

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
  //  else if (digit == 10) {
  //    rowVal[i] = 1;
  //    rowVal[++i] = 127;
  //    rowVal[++i] = 1;
  //  }   //If I want to TURN ON dot-matrix, use this part. Display 'ON'
  else {
    rowVal[1] = 255; //모두 끄기
    rowVal[2] = 255;
    rowVal[3] = 255;
    rowVal[5] = 255;
    rowVal[6] = 255;
    rowVal[7] = 255;
  }
}
