#include <Keypad.h>
#include <LiquidCrystal.h>
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

const int RS = 13, EN = 12, D4 = 8, D5 = 9, D6 = 10, D7 = 11;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const String USERNAME = "62";
const String PASSWORD = "123";

void setup() {
  lcd.begin(16, 2);
}

void loop() {
  String username = getInput("Username:");
  String password = getInput("Password:");
  lcd.clear();
  lcd.setCursor(0, 0);
  if (username != USERNAME)
    lcd.print("Invalid Username");
  else if (password != PASSWORD)
    lcd.print("Wrong Password");
  else
    lcd.print("Successful!");
  delay(1000);
}

String getInput(String mesg) {
  String input = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mesg);
  lcd.setCursor(0, 1);
  char key = keypad.waitForKey();
  while(key != 'C') {
    lcd.print(key);
    input += key;
    key = keypad.waitForKey();
  }
  lcd.clear();
  return input;
}
