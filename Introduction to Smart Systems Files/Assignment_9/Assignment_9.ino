#include <Adafruit_NeoPixel.h>
#include "DHT.h"
#include "pitches.h"
#define dht11_pin  5
#define LED 11
#define Buzzer 6

float bpm = 120; // 노래의 BPM
float shift = 60 / bpm * 250; // 한 음 재생 시간
int notes[] = {
  NOTE_A5, NOTE_A5, NOTE_A5, NOTE_G5, NOTE_A5,
  NOTE_B5, NOTE_C6, NOTE_D6, NOTE_CS6, NOTE_D6,
  NOTE_E6, NOTE_C6
};
float times[] = {
  3 * shift, 1 * shift, 4 * shift, 3 * shift,
  1 * shift, 2 * shift, 2 * shift, 3 * shift,
  1 * shift, 2 * shift, 2 * shift, 6 * shift
};
int button = 12;
int IR = 9;
int Touch = A4;
int potentiometer = A5;
const int dataPin = 10;
const int latchPin = 8;
const int clockPin = 7;
const int ballsensor = 4;
int VibSensor = A3;
int USensor = 13;

unsigned long cm = 0;
unsigned long pm = 0;
unsigned long pm2 = 0;
unsigned long pm3 = 0;
unsigned long dot_bright = 0;

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

DHT dht11(dht11_pin, DHT11);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

boolean previousSW = HIGH;
boolean currentSW = HIGH;
int state = 0; //default on
boolean detect = false; //default not detected
boolean TOUCH = digitalRead(Touch);
boolean permission = true;
boolean Ir = 0;
int ir_count = 0;
boolean Usensor;
boolean openclose = false;
boolean siren = false;
boolean score_draw = false;
int led_color = 0;
int Red, Blue, Green;
int brightness;
int X = A0;
int Y = A1;
int Z = 3;
int i = 0; //Siren room number
int pre_h = 0;
int pre_t = 0;
int kount = 1; //count room
boolean started = false;
int posy = B00000001;
int posx = B00010000;
int dotY = 128;
int dotX[4];//int dotX; //4개가 동시에 뜨도록 작성
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
boolean newDot = false;
int score = 0;
int Draw_Score = 0;
int previousScore = 0;
int speedUp = 20;

