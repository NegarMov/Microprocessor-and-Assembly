#include <LiquidCrystal.h>
#include <String.h>

const int RS = 13, EN = 12, D4 = 8, D5 = 9, D6 = 10, D7 = 11;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

int row = 0;
const String DATE = "2022/03/02";

void setup() {
  lcd.begin(16, 2);
}

void loop() {
  for (int i=0; i<=16-DATE.length(); i++) {
    lcd.clear();
    lcd.setCursor(i, row);
    lcd.print(DATE);
    delay(500);
  }
  row = 1 - row;
}
