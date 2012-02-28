/***********************************************************************
 *  HIDUINO_MIDI Firmware v1.3dev
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design - July 2011
 *  http://mtiid.calarts.edu
 *  http://www.dimitridiakopoulos.com
 ***********************************************************************/

#include "HIDUINO_MIDI.h"

MIDI_EventPacket_t MIDI_FROM_ARDUINO; 

// Counters to keep track of recieved bytes
volatile uint8_t dCount = 0;
volatile uint8_t complete = 0; 

USB_ClassInfo_MIDI_Device_t MIDI_Interface =
	{
		.Config =
			{
				.StreamingInterfaceNumber = 1,

				.DataINEndpointNumber      = MIDI_STREAM_IN_EPNUM,
				.DataINEndpointSize        = MIDI_STREAM_EPSIZE,
				.DataINEndpointDoubleBank  = false,

				.DataOUTEndpointNumber     = MIDI_STREAM_OUT_EPNUM,
				.DataOUTEndpointSize       = MIDI_STREAM_EPSIZE,
				.DataOUTEndpointDoubleBank = false,
			},
	};

	
int main(void) {

	SetupHardware();
	
	sei();
	
	for (;;) { 

		MIDI_IN();
		MIDI_OUT(); 

		MIDI_Device_USBTask(&MIDI_Interface);
		USB_USBTask();
		
	}
	
}


// Configures the board hardware and chip peripherals for the demo's functionality.
void SetupHardware(void) {

	// Disable watchdog if enabled by bootloader/fuses
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	
	// Hardware Initialization  
	Serial_Init(9600, false);
	
	LEDs_Init();
	USB_Init();
	
	// Start the flush timer so that overflows occur rapidly to push received bytes to the USB interface
	TCCR0B = (1 << CS02);

}


// Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void) {
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

// Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void) {
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

// Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;
	ConfigSuccess &= MIDI_Device_ConfigureEndpoints(&MIDI_Interface);
	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

// Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void) {
	MIDI_Device_ProcessControlRequest(&MIDI_Interface);
}


void MIDI_IN(void) {

	MIDI_EventPacket_t ReceivedMIDIEvent;
	
	if (MIDI_Device_ReceiveEventPacket(&MIDI_Interface, &ReceivedMIDIEvent)) {
		LEDs_TurnOnLEDs(LEDMASK_RX);
		Serial_TxByte(ReceivedMIDIEvent.Data1);
		Serial_TxByte(ReceivedMIDIEvent.Data2); 
		Serial_TxByte(ReceivedMIDIEvent.Data3); 
	}
	
	else {
		LEDs_TurnOffLEDs(LEDMASK_RX);
	}
	
}

void MIDI_OUT(void) {
	
	uint8_t Channel = 0; 
	
	if (Serial_IsCharReceived()) {
	
		LEDs_TurnOnLEDs(LEDMASK_TX);
		uint8_t ReceivedByte = Serial_RxByte(); 
		
		if ( (ReceivedByte >> 7 ) == 1 ) {
			dCount = 0; 
			memset(&MIDI_FROM_ARDUINO, -1, sizeof(MIDI_EventPacket_t));
			MIDI_FROM_ARDUINO.Data1 = ReceivedByte;
		}
			
		else if ( ((ReceivedByte >> 7) == 0) && (dCount == 0) ) {
			dCount = 1; 
			MIDI_FROM_ARDUINO.Data2 = ReceivedByte;
		}
		
		else if ( ((ReceivedByte >> 7) == 0) && (dCount == 1) ) {
			dCount = 0; 
			MIDI_FROM_ARDUINO.Data3 = ReceivedByte;
			complete = 1; 
		}
		
	}
	
	else {
		LEDs_TurnOffLEDs(LEDMASK_TX);
	}
	
	if (complete) {
	
		complete = 0; 

		MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t) {
				.CableNumber = 0,
				.Command     = MIDI_FROM_ARDUINO.Data1 >> 4,
				.Data1       = MIDI_FROM_ARDUINO.Data1 | Channel, 
				.Data2       = MIDI_FROM_ARDUINO.Data2, 
				.Data3       = MIDI_FROM_ARDUINO.Data3,		
			};
			
		MIDI_Device_SendEventPacket(&MIDI_Interface, &MIDIEvent);
		MIDI_Device_Flush(&MIDI_Interface);
		
	}
	
}