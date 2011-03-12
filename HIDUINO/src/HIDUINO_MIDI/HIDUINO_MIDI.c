/***********************************************************************
 *  HIDUINO_MIDI Firmware v1.1
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design - January 2011
 *  http://mtiid.calarts.edu
 *  http://www.dimitridiakopoulos.com
 *  Based on the LUFA MIDI Demo by Dean Camera 
 * 		- http://www.fourwalledcubicle.com/LUFA.php
 ***********************************************************************/

#include "HIDUINO_MIDI.h"

volatile struct
{
	uint8_t TxLEDPulse;
	uint8_t RxLEDPulse; 
	uint8_t PingPongLEDPulse;
} PulseMSRemaining;

MIDI_EventPacket_t MIDI_FROM_ARDUINO; 

// Counters to keep track of recieved bytes
volatile uint8_t dCount = 0;
volatile uint8_t complete = 0; 

USB_ClassInfo_MIDI_Device_t Keyboard_MIDI_Interface =
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

		serialToUSB(); 
	
		MIDI_EventPacket_t ReceivedMIDIEvent;
		
		if (MIDI_Device_ReceiveEventPacket(&Keyboard_MIDI_Interface, &ReceivedMIDIEvent)) {
			if ((ReceivedMIDIEvent.Command == (MIDI_COMMAND_NOTE_ON >> 4)) && (ReceivedMIDIEvent.Data3 > 0)){
			  LEDs_SetAllLEDs(LEDMASK_RX);
			  usbToSerial(ReceivedMIDIEvent.Data1, ReceivedMIDIEvent.Data2, ReceivedMIDIEvent.Data3); 
			}
			else
			  LEDs_SetAllLEDs(LEDS_NO_LEDS);	
		}

		MIDI_Device_USBTask(&Keyboard_MIDI_Interface);
		USB_USBTask();
	}
	
}


// Configures the board hardware and chip peripherals for the demo's functionality.
void SetupHardware(void) {

	// Disable watchdog if enabled by bootloader/fuses
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	
	// Serial Interrupts
	UCSR1B |= (1 << RXCIE1) | (1 << TXCIE1);
	
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
	ConfigSuccess &= MIDI_Device_ConfigureEndpoints(&Keyboard_MIDI_Interface);
	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

// Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void) {
	MIDI_Device_ProcessControlRequest(&Keyboard_MIDI_Interface);
}


// Reads our Arduino/MIDI protocol via USART and relays as properly formed MIDI messages over USB 
void serialToUSB(void) {

	USART_GetByte(); 
	
	uint8_t Channel = 0; 
	
	if (MIDI_FROM_ARDUINO.Data3 > 0 && complete == 1) {
	
		complete = 0; 
		
		MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t) {
				.CableNumber = 0,
				.Command     = MIDI_FROM_ARDUINO.Data1 >> 4,
				.Data1       = MIDI_FROM_ARDUINO.Data1 | Channel, 
				.Data2       = MIDI_FROM_ARDUINO.Data2, 
				.Data3       = MIDI_FROM_ARDUINO.Data3,		
			};
			
		MIDI_Device_SendEventPacket(&Keyboard_MIDI_Interface, &MIDIEvent);
		MIDI_Device_Flush(&Keyboard_MIDI_Interface);

	}
	
}


// Keep track of incoming bytes over USART
void USART_GetByte() {

	if (Serial_IsCharReceived()) {

		uint8_t ReceivedByte = Serial_RxByte(); 
			
		if ( (ReceivedByte >> 7 ) == 1 ) {
			dCount = 0; 
			delMsg(); 
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
	
}


// usb to serial sender
void usbToSerial(uint8_t data1, uint8_t data2, uint8_t data3) {
	USART_SendByte(data1);
	USART_SendByte(data2);
	USART_SendByte(data3); 	
}


// push a single the byte over USART
void USART_SendByte(uint8_t data) { 
	Serial_TxByte(data); 
}


void delMsg() {
	MIDI_FROM_ARDUINO.Data1 = 0; 
	MIDI_FROM_ARDUINO.Data2 = 0; 
	MIDI_FROM_ARDUINO.Data3 = 0; 
	complete = 0; 
}