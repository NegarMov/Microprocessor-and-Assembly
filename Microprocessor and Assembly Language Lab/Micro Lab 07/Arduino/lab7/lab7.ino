#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h> 
#define ADDRESS 0b1010000

const byte ROWS = 4;
const byte COLS = 4;
char myKeys[ROWS][COLS] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'c','0','=','+'}
};
byte rowPins[ROWS] = {22, 23, 24,25};
byte colPins[COLS] = {26, 27, 28, 29};
Keypad customKeypad = Keypad( makeKeymap(myKeys), rowPins, colPins, ROWS, COLS); 

int done = 1;
int arr_timer[4] ={0};

LiquidCrystal lcd(5, 4, 3, 2, 1, 0);

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  Wire.begin();
 if (readFromMemory(6) != 0) {
    writeToMemory(1, 5);
    delay(100);
    writeToMemory(2, 5);
    delay(100);
    writeToMemory(3, 5);
    delay(100);
    writeToMemory(4, 5);
    delay(100);
    writeToMemory(6, 1);
    delay(100);
    
  }
   lcd.print(readFromMemory(6));
   lcd.print(' ');
   lcd.print(readFromMemory(1));
   lcd.print(' ');
   lcd.print(readFromMemory(2));
   lcd.print(' ');
   lcd.print(readFromMemory(3));
   lcd.print(' ');
   lcd.print(readFromMemory(4));
   lcd.print(' ');
  delay(500);
}

void loop() {
 checkIfNew(); 
 
}

void writeToMemory(uint16_t memAddr, int data){
  Wire.beginTransmission(ADDRESS);
  Wire.write((memAddr & 0xFF00) >> 8);
  Wire.write((memAddr & 0x00FF) >> 0);
  Wire.write(data);
  Wire.endTransmission();
}

int readFromMemory(uint16_t memAddr){
  Wire.beginTransmission(ADDRESS);
  Wire.write((memAddr & 0xFF00) >> 8);
  Wire.write((memAddr & 0x00FF) >> 0);
  Wire.endTransmission();
  Wire.requestFrom(ADDRESS, 4);
  return Wire.read();
}

void showRemainTime(int mode, int remainTime){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mode:");
  lcd.print(mode);
  lcd.setCursor(0,1);
  lcd.print(remainTime);
  lcd.print("s");
}

int getInput() {
  int input = 0;
  while(true) {
    char customKey = customKeypad.getKey();
    if (customKey == '1' 
      || customKey == '2'
      || customKey == '3'
      || customKey == '4'
      || customKey == '5'
      || customKey == '6'
      || customKey == '7'
      || customKey == '8'
      || customKey == '9'
      || customKey == '0'){
      lcd.print(customKey);
      input=input*10+(customKey-48);
    } else if(customKey == '=') {
      return input;
    }
  }
}

void checkIfNew(){
  done = readFromMemory(6);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("welcome");
    delay(500);
  if(done == 1){
    settings();
  }
  doTheJob();
}
void settings(){
  char key;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("default?y:=/n=:*");
  lcd.setCursor(0,1);
  for(int i=1; i<=4; i++){
    lcd.print(i);
    lcd.print(':');
    lcd.print(readFromMemory(i));
    lcd.print(' ');
  }
  key = customKeypad.waitForKey();
  if(key =='*'){
  for(int i=1; i<=4; i++){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("you want the");
    lcd.setCursor(13,0);
    lcd.print(i);
    lcd.setCursor(0,1);
    lcd.print("yes[=] no[*]");
    key = customKeypad.waitForKey();
    if(key == '='){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("set the time:");
      arr_timer[i-1] = getInput();
      writeToMemory(i, arr_timer[i-1]);
      delay(100);
    }else{
      writeToMemory(i, 0);
      delay(100);
    }
  }
  }else{
    for(int i=1; i<=4; i++){
      arr_timer[i-1] = 5;
    }
    writeToMemory(1, 5);
    delay(100);
    writeToMemory(2, 5);
    delay(100);
    writeToMemory(3, 5);
    delay(100);
    writeToMemory(4, 5);
    delay(100);
  }
  done = 0;
  writeToMemory(6, done);
  delay(100);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ok! Ready to go!");
  delay(500);
  }
  
void doTheJob(){
  for(int mode = 1; mode<=4; mode++){
    int modeTime = readFromMemory(mode);
    if(modeTime != 0){
      digitalWrite(mode + 7, HIGH);
      timer(mode, modeTime);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(mode);
      lcd.print("is done!");
      digitalWrite(mode +7, LOW);
      delay(100);
    }
    
  }
  done = 1;
  writeToMemory(6, done);
  delay(100);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("job is done!");
  for(int i=1; i<=4; i++){
    writeToMemory(i, 5);
    delay(100);
  }
  delay(500);
  
}

void timer(int mode, int modeTime){
  while(modeTime>=0){
    checkForButton();
    showRemainTime(mode, readFromMemory(mode));
    delay(100);
    modeTime--;
    writeToMemory(mode, modeTime);
    delay(300);
  }
  writeToMemory(mode, 0);
  delay(200);
}

void checkForButton(){
  char key = customKeypad.getKey();
  if(key == '/'){
    lcd.clear();
    lcd.print("hold...");
    customKeypad.waitForKey();
  }
  //key = null;
  
}
