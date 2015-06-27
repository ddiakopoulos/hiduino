/***********************************************************************
 *  arduino_midi firmware, 02.01.2015
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Based on the LUFA low-level midi example by Dean Camera 
 *  (http://www.fourwalledcubicle.com/LUFA.php)
 *  Compiled against LUFA-140928
 ***********************************************************************/

#ifndef _arduino_midi_h
#define _arduino_midi_h

	#include <avr/io.h>
	#include <avr/wdt.h>
	#include <avr/power.h>
	#include <avr/interrupt.h>
	#include <stdbool.h>

	#include "Descriptors.h"

	#include <LUFA/Drivers/USB/USB.h>
	#include <LUFA/Drivers/Board/LEDs.h>
	#include <LUFA/Drivers/Peripheral/Serial.h>
	#include <LUFA/Platform/Platform.h>

	#define LEDMASK_USB_NOTREADY      LEDS_LED1
	#define LEDMASK_USB_ENUMERATING   LEDS_LED2
	#define LEDMASK_USB_READY         LEDS_LED2
	#define LEDMASK_USB_ERROR         LEDS_LED1

	void SetupHardware(void);
	void MIDI_To_Arduino(void);
	void MIDI_To_Host(void);

	void EVENT_USB_Device_Connect(void);
	void EVENT_USB_Device_Disconnect(void);
	void EVENT_USB_Device_ConfigurationChanged(void);

	typedef enum
	{
	    InvalidType           = 0x00,    ///< For notifying errors
	    NoteOff               = 0x80,    ///< Note Off
	    NoteOn                = 0x90,    ///< Note On
	    AfterTouchPoly        = 0xA0,    ///< Polyphonic AfterTouch
	    ControlChange         = 0xB0,    ///< Control Change / Channel Mode
	    ProgramChange         = 0xC0,    ///< Program Change
	    AfterTouchChannel     = 0xD0,    ///< Channel (monophonic) AfterTouch
	    PitchBend             = 0xE0,    ///< Pitch Bend
	    SystemExclusive       = 0xF0,    ///< System Exclusive
	    TimeCodeQuarterFrame  = 0xF1,    ///< System Common - MIDI Time Code Quarter Frame
	    SongPosition          = 0xF2,    ///< System Common - Song Position Pointer
	    SongSelect            = 0xF3,    ///< System Common - Song Select
	    TuneRequest           = 0xF6,    ///< System Common - Tune Request
	    Clock                 = 0xF8,    ///< System Real Time - Timing Clock
	    Start                 = 0xFA,    ///< System Real Time - Start
	    Continue              = 0xFB,    ///< System Real Time - Continue
	    Stop                  = 0xFC,    ///< System Real Time - Stop
	    ActiveSensing         = 0xFE,    ///< System Real Time - Active Sensing
	    SystemReset           = 0xFF,    ///< System Real Time - System Reset
	} MidiMessageType;

    uint8_t		mRunningStatus_RX;
    uint8_t		mRunningStatus_TX;
	uint8_t		mPendingMessage[3];
    uint8_t		mPendingMessageExpectedLength;
    uint8_t		mPendingMessageIndex;
    bool		mPendingMessageValid;

	MIDI_EventPacket_t mCompleteMessage;

    MidiMessageType getStatus(MidiMessageType inType, uint8_t inChannel);
    MidiMessageType getTypeFromStatusByte(uint8_t inStatus);
    uint8_t getChannelFromStatusByte(uint8_t inStatus);
	bool isChannelMessage(uint8_t inType);

#endif

