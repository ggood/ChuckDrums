#include <math.h>

#include "Wire.h"
#include "WiiChuck.h"

#define MIDI_CHANNEL 1
#define DELAY 100

WiiChuck chuck = WiiChuck(); // The nunchuck controller

int xVal, yVal, zVal, zSum, zAvg;
int zValues[10] = {0};
byte note;
boolean cButton, zButton;
int i;
unsigned long noteOnTime;

void setup() {
  cButton = zButton = false;
  note = 40;
  chuck.begin();  // Initialize the nunchuck
}


void loop() {
  chuck.update(); 
  delay(1);
  zVal = chuck.readAccelZ();
  zSum -= zValues[i];
  zSum += zVal;
  zValues[i] = zVal;
  i = (i + 1) % 10;
  zAvg = zSum / 10;
  if (zAvg > 500) {
    if (millis() - noteOnTime > DELAY) {
      usbMIDI.sendNoteOn(note, 100, MIDI_CHANNEL);
      usbMIDI.sendNoteOff(note, 100, MIDI_CHANNEL);
      noteOnTime = millis();
    }
  } 
  
  if (chuck.cPressed()) {
    if (!cButton) {
      // Rising edge
      note++;
      cButton = true;
    }
  } else {
    cButton = false;
  }
  if (chuck.zPressed()) {
    if (!zButton) {
      // Rising edge
      note--;
      zButton = true;
    }
  } else {
    zButton = false;
  }
}
