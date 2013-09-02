#include <i2c_t3.h>
#include <math.h>

#include "WiiChuckTeensy3.h"

#define MIDI_CHANNEL 1
#define DELAY 80
#define Z_THRESHOLD 500

// The nunchuck controllers
WiiChuckTeensy3 chuckL = WiiChuckTeensy3(0);
WiiChuckTeensy3 chuckR = WiiChuckTeensy3(1);

int xValL, yValL, zValL, zSumL, zAvgL;
int zValuesL[10] = {0};
int xValR, yValR, zValR, zSumR, zAvgR;
int zValuesR[10] = {0};
byte noteL, noteR;
boolean cButtonL, zButtonL;
boolean cButtonR, zButtonR;
int iL, iR;
unsigned long noteOnTimeL;
unsigned long noteOnTimeR;

void setup() {
  cButtonL = zButtonL = false;
  cButtonR = zButtonR = false;
  noteL = 40;
  noteR = 44;
  // Initialize the nunchucks
  chuckL.begin();
  chuckR.begin();
}


void loop() {
  chuckL.update(); 
  delay(1);
  zValL = chuckL.readAccelZ();
  zSumL -= zValuesL[iL];
  zSumL += zValL;
  zValuesL[iL] = zValL;
  iL = (iL + 1) % 10;
  zAvgL = zSumL / 10;
  // Instead of this, detect when zAvgL is within some distance
  // of zero, and trigger.
  // Also, in the loop, watch zValL, which should should be
  // > gravity if going down and < gravity if going up. Only
  // trigger if it looks like we're going down.
  if (zAvgL > Z_THRESHOLD) {
    if (millis() - noteOnTimeL > DELAY) {
      usbMIDI.sendNoteOn(noteL, 100, MIDI_CHANNEL);
      usbMIDI.sendNoteOff(noteL, 100, MIDI_CHANNEL);
      noteOnTimeL = millis();
    }
  }
 
 chuckR.update(); 
  delay(1);
  zValR = chuckR.readAccelZ();
  zSumR -= zValuesR[iR];
  zSumR += zValR;
  zValuesR[iR] = zValR;
  iR = (iR + 1) % 10;
  zAvgR = zSumR / 10;
  if (zAvgR > Z_THRESHOLD) {
    if (millis() - noteOnTimeR > DELAY) {
      usbMIDI.sendNoteOn(noteR, 100, MIDI_CHANNEL);
      usbMIDI.sendNoteOff(noteR, 100, MIDI_CHANNEL);
      noteOnTimeR = millis();
    }
  }  
  
  if (chuckL.cPressed()) {
    if (!cButtonL) {
      // Rising edge
      noteL++;
      cButtonL = true;
    }
  } else {
    cButtonL = false;
  }
  if (chuckL.zPressed()) {
    if (!zButtonL) {
      // Rising edge
      noteL--;
      zButtonL = true;
    }
  } else {
    zButtonL = false;
  }
  
  if (chuckR.cPressed()) {
    if (!cButtonR) {
      // Rising edge
      noteR++;
      cButtonR = true;
    }
  } else {
    cButtonR = false;
  }
  if (chuckR.zPressed()) {
    if (!zButtonR) {
      // Rising edge
      noteR--;
      zButtonR = true;
    }
  } else {
    zButtonR = false;
  }
}
