#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <IRremote.h>

#define led A2
#define TRIG 6 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 5 //ECHO 핀 설정 (초음파 받는 핀)
#define RST_PIN 9
#define SS_PIN 10

int servo_Ultra = 2;
int servo_Trig = 4;
int servo_Barrel = 3;
int lazer = A7;
int pir = A0;
int X = A5;
int Z = A3;
int tilt = A1;
int button = A4;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, led, NEO_GRB + NEO_KHZ800);
MFRC522 mfrc(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
SoftwareSerial mySoftware(0, 1);
DFRobotDFPlayerMini player;

Servo Servo_Ultra;
Servo Servo_Trig;
Servo Servo_Barrel;
int angle = 90; // servo position in degrees
int ti = 0;
long duration, distance;
long cm = 0, pm = 0, pm_state = 0;
int TrigAngle[4] = {180, 120, 80, 20};
int active = 0;
boolean INIT = 0;
boolean previousSW = HIGH;
boolean currentSW = HIGH;
int addAngle = 10;
int prev_state = 0;
char mode = ' ';
int randomNum = 0;

void setup() {
  SPI.begin();
  mfrc.PCD_Init(); // Init MFRC522
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  pixels.begin();
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(pir, INPUT);
  pinMode(X, INPUT);
  pinMode(button, INPUT);
  pinMode(Z, INPUT);
  pinMode(tilt, INPUT);
  Servo_Ultra.attach(servo_Ultra);
  Servo_Trig.attach(servo_Trig);
  Servo_Barrel.attach(servo_Barrel);
  mySoftware.begin(9600);
  mySoftware.listen();
  player.begin(mySoftware);
  player.reset();
  player.volume(20);
  delay(1);
  player.play(1);
  randomSeed(analogRead(0));
}

void loop() {
  if (INIT == 0) {
    if ((!mfrc.PICC_IsNewCardPresent() || ! mfrc.PICC_ReadCardSerial())) {
      return;
    }
    else if (checkRFID() == 1) INIT = 1;
  }
  printing();
  cm = millis();
  currentSW = debounce(previousSW, button);
  if (previousSW == HIGH && currentSW == LOW) {
    active++;
    if (active == 3) active = 0;
  }
  previousSW = currentSW;

  player.play(5);
  delay(5000);
  //  randomNum = random(5, 11);
  //  if (cm - pm >= 4000) {
  //    mySoftware.listen();
  //    switch (randomNum) {
  //      case 5:
  //        player.play(5);
  //        break;
  //      case 6:
  //        player.play(6);
  //        break;
  //      case 7:
  //        player.play(7);
  //        break;
  //      case 8:
  //        player.play(8);
  //        break;
  //      case 9:
  //        player.play(9);
  //        break;
  //      case 10:
  //        player.play(10);
  //        break;
  //    }
  //    pm = millis();
  //  }

  
  if (active == 0) { //Automatic
    angle += addAngle;
    UltraSonic();
    if (distance < 10)
      shoot();
    Servo_Ultra.write(angle);
    printing();
    if (angle >= 180 || angle <= 0) addAngle *= -1;

  }
  else if (active == 1) { //Manual
    int degree = map(analogRead(X), 0, 1023, 0, 180);
    Servo_Barrel.write(degree);
    if (digitalRead(Z) == 0) shoot();

  }
  else if (active == 2) { //Sleep Mode

  }


  //  mySoftware.listen();

  mfrc.PICC_HaltA();
  mfrc.PCD_StopCrypto1();
}

void UltraSonic() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn (ECHO, HIGH);
  distance = duration * 17 / 1000;
}

int shoot() {
  //  mySoftware.listen();
  delay(100);
  if (active == 1) {
    Servo_Trig.write(TrigAngle[ti++]);
    if (ti == 4) reload();
    //    player.play(34);
    //    delay(1000);
    return 1;
  }
  else {
    while (1) {
      Servo_Barrel.write(angle);
      if (digitalRead(pir) == 1) {
        Servo_Trig.write(TrigAngle[ti++]);
        if (ti == 4) reload();
        return 1;
      }
      else {
        pm = millis();
        if (pm - cm >= 5000) return 0;
      }
    }
  }
}

int checkRFID() {
  if (mfrc.uid.uidByte[0] == 170 && mfrc.uid.uidByte[1] == 235
      && mfrc.uid.uidByte[2] == 146 && mfrc.uid.uidByte[3] == 159) {
    return 1;
  }
  else 0;
}

void reload() {
  delay(100);
  Servo_Trig.write(TrigAngle[0]);
  ti = 1;
}

boolean debounce (boolean previous, int butt) {
  boolean current = digitalRead(butt);
  if (previous != current) {
    delay(5);
    current = digitalRead(butt);
  }
  return current;
}

void led_state(int state, bool chng_state) {
  if (cm - pm_state >= 2000) {
    if (prev_state == 3) {
      state = 2;
    }
    else if (cm - pm_state >= 4000) state = 0;
  }
  if (chng_state == 1) prev_state = state;
  switch (state) {
    case 0: //state safety
      pixels.setPixelColor(0, 0, 0, 125);
      pixels.show();
      pm_state = millis();
      break;
    case 1: //state  Guarding
      pixels.setPixelColor(0, 0, 0, 0);
      pixels.show();
      pm_state = millis();
      break;
    case 2: //state danger
      pixels.setPixelColor(0, 125, 0, 0);
      pixels.show();
      pm_state = millis();
      break;
    default:
      break;
  }
}

void printing() {
  Serial.print("PIR: "); Serial.println(digitalRead(pir));
  Serial.print("Ultra: "); Serial.println(distance);
  Serial.print("button: "); Serial.println(digitalRead(button));
  Serial.println();
}
