/***********************************************************************
 *  HIDUINO_MIDI_Example02
 *  Example: MIDI Input
 *  by Dimitri Diakopoulos (http:// www.dimitridiakopoulos.com)
 *  Last Updated: 17 January 2013
 *************************************************************************/

#include <MIDI.h>

void HandleNoteOn(byte channel, byte pitch, byte velocity) { 
  // Do something here with your data!
}

void HandleCC(byte channel, byte pitch, byte velocity) {
  // Do something here with your data!
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) {
  // Do something here with your data!
}


void setup() {
  
  MIDI.begin(MIDI_CHANNEL_OMNI);
  
  // As of the MIDI Library v3.1, the lib uses C style function 
  // pointers to create a callback system for handling input events. 
  MIDI.setHandleNoteOn(HandleNoteOn); 
  MIDI.setHandleControlChange(HandleCC);
  MIDI.setHandleNoteOff(HandleNoteOff);
  
}


void loop() {
  MIDI.read();
}
