#include <Adafruit_NeoPixel.h>

#define LED 11
#define Buzzer 6

int button = 12;
int sound_sensor = A2;
int IR = 9;
int Touch = A4;
int potentiometer = A5;
const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;
int VibSensor = A3;
int USensor = 13;

unsigned long cm = 0;
unsigned long pm = 0;
unsigned long pm2 = 0;

int colVal[8] = {1, 2, 4, 8, 16, 32, 64, 128};
int rowVal[8] = {255, 255, 255, 255, 255, 255, 255, 255};
int RowVal = 0;
int Siren[10] = {1046, 1174, 1318, 1396, 1567, 1760, 1975, 2093, 0, 0};

byte smile[8] = {0x3C, 0x42, 0xA9, 0x85, 0x85, 0xA9, 0x42, 0x3C};
byte sad[8] = {0x3C, 0x42, 0xA5, 0x89, 0x89, 0xA5, 0x42, 0x3C};
byte one[8] = {0x00, 0x00, 0x10, 0x20, 0x7E, 0x00, 0x00, 0x00};
byte two[8] = {0x00, 0x00, 0x26, 0x4A, 0x52, 0x22, 0x00, 0x00};
byte three[8] = {0x00, 0x00, 0x42, 0x52, 0x52, 0x7E, 0x00, 0x00};
byte dot[8] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
byte space[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

boolean previousSW = HIGH;
boolean currentSW = HIGH;
boolean push = true; //default on
boolean detect = false; //default not detected
boolean sound = false; //default no big sound
boolean TOUCH = digitalRead(Touch);
boolean permission = true;
boolean Ir = 0;
int ir_count = 0;
boolean Usensor;
boolean openclose = false;
boolean siren = false;
int state = 0;
int Red, Blue, Green;
int X = A0;
int Y = A1;
int Z = 3;
int i = 0; //Siren room number
int kount = 1; //count room
boolean started = false;
int posy = B00000001;
int posx = B00010000;
int dotY = 128;
int dotX;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
boolean newDot = false;
int score = 0;
int previousScore = 0;
int speedUp = 20;

void setup() {
  pinMode(sound_sensor, INPUT);
  pinMode(IR, INPUT);
  pinMode(Touch, INPUT);
  pinMode(potentiometer, INPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(VibSensor, INPUT);
  pinMode(USensor, INPUT);
  pixels.begin();
  pixels.show();
  DotMatrixWrite(~0, 0);

  pinMode(Buzzer, OUTPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  pinMode(Z, INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(2));
  dotX = randomMapping(random(1, 9));
}

void loop() {
  cm = millis();

  if ( analogRead(VibSensor) < 100) {
    Serial.println("지진 감지됨");
    siren = !siren;
  }
  if (openclose != (Usensor = digitalRead(USensor))) {
    if (permission == 0) {
      siren = !siren;
    }
    openclose = !openclose;
    if(openclose == 0) Serial.println("문 닫힘");
    else if(openclose == 1) Serial.println("문 열림");
  }

  if (siren && (cm - pm2) >= 10) {
    tone(Buzzer, Siren[i], 8);
    i ++;
    if (i == 10) {
      kount++;
      i = 0;
      if ( kount == 11) {
        siren = !siren;
        kount = 1;
        Serial.println("경보 발생됨");
      }
    }
    pm2 = millis();
  }
  int J3 = digitalRead(Z);
  
  if (analogRead(sound_sensor) > 500) {
    sound = 1;
    if (siren) sound = 0;
    pm = millis();
  }
  if (!digitalRead(IR)) {
    detect = 1;
    pm = millis();
  }
  if(Ir != digitalRead(IR)) {
    Ir = digitalRead(IR);
    ir_count++;
    if(ir_count > 0 && ir_count%2 == 0) {
      permission = !permission;
    }
    if(Ir == 0&&permission == 1)
      Serial.println("경비 해제됨");
    else if (Ir == 0&&permission == 0)
      Serial.println("경비 설정됨");
    
  }
  if (sound == 1) {
    if ((cm - pm) < 2000) {
      if ((cm - pm) % 100 < 50) {
        pixels.setPixelColor(0, 50, 0, 0);
        pixels.show();
      }
      else {
        pixels.setPixelColor(0, 0, 0, 0);
        pixels.show();
      }
    }
    else
      sound = !sound;
  }
  else if (detect == 1) {
    if ((cm - pm) < 2000) {
      if ((cm - pm) % 100 < 50) {
        pixels.setPixelColor(0, 0, 0, 50);
        pixels.show();
      }
      else {
        pixels.setPixelColor(0, 0, 0, 0);
        pixels.show();
      }
    }
    else {
      detect = !detect;
    }
  }
  else {
    currentSW = debounce(previousSW, button);
    if (previousSW == HIGH && currentSW == LOW) {
      push = !push;
    }
    previousSW = currentSW;

    if (digitalRead(Touch) != TOUCH) {
      ++state;
      if (state == 4) state = 0;
      TOUCH = digitalRead(Touch);
    }

    if (state == 0) {
      Red = map(analogRead(potentiometer), 0, 1023, 0, 255);
      Blue = Green = 0;
      setRow(0);
    }
    else if (state == 1) {
      Green = map(analogRead(potentiometer), 0, 1023, 0, 255);
      Red = Blue = 0;
      setRow(1);
    }
    else if (state == 2) {
      Blue = map(analogRead(potentiometer), 0, 1023, 0, 255);
      Red = Green = 0;
      setRow(2);
    }
    else if (state == 3) {
      Red = analogRead(potentiometer) / 4;
      Green = 0;
      Blue = analogRead(potentiometer) / 8;
      setRow(3);
    }

    if (push) {
      pixels.setPixelColor(0, Red, Green, Blue);
      pixels.show();
    }
    else {
      pixels.setPixelColor(0, 0, 0, 0);
      pixels.show();
      setRow(-1);
    }
  }
  if (push && !started) {
    for (int colrow = 0; colrow < 8; colrow++) {
      DotMatrixWrite(rowVal[colrow], colVal[colrow]);
    }
  }

  if (J3 == 0 && started == false) {
    startGame();
    blinkCharacter();
    started = true;
  }
  if (started) {
    int x = map(analogRead(X), 0, 1023, -10, 10);
    int y = map(analogRead(Y), 0, 1023, -10, 10);

    if (x > 0 && posx != 128) posx = posx * 2;
    if (x < -1 && posx != 1) posx = posx / 2;
    if (y > 0 && posy != 128) posy = posy * 2;
    if (y < -1 && posy != 1) posy = posy / 2;

    currentMillis = millis();
    if ((currentMillis - previousMillis) >= speedUp) {
      dotY = dotY / 2;
      previousMillis = currentMillis;
    }
    if (dotY >= 1) newDot = false;
    else newDot = true;

    if (newDot) {
      dotX = randomMapping(random(1, 9));
      dotY = 128;
      score++;
      //Serial.println(score);
    }

    if ((score - previousScore) == 5) {
      previousScore = score;

      if (speedUp > 50) speedUp = speedUp - 25;
    }

    Draw(posx, posy, dotX, dotY);

    if (posx == dotX && posy == dotY) {
      tone(Buzzer, 2349, 250);
      delay(200);
      tone(Buzzer, 2349, 250);
      delay(200);
      tone(Buzzer, 2093, 250);
      delay(50);
      Display(sad, 1000);
      DotMatrixWrite(~0, 0);
      posx = B0001000;
      posy = B0000001;
      dotY = 128;
      started = false;
      Serial.print("Score : ");Serial.println(score*100);
      score = 0;
    }
  }
}

boolean debounce (boolean previous, int butt) {
  boolean current = digitalRead(butt);
  if (previous != current) {
    delay(5);
    current = digitalRead(butt);
  }
  return current;
}

void DotMatrixWrite(int rowVal, int colVal) {
  digitalWrite(latchPin, LOW); 
  shiftOut(dataPin, clockPin, MSBFIRST, rowVal); 
  shiftOut(dataPin, clockPin, MSBFIRST, colVal);
  digitalWrite(latchPin, HIGH);
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
  else if (color == 3) {//P
    rowVal[i] = 1;
    rowVal[++i] = 111;
    rowVal[++i] = 111;
    rowVal[++i] = 111;
    rowVal[++i] = 159;
  }
  else {
    rowVal[i] = 255; //모두 끄기
    rowVal[++i] = 255;
    rowVal[++i] = 255;
    rowVal[++i] = 255;
    rowVal[++i] = 255;
  }
}

void Display(byte data[], int millisec) {
  long startTime = millis();
  long endTime = 0;
  while ((endTime - startTime) <= millisec) {
    for (int i = 0; i < 8; i++) {
      RowVal = data[i];
      DotMatrixWrite(~RowVal, colVal[i]);
    }
    endTime = millis();
  }
}


void Draw(int posx, int posy, int dotX, int dotY) {
  int xChar = onePos(posx);
  int xDot = onePos(dotX);
  byte screen[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  if (xChar == xDot) screen[xChar] = posy + dotY;
  else {
    screen[xChar] = posy;
    screen[xDot] = dotY;
  }
  Display(screen, 100);
}


int onePos(int x) {
  String a = String(x, BIN);
  int c = a.length() - 1;
  return c;
}

void startGame() {
  Display(smile, 1000);
  tone(Buzzer, 3136, 250);
  Display(three, 1000);
  tone(Buzzer, 3136, 250);
  Display(two, 1000);
  tone(Buzzer, 3136, 250);
  Display(one, 1000);
}

void blinkCharacter() {
  tone(Buzzer, 4186, 1200);
  for (int i = 0; i < 3; i++) {
    Display(dot, 200);
    Display(space, 200);
  }
}


int randomMapping(int dotX) {
  switch (dotX) {
    case 1: dotX = 1; break;
    case 2: dotX = 2; break;
    case 3: dotX = 4; break;
    case 4: dotX = 8; break;
    case 5: dotX = 16; break;
    case 6: dotX = 32; break;
    case 7: dotX = 64; break;
    case 8: dotX = 128; break;
  }
  return dotX;
}
