#include <SPI.h>

const int SS0_PIN = 44, SS1_PIN = 45;

void setup() {
  Serial.begin(9600);
  Serial.println("MASTER~~~~~~~~~~~~~~~~~~");

  pinMode(SS0_PIN, OUTPUT);
  digitalWrite(SS0_PIN, HIGH);
  pinMode(SS1_PIN, OUTPUT);
  digitalWrite(SS1_PIN, HIGH);
  
  SPI.begin();
}

void sendMessage(const char *msg) {
  for (const char *p = msg ; char c = *p; p++) {
    SPI.transfer(c);
    Serial.print(c);
    delay(5);
  }
  Serial.println();
}

void loop() {
  // send the first message to the first slave
  digitalWrite(SS0_PIN, LOW);
  sendMessage("Maryam Moosavi - Negar Movaghatian\r");
  digitalWrite(SS0_PIN, HIGH);
  delay(500);

  // send the second message to the second slave
  digitalWrite(SS1_PIN, LOW);
  sendMessage("Hello, Maryam & Negar!\r");
  digitalWrite(SS1_PIN, HIGH);
  delay(500);
}
