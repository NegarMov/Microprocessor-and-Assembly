#include <Keypad.h>
#include <LiquidCrystal.h>
#include <String.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'C','0','=','+'}
};
byte rowPins[ROWS] = {23, 25, 27, 29};
byte colPins[COLS] = {31, 33, 35, 37};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); 

const int RS = 13, EN = 12, D4 = 8, D5 = 9, D6 = 10, D7 = 11;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

char key;

void setup() {
  lcd.begin(16, 2);
  Serial2.begin(9600);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  int num1 = 0;
  key = keypad.waitForKey();
  while(key <= '9' && key >= '0') {
    lcd.print(key);
    Serial2.print(key);
    num1 = num1 * 10 + (key - '0');
    key = keypad.waitForKey();
  }
  
  lcd.print(key);
  Serial2.print(key);
  char op = key;

  int num2 = 0;
  key = keypad.waitForKey();
  while(key <= '9' && key >= '0') {
    lcd.print(key);
    Serial2.print(key);
    num2 = num2 * 10 + (key - '0');
    key = keypad.waitForKey();
  }

  int res;
  bool valid = true;
  switch(op) {
    case '/':
      res = num1 / num2;
      break;
    case '*':
      res = num1 * num2;
      break;
    case '-':
      res = num1 - num2;
      break;
    case '+':
      res = num1 + num2;
      break;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Invalid Operand");
      Serial2.println();
      Serial2.print("Invalid Operand");
      valid = false;
  }

  if (valid) {
    lcd.setCursor(0, 1);
    lcd.print(res);
    Serial2.println();
    Serial2.print(res);
  }
  
  do {
    key = keypad.waitForKey();
  } while (key != 'C');

  Serial2.println();
  Serial2.println();
}
