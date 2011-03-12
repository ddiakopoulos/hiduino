/***********************************************************************
 *  HIDUINO_MIDI v1.0
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design, February 2011
 *  http://mtiid.calarts.edu
 *************************************************************************/

#ifndef HIDUINO_MIDI_h
#define HIDUINO_MIDI_h

#include <inttypes.h>
#include "WProgram.h"
#include "HardwareSerial.h" 

#define MIDI_NOTE_ON	0x90
#define MIDI_NOTE_OFF	0x80
#define MIDI_CC			0xB0

typedef uint8_t byte;

struct midimsg {
	byte data1;
	byte data2;
	byte data3;
	int complete; 
};

class HIDUINO_MIDI_Class {

	public:
	
		HIDUINO_MIDI_Class();
		~HIDUINO_MIDI_Class();
		
		// INPUT
		void read(byte inByte);
		
		// OUTPUT
		void send(byte data1, byte data2, byte data3);
		void sendNoteOn(byte NoteNumber, byte Velocity);
		void sendNoteOff(byte NoteNumber, byte Velocity);
		void sendControlChange(byte ControlNumber, byte ControlValue);
		
		void delMsg(); 
		
		midimsg mMessage;
		
	private: 
	
			int dCount; 
		
};

extern HIDUINO_MIDI_Class MIDI;

#endif
