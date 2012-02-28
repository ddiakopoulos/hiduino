/***********************************************************************
 *  HIDUINO_MIDI_Example02
 *  MIDI Input
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design, September 2011
 *  Revision 2.0
 *  http://mtiid.calarts.edu
 *************************************************************************/

#include <MIDI.h>

void HandleNoteOn(byte channel, byte pitch, byte velocity) { 
  // Do something here
}

void HandleCC(byte channel, byte pitch, byte velocity) {
  // Do something here
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) {
  // Do something here
}


void setup() {
  
  MIDI.begin(MIDI_CHANNEL_OMNI);
  
  // The latest Arduino MIDI Library (3.1) uses C style function pointers to create a callback system for handling input events. 
  MIDI.setHandleNoteOn(HandleNoteOn); 
  MIDI.setHandleControlChange(HandleCC);
  MIDI.setHandleNoteOff(HandleNoteOff);
  
}


void loop() {
  MIDI.read();
}
