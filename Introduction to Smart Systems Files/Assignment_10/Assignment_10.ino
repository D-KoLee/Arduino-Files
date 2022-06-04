#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

#define RS 12
#define RW 11
#define E 10
#define DB4 5
#define DB5 4
#define DB6 3
#define DB7 2
#define LED A5

LiquidCrystal lcd(RS, RW, E, DB4, DB5, DB6, DB7);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);

int reed = 13;
int hall = 8;
int fire = 6;
int soil = A3;
int IR = 7;
int coll = 9;
int potentiometer = A4;
int state = -1;

boolean fire_situation = 0;
boolean previousSW = HIGH;
boolean currentSW = HIGH;

void setup() {
  pinMode(IR, INPUT);
  pinMode(reed, INPUT);
  pinMode(hall, INPUT);
  pinMode(soil, INPUT);
  pinMode(coll, INPUT);
  pinMode(fire, INPUT);
  pinMode(potentiometer, INPUT);
  lcd.begin(16, 2);
  lcd.clear();
  pixels.begin();
  pixels.show();
  lcd.setCursor(0, 0);
  lcd.print("Welcom to Mars");
  lcd.setCursor(0, 1);
  lcd.print("Astronaut Mark");
  Serial.begin(9600);
}

void loop() {
  int brightness = map(analogRead(potentiometer), 0, 1023, 0, 255);
  int moisture = analogRead(soil);

  //fire detection
  if (!digitalRead(fire)) fire_situation = 1;

  if (fire_situation == 0) {
    currentSW = digitalRead(coll);
    if (previousSW == 1 && currentSW == 0) {
      lcd.clear();
      state++;
      if (state == 3) state = 0;
    }
    previousSW = currentSW;

    if (state == 0) {//Flash Mode
      lcd.clear();
      int light = map(analogRead(potentiometer), 0, 1023, 0, 100);
      lcd.setCursor(0, 0);
      lcd.print("Light Mode");
      lcd.setCursor(0, 1);
      lcd.print(light);
      lcd.print(" %");
      pixels.setPixelColor(0, brightness, brightness, brightness);
      pixels.show();
      delay(20);
    }
    else if (state == 1) { //Find Magnet. 
      lcd.clear();
      if (digitalRead(reed) == 0) {//And tag the magnet on the reed sensor. 
        pixels.setPixelColor(0, brightness, brightness, 0);
        pixels.show();
        lcd.setCursor(0, 0);
        lcd.print("Moisture : "); //Then I can check soil moisture
        int mos = 100 - map(moisture, 0, 1023, 0, 100); if (mos < 5) mos = 0;
        lcd.print(mos);
        if (moisture <= 300) {
          lcd.setCursor(0, 1);
          lcd.print("state : FLOODING");
        }
        else if (moisture > 300 && moisture <= 700) {
          lcd.setCursor(0, 1);
          lcd.print("state : HUMID");
        }
        else if (moisture >= 700) {
          lcd.setCursor(0, 1);
          lcd.print("state : DRY");
        }
        delay(20);
      }
      else {
        pixels.setPixelColor(0, 0, brightness, brightness);
        pixels.show();
        lcd.setCursor(0, 0);
        lcd.print("Find Magnet");
        lcd.setCursor(0, 1);
        if (digitalRead(hall) == 0) lcd.print("Detected");
        else lcd.print("Nothing here");
        delay(20);
      }
    }
    else if (state == 2) { //Obstacle detection
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Now");
      lcd.setCursor(0, 1);
      if (digitalRead(IR) == 0) {
        lcd.print("STOP");
        pixels.setPixelColor(0, 255, 0, 0);
        pixels.show();
      }
      else {
        lcd.print("Go forward");
        pixels.setPixelColor(0, 0, 255, 0);
        pixels.show();
      }
    }
  }
  else if (fire_situation == 1) { //fire alarm
    lcd.clear();
    if (digitalRead(fire) == 1) { //when extinguished
      if(state == -1) state++;
      lcd.setCursor(0, 0);
      lcd.print("Fire has been");
      lcd.setCursor(0, 1);
      lcd.print("EXTINGUISHED");
      delay(1000);
      fire_situation = 0;
    }

    pixels.setPixelColor(0, 255, 0, 0);
    pixels.show();
    lcd.setCursor(0, 0);
    lcd.print("--DANGER--");
    lcd.setCursor(0, 1);
    lcd.print("EVACUATE NOW");

  }
}
