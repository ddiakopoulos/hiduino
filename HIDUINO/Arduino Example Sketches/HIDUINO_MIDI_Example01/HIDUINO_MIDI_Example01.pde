/***********************************************************************
 *  HIDUINO_MIDI_Example01
 *  MIDI Output
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design, September 2011
 *  Revision 2.0
 *  http://mtiid.calarts.edu
 *************************************************************************/

#include <MIDI.h>

void setup() {}


void loop() {

  // Send Note On
  for (int i = 0; i < 24; i++) {
    MIDI.sendNoteOn(i, i*2); 
    delay(25); 
  }
  
  // Send Note Off
  for (int i = 0; i < 24; i++) {
    MIDI.sendNoteOff(i, i*2); 
    delay(25); 
  }
  
  // Send Control Change
  for (int i = 0; i < 24; i++) {
    MIDI.sendControlChange(i, i*2); 
    delay(25); 
  }
  
  // Never actually use delays -- set up an interrupt based timer instead! 
  delay(500); 

}


