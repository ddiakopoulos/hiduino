///////////////////////////////////////////////////////////
//  midi_input_output.ino                                //
//  Example: MIDI input & MIDI output                    //
//  https://github.com/ddiakopoulos/hiduino              //
//  Last Updated: 17 December 2015                       //
///////////////////////////////////////////////////////////

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

const int ledPin = 13; 

long previousMillis = 0; 
long interval = 1000;

void HandleNoteOn(byte channel, byte pitch, byte velocity) 
{ 
  digitalWrite(ledPin, HIGH); 
  delay(50);  
  digitalWrite(ledPin, LOW);
}


void setup() 
{
  pinMode(13, OUTPUT); 

  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(HandleNoteOn); 
}

void loop() 
{
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) 
  {
    MIDI.sendNoteOn(60, 60, 1); 
    previousMillis = currentMillis; 
  }

  MIDI.read();
}
