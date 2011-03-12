/***********************************************************************
 *  HIDUINO_MIDI v1.0
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design, February 2011
 *  http://mtiid.calarts.edu
 *************************************************************************/

#include "WProgram.h"
#include <inttypes.h> 
#include <stdlib.h>
#include "HIDUINO_MIDI.h"

// Extern Object
HIDUINO_MIDI_Class MIDI;

// Constructor
HIDUINO_MIDI_Class::HIDUINO_MIDI_Class() {}

// Deconstructor
HIDUINO_MIDI_Class::~HIDUINO_MIDI_Class() {}

void HIDUINO_MIDI_Class::send(byte data1, byte data2, byte data3) {

	data2 &= 0x7F;
	data3 &= 0x7F;

	Serial.write(data1);
	Serial.write(data2); 
	Serial.write(data3);
	
}


void HIDUINO_MIDI_Class::sendNoteOn(byte NoteNumber, byte Velocity) { 
	send(MIDI_NOTE_ON, NoteNumber, Velocity); 
}

void HIDUINO_MIDI_Class::sendNoteOff(byte NoteNumber, byte Velocity) { 
	send(MIDI_NOTE_OFF, NoteNumber, Velocity); 
}

void HIDUINO_MIDI_Class::sendControlChange(byte ControlNumber, byte ControlValue) { 
	send(MIDI_CC, ControlNumber, ControlValue); 
}

void HIDUINO_MIDI_Class::delMsg() {
	mMessage.data1 = 0;
	mMessage.data2 = 0;
	mMessage.data3 = 0;
	mMessage.complete = 0; 
}

void HIDUINO_MIDI_Class::read(byte inByte) {

	if ( ((inByte >> 7) == 1 ) ) {
		dCount = 0;		
		delMsg(); 
		mMessage.data1 = inByte;
	}
		
	else if ( ((inByte >> 7) == 0) && (dCount == 0) ) {
		dCount = 1; 
		mMessage.data2 = inByte;
	}
	
	else if ( ((inByte >> 7) == 0) && (dCount == 1) ) {
		dCount = 0; 
		mMessage.data3 = inByte;
		mMessage.complete = 1; 
	}
	
}