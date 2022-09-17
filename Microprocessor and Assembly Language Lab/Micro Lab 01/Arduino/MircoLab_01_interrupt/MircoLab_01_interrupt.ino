#include<string.h>

bool flag[3];

void setup() {

  for (int i=0; i<7; i++)
    pinMode(i, OUTPUT);

  for (int i=19; i<22; i++)
    pinMode(i, INPUT);

  attachInterrupt(digitalPinToInterrupt(19), firstButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(20), secondButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(21), thirdButton, FALLING);
  
}

void loop() {
  if(flag[0]) {
    flag[0] = false;
    for (int i=5; i>=0; i--) {
      digitalWrite(i, HIGH);
      delay(250);
    } 
  } 
  else if(flag[1]) {
    flag[1] = false;
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
  else if(flag[2]) {
    flag[2] = false;
    for (int i=0; i<6; i++)
      digitalWrite(i, LOW);
  }
}

void firstButton() {
  flag[0] = true;
}

void secondButton() {
  flag[1] = true;
}

void thirdButton() {
  flag[2] = true;
}
