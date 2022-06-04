#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>


#define RS 13
#define RW 12
#define E 11
#define DB4 4
#define DB5 3
#define DB6 2
#define DB7 1
#define LED A5

LiquidCrystal lcd(RS, RW, E, DB4, DB5, DB6, DB7);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, LED, NEO_GRB + NEO_KHZ800);


int reed = 13;
int hall = 9;
int flame = 7;
int soil= A0;
int IRobstacle = 5;
int collision = 3;

void setup() {
  pinMode(reed, INPUT);
  pinMode(hall, INPUT);
  pinMode(flame, INPUT);
  pinMode(soil, INPUT);
  pinMode(IRobstacle, INPUT);
  pinMode(collision, INPUT);
  lcd.begin(16,2);
  lcd.clear();
  Serial.begin(9600);
}

void loop(){
  int reed_switchValue = digitalRead(reed);
  int hall_sensorValue = digitalRead(hall);
  int flame_sensorValue = digitalRead(flame);
  int soil_sensorValue = digitalRead(soil);
  int IRobstacle_sensorValue = digitalRead(IRobstacle);
  int collision_sensorValue = digitalRead(collision);
  
    lcd.setCursor(0,1);
    lcd.print("Magnetic Field");
  if(reed_switchValue == HIGH){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Magnetic Field");
  }

  else if(hall_sensorValue == LOW) {
    lcd.setCursor(0,1);
    lcd.print("Magnet stick");
  }

  else if(flame_sensorValue == LOW) {
    lcd.setCursor(0,1);
    lcd.print("Evacuate");
  }

  else if(soil_sensorValue == LOW){
    lcd.setCursor(0,0);
    lcd.print("Soil Sensor");
    lcd.setCursor(0,1);
    lcd.print("Water Flood");
  }
  
  else if(IRobstacle_sensorValue == LOW){
    lcd.setCursor(0,1);
    lcd.print("Obstacle Detection");
  }
  

  else if(collision_sensorValue == LOW){
    lcd.setCursor(0,1);
    lcd.print("Collision detection");
  }
}
