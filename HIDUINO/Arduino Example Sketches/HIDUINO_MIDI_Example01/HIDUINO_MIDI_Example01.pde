/***********************************************************************
 *  HIDUINO_MIDI_Example01
 *  MIDI Output
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design, February 2011
 *  http://mtiid.calarts.edu
 *************************************************************************/

#include <HIDUINO_MIDI_Lib.h>

void setup() {
  Serial.begin(9600);
}


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


