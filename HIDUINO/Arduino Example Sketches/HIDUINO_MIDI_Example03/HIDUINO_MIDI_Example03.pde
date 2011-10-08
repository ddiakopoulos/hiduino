/***********************************************************************
 *  HIDUINO_MIDI_Example03
 *  MIDI Input & Output
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design, October 2011
 *  Revision 2.2
 *  http://mtiid.calarts.edu
 *************************************************************************/

#include <MIDI.h>

const int ledPin = 13; 

long previousMillis = 0; 
long interval = 1000;

void HandleNoteOn(byte channel, byte pitch, byte velocity) { 
    digitalWrite(ledPin, HIGH); 
    delay(50);  
    digitalWrite(ledPin, LOW);
}


void setup() {
  
  pinMode(13, OUTPUT); 
  
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(HandleNoteOn); 
  
}

void loop() {
   
   unsigned long currentMillis = millis();
   
   if(currentMillis - previousMillis > interval) {
      MIDI.sendNoteOn(60, 60, 1); 
      previousMillis = currentMillis; 
   }
   
   MIDI.read();
   
}


