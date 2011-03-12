/***********************************************************************
 *  HIDUINO_MIDI_Example02
 *  MIDI Input
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design, February 2011
 *  http://mtiid.calarts.edu
 *************************************************************************/

#include <HIDUINO_MIDI_Lib.h>

void setup() {
  Serial.begin(9600);
}


void loop() {

  if (Serial.available() > 0) {
    MIDI.read(Serial.read()); 
  }

  // Read MIDI note on message: 
  if (MIDI.mMessage.data1 == 144 && MIDI.mMessage.data3 >= 0 && MIDI.mMessage.complete == 1) {
    MIDI.mMessage.complete = 0; 

    // Your code here

  }

}