void setup() {
  pinMode(IR, INPUT);
  pinMode(Touch, INPUT);
  pinMode(potentiometer, INPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(VibSensor, INPUT);
  pinMode(ballsensor, INPUT);

  pinMode(USensor, INPUT);
  pixels.begin();
  pixels.show();
  DotMatrixWrite(~0, 0);
  dht11.begin();

  pinMode(Buzzer, OUTPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
  pinMode(Z, INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(2));
  for (int i = 0; i < 4; i++) {
    dotX[i] = randomMapping(random(1, 9));
    if (i == 1)
      while (dotX[i] == dotX[i - 1])
        dotX[i] = randomMapping(random(1, 9));
    if (i == 2)
      while (dotX[i] == dotX[i - 1] || dotX[i] == dotX[i - 2])
        dotX[i] = randomMapping(random(1, 9));
    if (i == 3)
      while (dotX[i] == dotX[i - 1] || dotX[i] == dotX[i - 2] || dotX[i] == dotX[i - 3])
        dotX[i] = randomMapping(random(1, 9));
  }
}

void loop() {
  cm = millis();
  int J3 = digitalRead(Z);

  if ( analogRead(VibSensor) < 100) {
    Serial.println("지진 감지됨");
    siren = !siren;
  }
  else if (openclose != (Usensor = digitalRead(USensor))) {
    if (permission == 0) {
      siren = !siren;
    }
    openclose = !openclose;
    if (openclose == 0) Serial.println("문 닫힘");
    else if (openclose == 1) Serial.println("문 열림");
  }
  else if (digitalRead(ballsensor) > 0.5) {
    Serial.println("I am tilted!");
       Melody();
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

  if (!digitalRead(IR)) {
    detect = 1;
    pm = millis();
  }
  if (Ir != digitalRead(IR)) {
    Ir = digitalRead(IR);
    ir_count++;
    if (ir_count > 0 && ir_count % 2 == 0) {
      permission = !permission;
    }
    if (Ir == 0 && permission == 1)
      Serial.println("경비 해제됨");
    else if (Ir == 0 && permission == 0)
      Serial.println("경비 설정됨");

  }
  if (detect == 1) {
    if ((cm - pm) < 2000) {
      if ((cm - pm) % 100 < 50) {
        pixels.setPixelColor(0, 0, 0, 50);
        pixels.show();
      }
      else {
        pixels.setPixelColor(0, 0, 0, 0);
        pixels.show();
      }
      if (!started && state != 3) {
        for (int colrow = 0; colrow < 8; colrow++) {
          DotMatrixWrite(rowVal[colrow], colVal[colrow]);
        }
      }
    }
    else {
      detect = !detect;
    }
  }
  else {
    currentSW = debounce(previousSW, button);
    if (previousSW == HIGH && currentSW == LOW) {
      ++state;
      if (state == 4) state = 0;
    }
    previousSW = currentSW;

    brightness = 20 - (analogRead(potentiometer) / 50);
    if (cm - dot_bright < brightness) DotMatrixWrite(0, 0);
    else {
      dot_bright = cm;
      if (score_draw) {
        cm = millis();
        if ((cm - pm) <= 1000) {
          SetRow(-1, score / 10, 10, 1);
          SetRow(-1, score % 10, 1, 0);
          if (analogRead(potentiometer) < 3)  SetRow(-1, 0, 0, 1);
          else if (!started) {
            for (int colrow = 0; colrow < 8; colrow++) {
              DotMatrixWrite(rowVal[colrow], colVal[colrow]);
            }
          }
        }
        else {
          score_draw = false;
          score = 0;
        }
      }

      else if (state == 0) { //defualt
        if (digitalRead(Touch) != TOUCH) {
          ++led_color;
          if (led_color == 4) led_color = 0;
          TOUCH = digitalRead(Touch);
        }

        if (led_color == 0) {
          Red = map(analogRead(potentiometer), 0, 1023, 0, 255);
          Blue = Green = 0;
          SetRow(0, 0, 0, 1);
          if (analogRead(potentiometer) < 3)  SetRow(-1, 0, 0, 1);
          else if (!started) {
            for (int colrow = 0; colrow < 8; colrow++) {
              DotMatrixWrite(rowVal[colrow], colVal[colrow]);
            }
          }
        }
        else if (led_color == 1) {
          Green = map(analogRead(potentiometer), 0, 1023, 0, 255);
          Red = Blue = 0;
          SetRow(1, 0, 0, 1);
          if (analogRead(potentiometer) < 3)  SetRow(-1, 0, 0, 1);
          else if (!started) {
            for (int colrow = 0; colrow < 8; colrow++) {
              DotMatrixWrite(rowVal[colrow], colVal[colrow]);
            }
          }
        }
        else if (led_color == 2) {
          Blue = map(analogRead(potentiometer), 0, 1023, 0, 255);
          Red = Green = 0;
          SetRow(2, 0, 0, 1);
          if (analogRead(potentiometer) < 3)  SetRow(-1, 0, 0, 1);
          else if (!started) {
            for (int colrow = 0; colrow < 8; colrow++) {
              DotMatrixWrite(rowVal[colrow], colVal[colrow]);
            }
          }
        }
        else if (led_color == 3) {
          Red = analogRead(potentiometer) / 4;
          Green = 0;
          Blue = analogRead(potentiometer) / 8;
          SetRow(3, 0, 0, 1);
          if (analogRead(potentiometer) < 3)  SetRow(-1, 0, 0, 1);
          else if (!started) {
            for (int colrow = 0; colrow < 8; colrow++) {
              DotMatrixWrite(rowVal[colrow], colVal[colrow]);
            }
          }
        }
        pixels.setPixelColor(0, Red, Green, Blue);
        pixels.show();
      }
      else if (state == 1) {
        pixels.setPixelColor(0, 0, 0, 0);
        pixels.show();
        if ((cm - pm3) >= 10) {
          int h = dht11_h();
          SetRow(-1, h / 10, 10, 1);
          SetRow(-1, h % 10, 1, 0);
          if (pre_h != h) {
            Serial.print("Humidity: "); Serial.print(h); Serial.println("%");
            pre_h = h;
          }
          if (analogRead(potentiometer) < 3)  SetRow(-1, 0, 0, 1);
          else if (!started) {
            for (int colrow = 0; colrow < 8; colrow++) {
              DotMatrixWrite(rowVal[colrow], colVal[colrow]);
            }
          }
          pm3 = millis();
        }
      }
      else if (state == 2) {
        pixels.setPixelColor(0, 0, 0, 0);
        pixels.show();
        if ((cm - pm3) >= 10) {
          int t = dht11_t();
          SetRow(-1, t / 10, 10, 1);
          SetRow(-1, t % 10, 1, 0);
          if (pre_t != t) {
            Serial.print("Temperature: "); Serial.print(t); Serial.println("°C");
            pre_t = t;
          }
          if (analogRead(potentiometer) < 3)  SetRow(-1, 0, 0, 1);
          else if (!started) {
            for (int colrow = 0; colrow < 8; colrow++) {
              DotMatrixWrite(rowVal[colrow], colVal[colrow]);
            }
          }
          pm3 = millis();
        }
      }
      else if (state == 3) {
        pixels.setPixelColor(0, 0, 0, 0);
        pixels.show();
        pre_h = pre_t = 0;

        DotMatrixWrite(0, 0);

      }
    }
  }
  if (J3 == 0 && started == false && analogRead(potentiometer) > 10) {
    Serial.println("Game Start!");
    startGame();
    blinkCharacter();
    started = true;
  }
  if (started) {
    int x = map(analogRead(X), 0, 1023, -10, 10);
    int y = map(analogRead(Y), 0, 1023, -10, 10);

    if (x > 0 && posx != 128) posx *= 2;
    if (x < -1 && posx != 1) posx /= 2;
    if (y > 0 && posy != 128) posy *= 2;
    if (y < -1 && posy != 1) posy /= 2;

    currentMillis = millis();
    if ((currentMillis - previousMillis) >= speedUp) {
      dotY /= 2;
      previousMillis = currentMillis;
    }
    if (dotY >= 1) newDot = false;
    else newDot = true;

    if (newDot) {
      for (int i = 0; i < (led_color + 1); i++) {
        dotX[i] = randomMapping(random(1, 9));
        if (i == 1)
          while (dotX[i] == dotX[i - 1])
            dotX[i] = randomMapping(random(1, 9));
        if (i == 2)
          while (dotX[i] == dotX[i - 1] || dotX[i] == dotX[i - 2])
            dotX[i] = randomMapping(random(1, 9));
        if (i == 3)
          while (dotX[i] == dotX[i - 1] || dotX[i] == dotX[i - 2] || dotX[i] == dotX[i - 3])
            dotX[i] = randomMapping(random(1, 9));
      }
      dotY = 128;
      score++;
    }

    if ((score - previousScore) == 5) {
      previousScore = score;

      if (speedUp > 50) speedUp = speedUp - 25;
    }

    Draw(posx, posy, dotX, dotY);

    for (int i = 0; i < (led_color + 1); i++) {
      if (posx == dotX[i] && posy == dotY) {
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
        Serial.print("Score : "); Serial.println(score);
        score_draw = true;
        if (score >= 99) score = 99;
        pm = millis();
        break;
      }
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

void SetRow(int color, int digit, int Unit, boolean reset) {
  int i = 2;
  boolean set_color = 0;

  if (reset == 1) for (int k = 0; k < 8; k++) rowVal[k] = 255;

  if (Unit == 10) i = 0;      //10의 자릿수를 1~3row에 표현하기 위함
  else if (Unit == 1) i = 4;  // 1의 자릿수를 5~7row에 표현하기 위함
  else if (Unit == 0) set_color = 1;  //필요없는 부분이나 확실하게 하기 위함

  if (set_color == 0) {
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
  }
  if (set_color == 1) {
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


void Draw(int posx, int posy, int dotX[], int dotY) {
  boolean not_same = false;
  int xDot[4];
  int xChar = onePos(posx);
  byte screen[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  for (int i = 0; i < (led_color + 1); i++) {
    xDot[i] = onePos(dotX[i]);
    if (xChar == xDot[i]) {
      screen[xChar] = posy + dotY;
      not_same = true;
    }
    else {
      if (!not_same) screen[xChar] = posy;
      screen[xDot[i]] = dotY;
    }
  }
  not_same = false;
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

int dht11_h() {
  int h = dht11.readHumidity();
  return h;
}

int dht11_t() {
  int t = dht11.readTemperature();
  return t;
}

int Melody() {
  Serial.println("세워라 반석위에 선린의 터를");
  for (int i = 0; i < 12; i++)
  {
    tone(Buzzer, notes[i], times[i]);
    Serial.print("♪");
    delay(times[i]);
    //if (digitalRead(ballsensor) < 0.5) return 0;
  }
  Serial.println("");
  return 0;
}
