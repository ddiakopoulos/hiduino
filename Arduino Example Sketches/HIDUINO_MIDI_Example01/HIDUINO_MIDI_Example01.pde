

/***********************************************************************
 *  HIDUINO_MIDI_Example01
 *  MIDI Output
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design, October 2011
 *  Revision 2.1
 *  http://mtiid.calarts.edu
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

  // Never actually use delays -- set up an interrupt based timer instead! 
  delay(2000); 

}


