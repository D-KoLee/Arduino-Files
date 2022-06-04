#include <LiquidCrystal.h>

#define RS 12
#define RW 11
#define E 10
#define DB4 5
#define DB5 4
#define DB6 3
#define DB7 2

LiquidCrystal lcd(RS, RW, E, DB4, DB5, DB6, DB7);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hello, World");
  lcd.setCursor(0,1);
  lcd.print("Smart System");
}

void loop() {
  // put your main code here, to run repeatedly:

}
