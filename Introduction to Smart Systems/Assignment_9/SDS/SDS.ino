#include <Adafruit_NeoPixel.h>
#include "DHT.h"
#define LED 11
#define dht11_pin 13

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);
DHT dht11(dht11_pin, DHT11);

const int u_pin = 12;
const int vibrating_pin = A3;
const int buzzer_pin = 2;
const int Z = 3;
const int touch_pin = 4;
const int SW = 6;
const int clockPin = 7;
const int latchPin = 8;
const int rollball_pin = 9;
const int dataPin = 10;
const int IR_pin = 5;
const int poten_pin = A5;
const int Y = A2;
const int X = A0;

boolean previousSW = HIGH;
boolean currentSW = HIGH;
boolean previousTo = HIGH;
boolean soundon = false;
boolean IRon = false;
boolean permission = true;
boolean previous_door = false;
boolean score_display = false;


int state = 0;
int Dot_state = 0;
int brightness;
int h;
int t;
int Ten;
int One;
int rowV = 0;
int colVal[8] = {1, 2, 4, 8, 16, 32, 64, 128};
byte rowVal[8] = {B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111};
byte color[24] = {
  B10000000, B10000000, B10110011, B10110001, B10000100, B11001110, //R
  B11000001, B10000000, B10111110, B10111010, B10011000, B11011001, //G
  B10000000, B10000000, B10110110, B10110110, B10000000, B11001001, //B
  B10000000, B10000000, B10110111, B10110111, B10000111, B11001111  //P
};
byte num[30] = {
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
byte smile[8] = {0x3C, 0x42, 0xA9, 0x85, 0x85, 0xA9, 0x42, 0x3C};
byte sad[8] = {0x3C, 0x42, 0xA5, 0x89, 0x89, 0xA5, 0x42, 0x3C};
byte one[8] = {0x00, 0x00, 0x10, 0x20, 0x7E, 0x00, 0x00, 0x00};
byte two[8] = {0x00, 0x00, 0x26, 0x4A, 0x52, 0x22, 0x00, 0x00};
byte three[8] = {0x00, 0x00, 0x42, 0x52, 0x52, 0x7E, 0x00, 0x00};
byte dot[8] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
byte space[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

unsigned long current_time = 0;
unsigned long previous_time = 0;
unsigned long dot_time = 0;
int incomingByte = 0;
int r = 0;
int g = 0;
int b = 0;
boolean started = false;
int posy = B00000001;
int posx = B00010000;
int dotY = 128;
int dotX[4];
boolean newDot = true;
int score = 0;
int previousScore = 0;
int speedUp = 200;

void setup() {
  pinMode(IR_pin, INPUT);
  pinMode(touch_pin, INPUT);
  pinMode(poten_pin, INPUT);
  pinMode(u_pin, INPUT);
  pinMode(vibrating_pin, INPUT);
  pinMode(rollball_pin, INPUT);
  pinMode(SW, INPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  pinMode(Z, INPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  dht11.begin();
  pixels.begin();
  pixels.show();
  previousTo = digitalRead(touch_pin);
  Serial.begin(9600);
  randomSeed(analogRead(2));
  for (int i = 0; i < 4; i++) {
    dotX[i] = randomMapping(random(1, 9));
    if (i == 1) while (dotX[i] == dotX[i - 1]) dotX[i] = randomMapping(random(1, 9));
    if (i == 2) while (dotX[i] == dotX[i - 1] || dotX[i] == dotX[i - 2]) dotX[i] = randomMapping(random(1, 9));
    if (i == 3) while (dotX[i] == dotX[i - 1] || dotX[i] == dotX[i - 2] || dotX[i] == dotX[i - 3]) dotX[i] = randomMapping(random(1, 9));
    dotY = 128;
  }
}

void loop() {
  current_time = millis();

  if (IRon) blinkingBlue();
  else {
    currentSW = debounce(previousSW);
    if (previousSW == HIGH && currentSW == LOW) {
      Dot_state++;
      if (Dot_state == 4) Dot_state = 0;
    }
    previousSW = currentSW;

    if (digitalRead(touch_pin) != previousTo) {
      state++;
      if (state > 3) state = 0;
      previousTo = digitalRead(touch_pin);
    }

    if (Serial.available() > 0) control_permission();

    if (Dot_state == 1) {
      setcolor();
      pixels.setPixelColor(0, r, g, b);
      pixels.show();
      if (!digitalRead(Z)) Dodge();
    } else {
      pixels.setPixelColor(0, 0, 0, 0);
      pixels.show();
    }

    if (!digitalRead(IR_pin)) {
      IRon = true;
      previous_time = millis();
    }
  }

  brightness = 20 - (analogRead(poten_pin) / 50);
  if (current_time - dot_time < brightness) DotMatrixWrite(0, 0);
  else {
    dot_time = current_time;
    if (score_display) {
      current_time = millis();
      if ((current_time - previous_time) < 1000) {
        Ten = score / 10;
        One = score % 10;
        rowVal[0] = 255;
        for (int row = 1; row < 4; row++) rowVal[row] = num[3 * Ten + row - 1];
        rowVal[4] = 255;
        for (int row = 5; row < 8; row++) rowVal[row] = num[3 * One + row - 5];
        for (int col = 0; col < 8; col++) DotMatrixWrite(rowVal[col], colVal[col]);
      } else {
        score_display = false;
        score = 0;
      }
    } else if (Dot_state == 1) {
      rowVal[0] = 255; rowVal [7] = 255;
      for (int row = 1; row < 7; row++) rowVal[row] = color[6 * state + row - 1];
      for (int col = 0; col < 8; col++) DotMatrixWrite(rowVal[col], colVal[col]);
    } else if (Dot_state == 2) {
      h = dht11.readHumidity();
      Ten = h / 10;
      One = h % 10;
      for (int row = 1; row < 4; row++) rowVal[row] = num[3 * Ten + row - 1];
      rowVal[4] = 255;
      for (int row = 5; row < 8; row++) rowVal[row] = num[3 * One + row - 5];
      for (int col = 0; col < 8; col++) DotMatrixWrite(rowVal[col], colVal[col]);
    } else if (Dot_state == 3) {
      t = dht11.readTemperature();
      Ten = t / 10;
      One = t % 10;
      for (int row = 1; row < 4; row++) rowVal[row] = num[3 * Ten + row - 1];
      for (int row = 5; row < 8; row++) rowVal[row] = num[3 * One + row - 5];
      for (int col = 0; col < 8; col++) DotMatrixWrite(rowVal[col], colVal[col]);
    } else DotMatrixWrite(0, 0);
  }


  //if ((digitalRead(u_pin) != previous_door) && permission) previous_door = digitalRead(u_pin);
  //else if ((digitalRead(u_pin) != previous_door) && !permission) siren(10);

  //if(analogRead(vibrating_pin) < 30) siren(10);

  //if(digitalRead(rollball_pin)) melody();
}

boolean debounce(boolean previous) {
  boolean current = digitalRead(SW);
  if (previous != current) {
    delay(5);
    current = digitalRead(SW);
  }
  return current;
}

void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal);
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
}

void siren(int repeat) {
  for (int i = 0; i < repeat; i++) {
    for (int j = 1000; j < 2000; j++) {
      tone(buzzer_pin, j, 1);
    } for (int j = 2000; j > 1000; j--) {
      tone(buzzer_pin, j, 1);
    }
  }
}

void melody() {
  tone(buzzer_pin, 2093, 500); //도
  delay(500);
  tone(buzzer_pin, 2349, 500); //레
  delay(500);
  tone(buzzer_pin, 2637, 500); //미
  delay(500);
  tone(buzzer_pin, 2793, 500); //파
  delay(500);
  tone(buzzer_pin, 2793, 500); //파
  delay(500);
  tone(buzzer_pin, 2793, 500); //파
  delay(500);
  tone(buzzer_pin, 2793, 500); //파
  delay(500);
  tone(buzzer_pin, 2637, 500); //미
  delay(500);
  tone(buzzer_pin, 2349, 500); //레
  delay(500);
  tone(buzzer_pin, 2093, 500); //도
  delay(500);
}

void Dodge() {
  int J3 = digitalRead(Z);
  startGame();
  blinkCharacter();
  started = true;

  while (started) {
    int x = map(analogRead(X), 0, 1023, -10, 10);
    int y = map(analogRead(Y), 0, 1023, -10, 10);

    if (x > -1 && posx != 128) posx = posx * 2;
    if (x < -1 && posx != 1) posx = posx / 2;
    if (y > 0 && posy != 128) posy = posy * 2;
    if (y < 0 && posy != 1) posy = posy / 2;

    current_time = millis();
    if ((current_time - previous_time) >= speedUp) {
      dotY /= 2;
      previous_time = current_time;
    }
    if (dotY >= 1) newDot = false;
    else newDot = true;

    if (newDot) {
      for (int i = 0; i < (state + 1); i++) {
        dotX[i] = randomMapping(random(1, 9));
        if (i == 1) while (dotX[i] == dotX[i - 1]) dotX[i] = randomMapping(random(1, 9));
        if (i == 2) while (dotX[i] == dotX[i - 1] || dotX[i] == dotX[i - 2]) dotX[i] = randomMapping(random(1, 9));
        if (i == 3) while (dotX[i] == dotX[i - 1] || dotX[i] == dotX[i - 2] || dotX[i] == dotX[i - 3]) dotX[i] = randomMapping(random(1, 9));
      }
      dotY = 128;
      score++;
    }

    if ((score - previousScore) == 5) {
      previousScore = score;
      if (speedUp > 50) speedUp = speedUp - 25;
    }

    Draw(posx, posy, dotX, dotY);

    for (int i = 0; i < (state + 1); i++) {
      if (posx == dotX[i] && posy == dotY) {
        tone(buzzer_pin, 2349, 250);
        delay(200);
        tone(buzzer_pin, 2349, 250);
        delay(200);
        tone(buzzer_pin, 2093, 250);
        delay(50);
        Display(sad, 1000);
        DotMatrixWrite(~0, 0);
        posx = B0001000;
        posy = B0000001;
        dotY = 128;
        started = false;
        Serial.print("my score : ");
        Serial.print(score);
        Serial.println();
        score_display = true;
        if (score > 99) score = 99;
        previous_time = millis();
        break;
      }
    }
  }
}

void Display(byte data[], int millisec) {
  long startTime = millis();
  long endTime = 0;
  while ((endTime - startTime) <= millisec) {
    for (int i = 0; i < 8; i++) {
      rowV = data[i];
      DotMatrixWrite(~rowV, colVal[i]);
    }
    endTime = millis();
  }
}

void Draw(int posx, int posy, int dotX[], int dotY) {
  boolean sameX = false;
  int xDot[4];
  int xChar = onePos(posx);
  byte screen[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  for (int i = 0; i < (state + 1); i++) {
    xDot[i] = onePos(dotX[i]);
    if (xChar == xDot[i]) {
      screen[xChar] = posy + dotY;
      sameX = true;
    }
    else {
      if (!sameX) screen[xChar] = posy;
      screen[xDot[i]] = dotY;
    }
  }

  sameX = false;
  Display(screen, 100);
}

int onePos(int x) {
  String a = String(x, BIN);
  int c = a.length() - 1;
  return c;
}

void startGame() {
  Display(smile, 1000);
  tone(buzzer_pin, 2093, 250);
  Display(three, 1000);
  tone(buzzer_pin, 2093, 250);
  Display(two, 1000);
  tone(buzzer_pin, 2093, 250);
  Display(one, 1000);
}

void blinkCharacter() {
  tone(buzzer_pin, 2349, 1200);
  for (int i = 0; i < 3; i++) {
    Display(dot, 200);
    Display(space, 200);
  }
}

int randomMapping(int dotX) {
  int d = 1;
  for (int i = 0; i < (dotX - 1); i++) d *= 2;
  return d;
}

void control_permission() {
  incomingByte = Serial.read();
  if (incomingByte == 48) {
    permission = false;
    Serial.println("문이 잠금 상태가 되었습니다.");
  }
  else if (incomingByte == 49) {
    permission = true;
    Serial.println("잠금이 해재 되었습니다.");
  }
}

void blinkingBlue() {
  if ((current_time - previous_time) < 2000) {
    if ((current_time - previous_time) % 100 < 50) {
      pixels.setPixelColor(0, 0, 0, (5 * (current_time - previous_time)) % 100 / 2);
      pixels.show();
    } else {
      pixels.setPixelColor(0, 0, 0, 500 - (5 * (current_time - previous_time)) % 100 / 2);
      pixels.show();
    }
  } else IRon = false;
}

void setcolor() {
  switch (state) {
    case 0:
      r = analogRead(poten_pin) / 4;
      g = 0;
      b = 0;
      break;
    case 1:
      r = 0;
      g = analogRead(poten_pin) / 4;
      b = 0;
      break;
    case 2:
      r = 0;
      g = 0;
      b = analogRead(poten_pin) / 4;
      break;
    case 3:
      r = analogRead(poten_pin) / 4;
      g = analogRead(poten_pin) / 16;
      b = analogRead(poten_pin) / 12;
      break;
  }
}
