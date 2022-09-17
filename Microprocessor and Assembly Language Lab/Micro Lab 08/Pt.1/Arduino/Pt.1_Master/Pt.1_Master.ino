#include <SPI.h>

const int SS_PIN = 45;

void setup() {
  Serial.begin(9600);
  Serial.println("MASTER~~~~~~~~~~~~~~~~~~");

  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
  
  SPI.begin();
}

void loop() {
  // by setting SS to low, send a signal that shows you want to send data to slave
  digitalWrite(SS_PIN, LOW);
  delay(10);

  // transfer the message char by char and print it on the terminal
  for (const char *msg_p = "Maryam Moosavi - Negar Movaghatian\r" ; char c = *msg_p; msg_p++) {
    SPI.transfer(c);
    Serial.print(c);
    delay(5);
  }
  Serial.println();

  // end the data transfer by setting SS to high
  digitalWrite(SS_PIN, HIGH);

  delay(1000);
}
