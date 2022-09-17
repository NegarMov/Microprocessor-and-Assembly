#include <Keypad.h>
#include <String.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'7','8','9','÷'},
  {'4','5','6','×'},
  {'1','2','3','-'},
  {'C','0','=','+'}
};
byte rowPins[ROWS] = {23, 25, 27, 29};
byte colPins[COLS] = {31, 33, 35, 37};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

int ledPins[9] = {A0, A1, A2, A3, A6, A4, A9, A10, A11};

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(250);

  for (int i=0; i<9; i++)
    pinMode(ledPins[i], OUTPUT);
}

void setLED(int number) {
  if (number > 9 || number < 0)
      Serial.println("  -> Invalid number!");
  else
    for (int i=0; i<9; i++)
      if (i < number)
        digitalWrite(ledPins[i], HIGH);
      else
        digitalWrite(ledPins[i], LOW);  
}

void loop() {
  if (Serial.available() > 0) {
    int number = Serial.parseInt();
    Serial.print("Terminal input: ");
    Serial.println(number);
    setLED(number);
  } 
  else {
    char key = keypad.getKey();
    if (key) {
      Serial.print("Keypad input: ");
      Serial.println(key);
      setLED(key - '0');
    }
  }
}
