/***********************************************************************
 *  HIDUINO_MIDI Firmware v1.2
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design - April 2011
 *  http://mtiid.calarts.edu
 *  http://www.dimitridiakopoulos.com
 *  Based on the LUFA MIDI Demo by Dean Camera 
 * 		- http://www.fourwalledcubicle.com/LUFA.php
 ***********************************************************************/

#ifndef _AUDIO_OUTPUT_H_
#define _AUDIO_OUTPUT_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <stdbool.h>
		#include <string.h>

		#include "Descriptors.h"
				
		#include <LUFA/Version.h>
		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/Peripheral/Serial.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Drivers/USB/Class/MIDI.h>

	/* Macros: */
		/** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
		#define LEDMASK_USB_NOTREADY      LEDS_LED1

		/** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
		#define LEDMASK_USB_ENUMERATING  (LEDS_LED2 | LEDS_LED3)

		/** LED mask for the library LED driver, to indicate that the USB interface is ready. */
		#define LEDMASK_USB_READY        (LEDS_LED2 | LEDS_LED4)

		/** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
		#define LEDMASK_USB_ERROR        (LEDS_LED1 | LEDS_LED3)
		
		/** LED mask for the library LED driver, to indicate TX activity. */
		#define LEDMASK_TX               LEDS_LED1

		/** LED mask for the library LED driver, to indicate RX activity. */
		#define LEDMASK_RX               LEDS_LED2
		
		#define	MIDI_CMD_NOTE_ON         0x90
		#define	MIDI_CMD_NOTE_OFF	 	     0x80
		#define	MIDI_CMD_CC              0xB0
		#define	MIDI_CMD_PB              0xE0
		#define MIDI_CMD_SYSEX_START     0xF0   
		#define MIDI_CMD_SYSEX_STOP      0xF7  
		
		/** Two-byte System Common messages like MTC, SongSelect, etc.*/
		#define USBMIDI_COMMON_2B 0x2 

		/** Three-byte System Common messages like SPP, etc. */
		#define USBMIDI_COMMON_3B 0x3  

		/** SysEx starts or continues */
		#define USBMIDI_SYSEX_START 0x4  		

		/** Single-byte System Common Message or SysEx ends with following single byte*/
		#define USBMIDI_SYSEX_STOP_1B 0x5

		/** SysEx ends with following two bytes. */
		#define USBMIDI_SYSEX_STOP_2B 0x6

		/** SysEx ends with following three bytes. */
		#define USBMIDI_SYSEX_STOP_3B 0x7 

	/* Function Prototypes: */
		void SetupHardware(void);
		
		void MIDI_OUT(void); 
		void MIDI_IN(void);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_UnhandledControlRequest(void);

#endif
