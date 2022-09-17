#include <SPI.h>

const int MISO_PIN = 50, MOSI_PIN = 51, SCK_PIN = 52, SS_PIN = 53;
volatile int msg_size = 0;
volatile boolean finished = false;
char msg_buffer[40];

void setup() {
  Serial.begin(9600);
  Serial.println("SLAVE~~~~~~~~~~~~~~~~~~~");

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
    Serial.println(msg_buffer);
    msg_size = 0;
    finished = false;
  }
}

ISR (SPI_STC_vect) {
  byte c = SPDR;
  if (msg_size < sizeof msg_buffer) {
    msg_buffer[msg_size++] = c;

    if (c == '\r')
      finished = true;
  }
}
