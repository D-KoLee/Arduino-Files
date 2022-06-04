#include <stdio.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j< 5; j++) {
      Serial.print("j = ");
      Serial.println(j);
      if(j == 3){
        Serial.println("breaked");
        break;
      }
      delay(50);
    }
    Serial.print("i = ");
    Serial.println(i);
    delay(50);
  }
  
}
