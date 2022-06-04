#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define Buzzer 6

float vout = 0.0;
float vin = 0.0;
float volt;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;
int snd = 0;

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini engine;
void printDetail(uint8_t type, int value);

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!engine.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  delay(1);
  pinMode(A0, INPUT);  
  engine.volume(30);  //Set volume value. From 0 to 30
  engine.play(1);  //Play the first mp3
}

void loop()
{
  static unsigned long timer = millis();

   value = analogRead(A0);        //A0핀으로 받아는 값 => 변수 value
  vout = (value * 5.0) / 1024.0; 
  vin = vout / (R2 / (R1 + R2)); //전압을 구하는 공식

  Serial.print("V: "); // 시리얼모니터 확인용
  Serial.println(vin); // 22
  volt = 5 - vin;
  boolean play = digitalRead(3);
  if (play == HIGH){
  if (volt >= 0 && volt < 0.2){
    engine.play(1);
    delay(200);
  }
    if (volt >= 0.2 && volt <0.4){
    engine.play(2);
    delay(200);
  }
    if (volt >= 0.4 && volt <0.6){
    engine.play(3);
    delay(200);
  }
    if (volt >= 0.6 && volt <0.8){
    engine.play(4);
    delay(200);
  }
    if (volt >= 0.8 && volt <1.0){
    engine.play(5);
    delay(200);
  }
    if (volt >= 1.0 && volt <1.2){
    engine.play(6);
    delay(200);
  }
    if (volt >= 1.2 && volt <1.4){
    engine.play(7);
    delay(200);
  }
    if (volt >= 1.4 && volt <1.6){
    engine.play(8);
    delay(200);
  }
    if (volt >= 1.6 && volt <1.8){
    engine.play(9);
    delay(200);
  }
    if (volt >= 1.8 && volt <2.0){
    engine.play(10);
    delay(200);
  }
    if (volt >= 2.0 && volt <2.2){
    engine.play(11);
    delay(200);
  }
    if (volt >= 2.2 && volt <2.4){
    engine.play(12);
    delay(200);
  }
    if (volt >= 2.4 && volt <2.6){
    engine.play(13);
    delay(200);
  }
    if (volt >= 2.6 && volt <2.8){
    engine.play(14);
    delay(200);
  }
    if (volt >= 2.8 && volt <3.0){
    engine.play(15);
    delay(200);
  }
    if (volt >= 3.0 && volt <3.2){
    engine.play(16);
    delay(200);
  }
    if (volt >= 3.2 && volt <3.4){
    engine.play(17);
    delay(200);
  }
    if (volt >= 3.4 && volt <3.6){
    engine.play(18);
    delay(200);
  }
    if (volt >= 3.6 && volt <3.8){
    engine.play(19);
    delay(200);
  }
    if (volt >= 3.8 && volt <4.0){
    engine.play(20);
    delay(200);
  }
    if (volt >= 4.0 && volt <4.2){
    engine.play(21);
    delay(200);
  }
    if (volt >= 4.2 && volt <4.4){
    engine.play(22);
    delay(200);
  }
    if (volt >= 4.4 && volt <4.6){
    engine.play(23);
    delay(200);
  }
    if (volt >= 4.6 && volt <4.8){
    engine.play(24);
    delay(200);
  }
    if (volt >= 4.8 && volt <5.0){
    engine.play(25);
    delay(200);
  }
    if (volt >= 5.0){
    engine.play(25);
    delay(200);
  }
  }

  if (engine.available()) {
    printDetail(engine.readType(), engine.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  
  
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
