#include <Keypad.h>
#include "pitches.h"
#include "mario.h"
#include "ode_to_joy.h"
#include "the_imperial_march.h"

/************************************************************************/
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

const int PIEZO_PIN = 8, PITCH_PIN = A0, PLAY_PIN = 13;

/************************************************************************/
float scale = 0;
int song = 0;
int *melodies[] = {mario_melody, odeToJoy_melody, theImperialMarch_melody};
int sizes[] = {sizeof(mario_melody) / sizeof(int), sizeof(odeToJoy_melody) / sizeof(int), sizeof(theImperialMarch_melody) / sizeof(int)};
int tempo[] = {mario_tempo, odeToJoy_tempo, theImperialMarch_tempo};
float *noteDurations[] = {mario_noteDurations, odeToJoy_noteDurations, theImperialMarch_noteDurations};

/************************************************************************/

void setup() {
  pinMode(PLAY_PIN, INPUT_PULLUP);
}

void loop() {
  if (1 - digitalRead(PLAY_PIN)) {
    for (int note = 0; note < sizes[song]; note++) {
      scale = analogRead(PITCH_PIN) / 512.0;
      int duration = 120 * 1000.0 / (tempo[song] * noteDurations[song][note]);
      tone(PIEZO_PIN, int(melodies[song][note] * scale), duration);
      delay(int(duration * 1.3));
      if (digitalRead(PLAY_PIN))
        break;
    }
  }
  else {
    char key = keypad.getKey();
    if (key != NO_KEY)
      song = key - '1';
  }
}
