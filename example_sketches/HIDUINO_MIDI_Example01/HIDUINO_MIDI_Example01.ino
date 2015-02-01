/***********************************************************************
 *  HIDUINO_MIDI_Example01
 *  Example: MIDI Output
 *  by Dimitri Diakopoulos (http:// www.dimitridiakopoulos.com)
 *  Last Updated: 17 January 2013
 *************************************************************************/

#include <MIDI.h>

void setup() {
    MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {

  for (int i = 0; i < 24; i++) {
    MIDI.sendNoteOn(i, i*2, 1); 
    delay(75); 
  }

  // Delay == Bad! Set up an interrupt based timer instead 
  // (or something like Blink Without Delay)
  delay(2000); 

}