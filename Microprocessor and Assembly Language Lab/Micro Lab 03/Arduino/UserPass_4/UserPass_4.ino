#include <LiquidCrystal.h>
#include <String.h>

const int RS = 13, EN = 12, D4 = 8, D5 = 9, D6 = 10, D7 = 11;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const String USERNAME = "62";
const String PASSWORD = "123";

void setup() {
  lcd.begin(16, 2);
  Serial2.begin(9600);
  Serial2.setTimeout(250);
}

void loop() {
  String username = getInput("Username: ");
  String password = getInput("Password: ");
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

  Serial2.print(mesg);

  char c;
  while (true) {
    while (Serial2.available() == 0);
    char c = Serial2.read();
    Serial2.print(c);
    if ((int) c == 13)
      break;
    lcd.print(c);
    input += c;
  }
  lcd.clear();
  
  return input;
}
