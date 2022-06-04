#include <IRremote.h>
#include <Adafruit_NeoPixel.h>

#define LED 11
#define Br 30

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

int colVal[8] = {0, 0, 4, 8, 16, 32, 64, 0};
int rowVal[8] = {255, 255, 255, 255, 255, 255, 255, 255};

int remoteSensor = 3;
const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;
int button = 4;
int pot_pin = A5;
int bright = 100;     //control RGB brightness
int color = 0;        //0 == R, 1 == G, 2 == B
boolean LED_On = true;  //LED(RGB) On and Off control
boolean DotOn = true;
boolean cg = false;   //Change, detect the change of Serial or IR
int state = 0;        //check the number
int SerialRead = 0;
boolean previousSW = HIGH;
boolean currentSW = HIGH;
char STATE[6] = {0, 1, 2, 3, 5, 8};
IRrecv irrecv(remoteSensor);
decode_results results;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(button, INPUT);
  pinMode(pot_pin, INPUT);
  pixels.begin();
  pixels.show();
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  setRow(0);
}

void loop() {
  int mapped_pot_output = map(analogRead(pot_pin), 0, 1023, 0, 255);
  Serial.println(mapped_pot_output);

  currentSW = debounce(previousSW, button);
  if (previousSW == HIGH && currentSW == LOW) {
    DotOn = !DotOn;
  }
  previousSW = currentSW;

  if (irrecv.decode(&results)) {
    if (results.decode_type == NEC) {
      delay(50);
      switch (results.value) {
        case 0x00FF6897://0
          state = 0;
          break;
        case 0x00FF30CF://1
          state = 1;
          break;
        case 0x00FF18E7://2
          state = 2;
          break;
        case 0x00FF7A85://3
          state = 3;
          break;
        case 0x00FF38C7://5
          state = 4;
          break;
        case 0x00FF4AB5://8
          state = 5;
          break;
        default:
          break;
      }
      cg = !cg;
    }
    irrecv.resume();
  }

  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == '0') {
      state = 0;
    }
    else if (input == '1') {
      state = 1;
    }
    else if (input == '2') {
      state = 2;
    }
    else if (input == '3') {
      state = 3;
    }
    else if (input == '5') {
      state = 4;
    }
    else if (input == '8') {
      state = 5;
    }
    cg = !cg;
    delay(5);
  }

  if (cg) {
    switch (STATE[state]) {
      case 0:
        setRow(3);
        LED_On = !LED_On;
        break;

      case 1: //1
        color = 0;
        setRow(0);
        break;

      case 2: //2
        color = 1;
        setRow(1);
        break;

      case 3: //3
        color = 2;
        setRow(2);
        break;

      case 5: //5
        if ((bright += Br) > 255) bright = 255;
        break;

      case 8: //8
        if ((bright -= Br) < 0) bright = 0;
        break;

      default:
        break;
    }
    cg = !cg;
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
  if (DotOn) {
    for (int colrow = 0; colrow < 8; colrow++) {
      DotMatrixWrite(rowVal[colrow], colVal[colrow]);
    }
  }
  else DotMatrixWrite(0, 0);
}

void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal);
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}

boolean debounce (boolean previous, int butt) {
  boolean current = digitalRead(butt);
  if (previous != current) {
    delay(5);
    current = digitalRead(butt);
  }
  return current;
}

void setRow(int color) {
  int i = 2;
  
  if (color == 0) { //R
    rowVal[i] = 1;
    rowVal[++i] = 111;
    rowVal[++i] = 111;
    rowVal[++i] = 111;
    rowVal[++i] = 145;
  }
  else if (color == 1) {//G
    rowVal[i] = 131;
    rowVal[++i] = 125;
    rowVal[++i] = 125;
    rowVal[++i] = 109;
    rowVal[++i] = 227;
  }
  else if (color == 2) {//B
    rowVal[i] = 1;
    rowVal[++i] = 109;
    rowVal[++i] = 109;
    rowVal[++i] = 109;
    rowVal[++i] = 147;
  }
  else if (color == 3) {//X
    rowVal[i] = 57;
    rowVal[++i] = 215;
    rowVal[++i] = 239;
    rowVal[++i] = 215;
    rowVal[++i] = 57;
  }
  else {
    rowVal[i] = 255; //모두 끄기
    rowVal[++i] = 255;
    rowVal[++i] = 255;
    rowVal[++i] = 255;
    rowVal[++i] = 255;
  }
}
