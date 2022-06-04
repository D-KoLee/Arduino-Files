#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
//#include <DFRobotDFPlayerMini.h>
#include <IRremote.h>

#define led A2
#define BT_RXD 8
#define BT_TXD 7
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
int remoteSensor = A1;
int button = A4;
const int SPEAKER = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, led, NEO_GRB + NEO_KHZ800);
SoftwareSerial bluetooth(BT_RXD, BT_TXD);
MFRC522 mfrc(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
IRrecv irrecv(remoteSensor);
decode_results results;
//SoftwareSerial mySoftware(0, 1);
//DFRobotDFPlayerMini player;

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
int manual = 0;
int manual_degree = 0;
int BTH = 0;
char mode = ' ';
//int randomNum = 0;

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
  pinMode(remoteSensor, INPUT);
  Servo_Ultra.attach(servo_Ultra);
  Servo_Trig.attach(servo_Trig);
  Servo_Barrel.attach(servo_Barrel);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  bluetooth.begin(9600);
  //  mySoftware.begin(9600);
  //  mySoftware.listen();
  //  player.begin(mySoftware);
  //  player.reset();
  //  player.volume(20);
  //  delay(1);
  //  player.play(1);
  //  randomSeed(analogRead(0));
}

void loop() {
  if (INIT == 0) {
    if ((!mfrc.PICC_IsNewCardPresent() || ! mfrc.PICC_ReadCardSerial())) {
      return;
    }
    else if (checkRFID() == 1) INIT = 1;
  }
  //printing();
  cm = millis();
  //  RemoteInput();
  currentSW = debounce(previousSW, button);
  if (previousSW == HIGH && currentSW == LOW) {
    active++;
    if (active == 3) active = 0;
  }
  previousSW = currentSW;

  led_state(-1, 0);

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
  //
  //  bluetooth.listen();
  //  if (bluetooth.available()) {
  //    Serial.write(bluetooth.read()); //0:left, 1: right, 2: fire, 3: Auto
  //  }
  //  if (Serial.available()) {         //4: Sleep
  //    bluetooth.write(Serial.read());
  //  }

  if (active == 0) { //Automatic
    angle += addAngle;
    UltraSonic();
    if (distance < 10) {
      led_state(1, 1);
      shoot();
    }
    Servo_Ultra.write(angle);
    if (angle >= 180 || angle <= 0) addAngle *= -1;

  }
  else if (active == 1) { //Manual
    if (analogRead(X) >= 510 || analogRead(X) <= 500) {
      manual = 0;
    }
    else if (irrecv.decode(&results)) {
      manual = 1;
    }
    else if (bluetooth.available()) {
      manual = 2;
      //      BTH = bluetooth.read() - 48;
      //    Serial.println(BTH);
    }
    if (manual == 0) {
      int degree = map(analogRead(X), 0, 1023, 0, 180);
      Servo_Barrel.write(degree);
      if (digitalRead(Z) == 0) shoot();
    }
    else if (manual == 1) {
      int IRdegree = IRRemoteInput();
      if (IRdegree >= 0) Servo_Barrel.write(IRdegree);
    }
    else if (manual == 2) {
      int BTHdegree = remote(bluetooth.read() - 48);
      if (BTHdegree >= 0) Servo_Barrel.write(BTHdegree);
      Serial.println(BTH);
      Serial.println(BTHdegree);
      Serial.println("");
    }
  }
  else if (active == 2) { //Sleep Mode
    Servo_Ultra.write(90);
    Servo_Barrel.write(90);
    Servo_Trig.write(180);
    pixels.setPixelColor(0, 0, 0, 0);
    pixels.show();
    ti = active = INIT = 0;
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
    bluetooth.write("발사되었습니다.\n");
    led_state(2, 1);
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
        bluetooth.write("발사되었습니다.\n");
        led_state(2, 1);
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
  bluetooth.write("재장전이 필요합니다.\n");
  while (1) {
    if (digitalRead(Z) == 0) return;
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

void led_state(int state, bool chng_state) {
  if (cm - pm_state >= 5000) {
    if (prev_state == 2) {
      prev_state = state = 1;
    }
    else if (cm - pm_state >= 10000) state = 0;
  }
  if (chng_state == 1) prev_state = state;
  switch (state) {
    case 0: //state safety
      pixels.setPixelColor(0, 0, 125, 0);
      pixels.show();
      pm_state = millis();
      break;
    case 1: //state  Guarding
      pixels.setPixelColor(0, 125, 125, 0);
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

int IRRemoteInput() {
  if (irrecv.decode(&results)) {
    if (results.decode_type == NEC) {
      delay(100);
      switch (results.value) {
        case 0x00FFE01F: //<-
          irrecv.resume();
          return remote(0);
          break;
        case 0x00FF906F: //->
          irrecv.resume();
          return remote(1);
          break;
        case 0x00FFA857: //shoot
          irrecv.resume();
          shoot();
          return -1;
          break;
        case 0x00FFC23D: //mode change
          irrecv.resume();
          active--;
          return -1;
          break;
        case 0x00FFA25D: //power off
          irrecv.resume();
          active++;
          return -1;
        default:
          break;
      }
    }
  }
}

int remote(int num) {
  switch (num) {
    case 0: //<-
      if ((manual_degree -= 10) <= 0) manual_degree = 0;
      return manual_degree;
      break;
    case 1: //->
      if ((manual_degree += 10) >= 180) manual_degree = 180;
      return manual_degree;
      break;
    case 2: //shoot
      shoot();
      return -1;
      break;
    case 3: //mode change
      active--;
      return -1;
      break;
    case 4:
      active++;
      return -1;
    default:
      break;
  }
}

void printing() {
  Serial.print("PIR: "); Serial.println(digitalRead(pir));
  Serial.print("Ultra: "); Serial.println(distance);
  Serial.print("button: "); Serial.println(digitalRead(button));
  Serial.print("X: "); Serial.println(analogRead(X));
  Serial.println();
}
