#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

#define RS_PIN 13
#define EN_PIN 12
#define D4_PIN 8
#define D5_PIN 9
#define D6_PIN 10
#define D7_PIN 11
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);
//========================================================================================
const byte ROWS = 4; // four rows
const byte COLS = 3; // three columns
// define the symbols on the buttons of the keypad
char hexaKeys[ROWS][COLS] = {
  {'7', '8', '9'},
  {'4', '5', '6'},
  {'1', '2', '3'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {24, 23, 22, 25}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {31, 30, 29};     // connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
//========================================================================================
#define LM35_PIN  A0
//========================================================================================

Servo servo;
int timer[2] = {0, 0}; // timer's minute and second
int requiredTemp = 0;  // required temperature entered by user

void setup() {
  pinMode(40, INPUT);       // reset button
  pinMode(46, OUTPUT);      // led
  pinMode(47, OUTPUT);      // buzzer
  servo.attach(45);
  lcd.begin(16, 2);

  digitalWrite(46, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("** Solar Oven **");
  delay(2000);
  lcd.clear();
  digitalWrite(46, LOW);
}

//========================================================================================

void loop() {
  // fill the oven and wait for the user's command
  fillOven();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("[#] : Start");
  lcd.setCursor(0, 1);
  lcd.print("[*] : Cancel");
  char key;

  while(true){
    key = keypad.getKey();
    if (key) {
      if (key == '#') { // start
        // get cooking parameters
        requiredTemp = getInput("Temperature:");
        int inputTime = getInput("Time (sec):");
        timer[0] = inputTime/ 60, timer[1] = inputTime % 60;

        // print input parameters
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(requiredTemp);
        lcd.print(" C | ");
        lcd.print(formatClock(timer[0], timer[1]));
        lcd.setCursor(0, 1);

        // wait for the user to confirm
        lcd.print("Confirm by [#]");
        while (keypad.waitForKey() != '#');

        // first reach the desired temperature and then start the time (end of cooking process)
        if (!reachDesiredTemperature()) // reset button has been pressed
          break;
        if (!runTimer()) // reset button has been pressed
          break;
        alarm();
        break;
      }
      else if (key == '*') { // cancel
        lcd.clear();
        delay(1000);
        break;
      }
      else { // invalid command
        continue;
      }
    }
  }
}

//========================================================================================

void fillOven() {
  // wait for the user to press # key (open)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Open by [#]");
  while (keypad.waitForKey() != '#');
  
  // open the lid
  lcd.clear();
  lcd.print("Please wait...");
  servo.write(180);
  delay(1000);
  digitalWrite(46, HIGH);
  
  // wait for the user to press # key (close)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Close by [#]");
  while (keypad.waitForKey() != '#');
  
  // close the lid
  lcd.clear();
  lcd.print("Please wait...");
  servo.write(0);
  delay(1000);
  digitalWrite(46, LOW);
}

//========================================================================================

int getInput(String msg) {
  char key;
  String inputs;
  int inputi, digit = msg.length();
  lcd.clear();

  while (true) { // repeat untill you get a valid input
    lcd.setCursor(0, 0);
    lcd.print(msg);
    lcd.setCursor(0, 1);
    lcd.print("Confirm by [#]");

    key = keypad.getKey();
    if (key) { // delete char just entered or enter new char
      if (key == '#') {
        //checking food
        if (inputs.length() > 0) {
          inputi = inputs.toInt();
          break;
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Invalid Input!");
          delay(1000);
          lcd.clear();
          inputs = "";
          digit = msg.length();
        }
      }
      else if ((key == '*') && (digit > msg.length())) {
        digit--;
        lcd.setCursor(digit, 0);
        lcd.print(' ');
        inputs = inputs.substring(0, inputs.length() - 1);
      }
      else {
        inputs = inputs + key;
        lcd.setCursor(digit, 0);
        lcd.print(key);
        digit++;
      }
    }
  }
  
  return inputi;
}

//========================================================================================

bool reachDesiredTemperature() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");

  float sensorTemp;
  while((sensorTemp = (analogRead(LM35_PIN) *  487.0) / 1024) < requiredTemp) { // wait until you get to the desired temperature
    if (resetButton())
      return false;
    lcd.setCursor(0, 1);
    lcd.print(sensorTemp);
    lcd.print(" C");
    delay(100);
  }

  // print the current temperature
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Oven has reached");
  lcd.setCursor(0, 1);
  lcd.print(sensorTemp);
  lcd.print(" C !");

  // turn led off and on after 1 seconds
  digitalWrite(46, HIGH);
  delay(2000);
  digitalWrite(46, LOW);

  return true;
}

//========================================================================================

bool runTimer() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timer:");
  
  while (timer[0] >= 0) {
    if (timer[1] <= 0 && timer[0] > 0) // minute > 0 and second = 0 so you have to decreament the timer's minute
        timer[0]--, timer[1] = 59;
        
    while(timer[1] > 0) { // decreament the seconds and display the clock
      lcd.setCursor(0, 1);
      lcd.print(formatClock(timer[0], timer[1]));
      for (int i=0; i<10; i++) {
        if (resetButton())
          return false;
        delay(100);
      }
      timer[1]--;
    }
    
    // check if we've reached 00:00
    if (!timer[0] && !timer[1])
      return true;
  }
}

//========================================================================================

String formatClock(int minute, int second) {
  String m, s;
  if (second < 10) // pad with 0 if needed
    s = "0" + String(second, DEC);
  else
    s = String(second, DEC);
  if (minute < 10) // pad with 0 if needed
    m = "0" + String(minute, DEC);
  else
    m = String(minute, DEC); 
    
  // format clock like 00:00
  return m + ":" + s;
}

//========================================================================================

void alarm(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready!~");
  
  // turn the buzzer and led on and off after 1 seconds
  digitalWrite(47, HIGH);
  digitalWrite(46, HIGH);
  delay(2000);
  digitalWrite(47, LOW);
  digitalWrite(46, LOW);

  // open the lid
  lcd.clear();
  lcd.print("Please wait...");
  servo.write(180);
  delay(1000);
  digitalWrite(46, HIGH);

  // wait for the user to remove his meal and close the lid
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enjoy your meal!");
  lcd.setCursor(0, 1);
  lcd.print("Close by [#]");
  while (keypad.waitForKey() != '#');
  
  // close the lid
  lcd.clear();
  lcd.print("Please wait...");
  servo.write(0);
  delay(1000);
  digitalWrite(46, LOW);
}

//========================================================================================

bool resetButton() {
  if (digitalRead(40) == HIGH) { // check if the reset button is pressed
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reset Pressed!");

    // turn the buzzer and led on and off after 1 seconds
    digitalWrite(47, HIGH);
    digitalWrite(46, HIGH);
    delay(500);
    digitalWrite(47, LOW);
    digitalWrite(46, LOW);
    
    return true;
  }
  return false;
}
