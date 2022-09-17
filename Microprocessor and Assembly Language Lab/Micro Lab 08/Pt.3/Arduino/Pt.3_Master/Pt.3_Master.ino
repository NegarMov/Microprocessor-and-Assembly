#include <SPI.h>

const int SS0_PIN = 44, SS1_PIN = 45, TEMP_PIN = A8, LIGHT_PIN = A9;
uint8_t lightValue, tempValue;

void setup() {
  Serial.begin(9600);
  Serial.println("MASTER~~~~~~~~~~~~~~~~~~");

  pinMode(TEMP_PIN, INPUT);
  pinMode(LIGHT_PIN, INPUT);

  pinMode(SS0_PIN, OUTPUT);
  digitalWrite(SS0_PIN, HIGH);
  pinMode(SS1_PIN, OUTPUT);
  digitalWrite(SS1_PIN, HIGH);
  
  SPI.begin();
}

void loop() {
  // read the light value, print it on the terminal and send it to the first slave
  int sensorValue = analogRead(LIGHT_PIN);
  lightValue = map(sensorValue, 0, 1023, 0, 100);
  digitalWrite(SS0_PIN, LOW);
  SPI.transfer(lightValue);
  Serial.print("Light: ");
  Serial.print(lightValue);
  Serial.println(" %");
  delay(5);
  digitalWrite(SS0_PIN, HIGH);
  delay(500);

  // read the temperature value, print it on the terminal and send it to the second slave
  sensorValue = analogRead(TEMP_PIN);
  tempValue = map(sensorValue, 0, 306, 0, 150);
  digitalWrite(SS1_PIN, LOW);
  SPI.transfer (tempValue);
  Serial.print("Temperature: ");
  Serial.print(tempValue);
  Serial.println(" C");
  delay(5);
  digitalWrite(SS1_PIN, HIGH);
  delay(500);

  Serial.println();
}
