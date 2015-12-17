///////////////////////////////////////////////////////////
//  midi_output.ino                                      //
//  Example: MIDI Output                                 //
//  https://github.com/ddiakopoulos/hiduino              //
//  Last Updated: 17 December 2015                       //
///////////////////////////////////////////////////////////

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() 
{
    MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() 
{

  for (int i = 0; i < 24; i++) 
  {
    MIDI.sendNoteOn(i, i*2, 1); 
    delay(75); 
  }

  // Delay == Bad! Set up an interrupt based timer instead 
  // (or something like Blink Without Delay)
  delay(2000); 

}
