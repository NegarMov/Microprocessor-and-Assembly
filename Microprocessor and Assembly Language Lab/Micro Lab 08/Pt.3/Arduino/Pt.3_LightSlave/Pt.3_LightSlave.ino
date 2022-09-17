#include <SPI.h>

const int MISO_PIN = 50, MOSI_PIN = 51, SCK_PIN = 52, SS_PIN = 53;
volatile boolean finished = false;
int lightValue;

void setup() {
  Serial.begin(9600);
  Serial.println("LIGHT SLAVE~~~~~~~~~~~~~");

  pinMode(MOSI_PIN, INPUT);
  pinMode(MISO_PIN, OUTPUT);
  pinMode(SCK_PIN, INPUT);
  pinMode(SS_PIN, INPUT_PULLUP);

  SPCR |= _BV(SPE); // change mode to slave
  SPI.attachInterrupt();
}

void loop() {
  // busy wait until the ISR is called, the data is transferred and finished is set to true
  if (finished) {
    Serial.print("Light: ");
    Serial.print(lightValue);
    Serial.println(" %");
    finished = false;
  }
}

ISR (SPI_STC_vect) {
  byte value = SPDR;
  lightValue = (uint8_t) value;
  finished = true;
}
