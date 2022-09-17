#include <Keypad.h>
#include <LiquidCrystal.h>
#include <String.h>

/************************************************************************/
const int RS = 13, EN = 12, D4 = 8, D5 = 9, D6 = 10, D7 = 11;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'C', '0', '=', '+'}
};
byte rowPins[ROWS] = {23, 25, 27, 29};
byte colPins[COLS] = {31, 33, 35, 37};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
/************************************************************************/

String contacts[2][20];
int empty[20];

void setup() {
  lcd.begin(20, 4);
  Serial2.begin(9600);
  Serial2.setTimeout(250);
  for (int i=0; i<20; i++)
    empty[i] = 1;
}

void loop() {
  printHelp();

  char operand = keypad.waitForKey();

  switch (operand) {  
    case '+' :
      addContact();
      break;
    case '/' :
      listContacts();
      break;
    case '-' :
      removeContact();
      break;
    case '*' :
      searchContact();
      break;
    case 'C' :
      filterContacts();
      break;
  }

  delay(1000);
}

void addContact() {
  Serial2.println("");
  String cname = getSerialInput("Contact's Name: ");
  String cphone = getKeypadInput("Contact's Phone: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  if (cname.length() > 28)
    lcd.print("Name is too long!");
  else if (cphone.length() < 8 || cphone.length() > 11)
    lcd.print("Invalid phone number");
  else {
    for (int i = 0; i < 20; i++)
      if (empty[i]) {
        contacts[0][i] = cname;
        contacts[1][i] = cphone;
        empty[i] = 0;
        break;
      }
    lcd.print("Added successfully!");
  }
  Serial2.println("");
}

void listContacts() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1 : Sort by Name");
  lcd.setCursor(0, 1);
  lcd.print("2 : Sort by Phone");
  char mode = keypad.waitForKey();
  Serial2.println("");
  Serial2.println("Contacts list ~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial2.println("");
  sortAndPrint(mode - '1');
  Serial2.println("");
  Serial2.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial2.println("");
}

void removeContact() {
  Serial2.println("");
  String cname = getSerialInput("Contact's Name: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  bool found = false;
  for (int i = 0; i < 20; i++)
    if (!empty[i] && contacts[0][i] == cname) {
      found = true;
      empty[i] = 1;
      break;
    }
  if (found)
    lcd.print("Removed successfully!");
  else
    lcd.print("Contact not found");
  Serial2.println("");
}

void searchContact() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1 : Search by Phone");
  lcd.setCursor(0, 1);
  lcd.print("2 : Search by Name");
  char mode = keypad.waitForKey();

  Serial2.println("");
  String info;
  if (mode == '1')
    info = getKeypadInput("Contact's Phone: ");
  else
    info = getSerialInput("Contact's Name: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  bool found = false;
  for (int i = 0; i < 20; i++)
    if (!empty[i] && ((mode == '1' && contacts[1][i] == info) || (mode == '2' && contacts[0][i] == info))) {
      found = true;
      Serial2.print("Name: ");
      Serial2.print(contacts[0][i]);
      Serial2.print(" - Phone: ");
      Serial2.print(contacts[1][i]);
      Serial2.println("");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Name: ");
      lcd.print(contacts[0][i]);
      lcd.setCursor(0, 1);
      lcd.print("Phone: ");
      lcd.print(contacts[1][i]);
      break;
    }
  Serial2.println("");
  if (!found)
    lcd.print("Contact not found");
  else
    keypad.waitForKey();
}

void filterContacts() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1 : Filter by Name");
  lcd.setCursor(0, 1);
  lcd.print("2 : Filter by Phone");
  char mode = keypad.waitForKey();
  lcd.clear();
  lcd.setCursor(0, 0);
  String filter;
  if (mode == '1')
    filter = getSerialInput("Name's Filter: ");
  else
    filter = getKeypadInput("Phone's Filter: ");
  
  Serial2.println("");
  Serial2.println("Filtered contacts list ~~~~~~~~~~~~~~~~");
  Serial2.println("");
  filterAndPrint(mode - '1', filter);
  Serial2.println("");
  Serial2.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial2.println("");
}

String getSerialInput(String mesg) {
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

void printHelp() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("+ : Add new contact");
  lcd.setCursor(0, 1);
  lcd.print("/ : Show contacts");
  lcd.setCursor(0, 2);
  lcd.print("* : Search contacts");
  lcd.setCursor(0, 3);
  lcd.print("- : Remove contact");
}

String getKeypadInput(String mesg) {
  String input = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mesg);
  lcd.setCursor(0, 1);
  char key = keypad.waitForKey();
  while (key != '=') {
    lcd.print(key);
    input += key;
    key = keypad.waitForKey();
  }
  lcd.clear();
  return input;
}

void swap(String *xp, String *yp) {
    String temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void sortAndPrint(int mode) {
  String s[2][20];
  int n = 0;
  for (int i=0; i<20; i++)
    if (!empty[i]) {
      s[0][n] = contacts[0][i];
      s[1][n++] = contacts[1][i];
    }
  
  bool swapped;
  for (int i=0; i<n-1; i++) {
    swapped = false;
    for (int j=0; j<n-i-1; j++) {
      if (s[mode][j] > s[mode][j+1]) {
        swap(&s[0][j], &s[0][j+1]);
        swap(&s[1][j], &s[1][j+1]);
        swapped = true;
      }
    }
    if (swapped == false)
      break;
  }

  for (int i=0; i<n; i++) {
      Serial2.print("Name: ");
      Serial2.print(s[0][i]);
      Serial2.print(" - Phone: ");
      Serial2.print(s[1][i]);
      Serial2.println("");
  }
}

void filterAndPrint(int mode, String filter) {
  String s[2][20];
  int n = 0;
  for (int i=0; i<20; i++)
    if (!empty[i] && (contacts[mode][i].indexOf(filter) != -1) || contacts[mode][i].indexOf(filter) != -1) {
      s[0][n] = contacts[0][i];
      s[1][n++] = contacts[1][i];
    }
  
  bool swapped;
  for (int i=0; i<n-1; i++) {
    swapped = false;
    for (int j=0; j<n-i-1; j++) {
      if (s[1][j] < s[1][j+1]) {
        swap(&s[0][j], &s[0][j+1]);
        swap(&s[1][j], &s[1][j+1]);
        swapped = true;
      }
    }
    if (swapped == false)
      break;
  }

  for (int i=0; i<n; i++) {
      Serial2.print("Name: ");
      Serial2.print(s[0][i]);
      Serial2.print(" - Phone: ");
      Serial2.print(s[1][i]);
      Serial2.println("");
  }
}
