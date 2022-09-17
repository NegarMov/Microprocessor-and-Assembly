#include<string.h>

void setup() {

  for (int i=0; i<6; i++)
    pinMode(i, OUTPUT);

  for (int i=19; i<22; i++)
    pinMode(i, INPUT);
  
}

void loop() {

  if (digitalRead(19) == 0) {
    for (int i=5; i>=0; i--) {
      digitalWrite(i, HIGH);
      delay(250);
    } 
  }

  if (digitalRead(20) == 0) {
    int count = strlen("Negar");
    for (int i=0; i<count; i++) {
      for (int j=0; j<6; j++)
        digitalWrite(j, LOW);
      delay(250);
      for (int j=0; j<6; j++)
        digitalWrite(j, HIGH);
      delay(250);
    }
  }

  if (digitalRead(21) == 0) {
    for (int i=0; i<6; i++)
      digitalWrite(i, LOW);
  }

}
