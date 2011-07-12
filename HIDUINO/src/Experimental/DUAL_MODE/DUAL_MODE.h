/***********************************************************************
 *  HIDUINO DUAL_MODE Firmware v1.2
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design - May 2011
 *  http://mtiid.calarts.edu
 *  http://www.dimitridiakopoulos.com
 ***********************************************************************/

#ifndef _HIDUINO_MIDI_H_
#define _HIDUINO_MIDI_H_

	/* Includes: */
	
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/interrupt.h>
		#include <avr/power.h>
				
		#include <LUFA/Version.h>
		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/Peripheral/Serial.h>
		#include <LUFA/Drivers/USB/USB.h>
		
		#include "Lib/LightweightRingBuff.h"
		
		// MIDI
		#include "MIDI_Descriptors.h"
		#include <LUFA/Drivers/USB/Class/MIDI.h>
		
		// CDC-ACM Virtual Serial
		#include "CDC_Descriptors.h"
		#include <LUFA/Drivers/USB/Class/CDC.h>
		
	/* Macros: */
		#define MODE_MIDI				1
		#define MODE_SERIAL				0
			
		#define	MIDI_CMD_NOTE_ON        0x90
		#define	MIDI_CMD_NOTE_OFF	 	0x80
		#define	MIDI_CMD_CC             0xB0
		#define	MIDI_CMD_PB             0xE0
		
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

	/* Function Prototypes: */
	
		void SetupHardware(void);
		
		void Serial_Task(void);
		void MIDI_Task(void);
		void MIDI_OUT(void); 
		void MIDI_IN(void); 	

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);
	
		void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo);
		void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo);

		uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex, const void** const DescriptorAddress) ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);
							
#endif
