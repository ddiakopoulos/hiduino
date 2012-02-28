/***********************************************************************
 *  HIDUINO DUAL_MODE Firmware v1.2
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Music Technology: Interaction, Intelligence & Design - May 2011
 *  http://mtiid.calarts.edu
 *  http://www.dimitridiakopoulos.com
 ***********************************************************************/
 
#include "DUAL_MODE.h"

// Set this in SetupHardware(): 
bool CURRENT_MODE = MODE_MIDI;

volatile struct {
	uint8_t TxLEDPulse;
	uint8_t RxLEDPulse; 
	uint8_t PingPongLEDPulse;
} PulseMSRemaining;

// CDC-ACM Virtual Serial Interface - 3 endpoints
USB_ClassInfo_CDC_Device_t VirtualSerial_Interface =
	{
		.Config = 
			{
				.ControlInterfaceNumber         = 0,

				.DataINEndpointNumber           = CDC_TX_EPNUM,
				.DataINEndpointSize             = CDC_TXRX_EPSIZE,
				.DataINEndpointDoubleBank       = false,

				.DataOUTEndpointNumber          = CDC_RX_EPNUM,
				.DataOUTEndpointSize            = CDC_TXRX_EPSIZE,
				.DataOUTEndpointDoubleBank      = false,

				.NotificationEndpointNumber     = CDC_NOTIFICATION_EPNUM,
				.NotificationEndpointSize       = CDC_NOTIFICATION_EPSIZE,
				.NotificationEndpointDoubleBank = false,
			},
	};

// MIDI Interface - 2 endpoints
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

MIDI_EventPacket_t MIDI_FROM_ARDUINO; 

volatile uint8_t dCount = 0;
volatile uint8_t complete = 0; 

RingBuff_t USBtoUSART_Buffer;
RingBuff_t USARTtoUSB_Buffer;
	
	
int main(void) {
	
	SetupHardware();
		
	sei();
	
	for (;;) { 
		
		if (CURRENT_MODE == MODE_SERIAL) {
			Serial_Task(); 
		} 
		
		else {
			MIDI_Task();
		}
		
		USB_USBTask();

	}
	
}


void SetupHardware(void) {
	
	// For selecting which firmware to use during power-on
	DDRB &= ~(_BV(PORTB1)); 				// Make PB1 an input
	PORTB |= _BV(PORTB1); 					// Enable pull-up on PB1
	CURRENT_MODE = PINB & _BV(PORTB1); 		// Read value of PB1
	PORTB &= ~(_BV(PORTB1)); 				// Disable pull-up on PB1 

	// Disable watchdog if enabled by bootloader/fuses
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
				
	// Hardware Initialization  
	Serial_Init(9600, false);
	LEDs_Init();
	USB_Init();
	
	// Start the flush timer so that overflows occur rapidly to push received bytes to the USB interface
	TCCR0B = (1 << CS02);
	
	/* Pull target /RESET line high */
	AVR_RESET_LINE_PORT |= AVR_RESET_LINE_MASK;
	AVR_RESET_LINE_DDR  |= AVR_RESET_LINE_MASK;
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
	
	if (CURRENT_MODE == MODE_SERIAL) {
		RingBuffer_InitBuffer(&USBtoUSART_Buffer);
		RingBuffer_InitBuffer(&USARTtoUSB_Buffer);
		ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_Interface);
	}
	else {
		UCSR1B |= (1 << RXCIE1) | (1 << TXCIE1);
		ConfigSuccess &= MIDI_Device_ConfigureEndpoints(&MIDI_Interface);
	}
	
	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
	
}


// Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void) {
	
	if (CURRENT_MODE == MODE_SERIAL)
	  CDC_Device_ProcessControlRequest(&VirtualSerial_Interface);
	else 
	  MIDI_Device_ProcessControlRequest(&MIDI_Interface);
	
}

void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo) {

	uint8_t ConfigMask = 0;
		
	switch (CDCInterfaceInfo->State.LineEncoding.ParityType) {
		case CDC_PARITY_Odd:
			ConfigMask = ((1 << UPM11) | (1 << UPM10));		
			break;
		case CDC_PARITY_Even:
			ConfigMask = (1 << UPM11);		
			break;
	}

	if (CDCInterfaceInfo->State.LineEncoding.CharFormat == CDC_LINEENCODING_TwoStopBits)
	  ConfigMask |= (1 << USBS1);

	switch (CDCInterfaceInfo->State.LineEncoding.DataBits) {
		case 6:
			ConfigMask |= (1 << UCSZ10);
			break;
		case 7:
			ConfigMask |= (1 << UCSZ11);
			break;
		case 8:
			ConfigMask |= ((1 << UCSZ11) | (1 << UCSZ10));
			break;
	}

	/* Must turn off USART before reconfiguring it, otherwise incorrect operation may occur */
	UCSR1B = 0;
	UCSR1A = 0;
	UCSR1C = 0;

	/* Special case 57600 baud for compatibility with the ATmega328 bootloader. */	
	UBRR1  = (CDCInterfaceInfo->State.LineEncoding.BaudRateBPS == 57600)
			 ? SERIAL_UBBRVAL(CDCInterfaceInfo->State.LineEncoding.BaudRateBPS)
			 : SERIAL_2X_UBBRVAL(CDCInterfaceInfo->State.LineEncoding.BaudRateBPS);	

	UCSR1C = ConfigMask;
	UCSR1A = (CDCInterfaceInfo->State.LineEncoding.BaudRateBPS == 57600) ? 0 : (1 << U2X1);
	UCSR1B = ((1 << RXCIE1) | (1 << TXEN1) | (1 << RXEN1));
			 
}


void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo) {
	
	bool CurrentDTRState = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR);
		
	if (CurrentDTRState)
		AVR_RESET_LINE_PORT &= ~AVR_RESET_LINE_MASK;
	else
		AVR_RESET_LINE_PORT |= AVR_RESET_LINE_MASK;
	
}

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue, const uint8_t wIndex, const void** const DescriptorAddress) {
	
	if (CURRENT_MODE == MODE_SERIAL)
		return Serial_GetDescriptor(wValue, wIndex, DescriptorAddress);
	else
		return MIDI_GetDescriptor(wValue, wIndex, DescriptorAddress);

}


void Serial_Task(void) {

	/* Only try to read in bytes from the CDC interface if the transmit buffer is not full */
	if (!(RingBuffer_IsFull(&USBtoUSART_Buffer))) {
		int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial_Interface);

		/* Read bytes from the USB OUT endpoint into the USART transmit buffer */
		if (!(ReceivedByte < 0))
		  RingBuffer_Insert(&USBtoUSART_Buffer, ReceivedByte);
	}
	
	/* Check if the UART receive buffer flush timer has expired or the buffer is nearly full */
	RingBuff_Count_t BufferCount = RingBuffer_GetCount(&USARTtoUSB_Buffer);
	if ((TIFR0 & (1 << TOV0)) || (BufferCount > BUFFER_NEARLY_FULL)) {
		TIFR0 |= (1 << TOV0);

		if (USARTtoUSB_Buffer.Count) {
			LEDs_TurnOnLEDs(LEDMASK_TX);
			PulseMSRemaining.TxLEDPulse = TX_RX_LED_PULSE_MS;
		}

		/* Read bytes from the USART receive buffer into the USB IN endpoint */
		while (BufferCount--)
		  CDC_Device_SendByte(&VirtualSerial_Interface, RingBuffer_Remove(&USARTtoUSB_Buffer));
		  
		/* Turn off TX LED(s) once the TX pulse period has elapsed */
		if (PulseMSRemaining.TxLEDPulse && !(--PulseMSRemaining.TxLEDPulse))
		  LEDs_TurnOffLEDs(LEDMASK_TX);

		/* Turn off RX LED(s) once the RX pulse period has elapsed */
		if (PulseMSRemaining.RxLEDPulse && !(--PulseMSRemaining.RxLEDPulse))
		  LEDs_TurnOffLEDs(LEDMASK_RX);
	}
	
	/* Load the next byte from the USART transmit buffer into the USART */
	if (!(RingBuffer_IsEmpty(&USBtoUSART_Buffer))) {
	  Serial_TxByte(RingBuffer_Remove(&USBtoUSART_Buffer));
		
		LEDs_TurnOnLEDs(LEDMASK_RX);
		PulseMSRemaining.RxLEDPulse = TX_RX_LED_PULSE_MS;
	}

	CDC_Device_USBTask(&VirtualSerial_Interface);

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

	if (complete == 1) {

		complete = 0;
		
		LEDs_TurnOnLEDs(LEDMASK_TX);
		
		uint8_t Channel = 0;  

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
	
	else {
		LEDs_TurnOffLEDs(LEDMASK_TX);
	}

}

void MIDI_Task(void) {

	MIDI_IN();
	MIDI_OUT(); 

	MIDI_Device_USBTask(&MIDI_Interface);

}

ISR(USART1_RX_vect, ISR_BLOCK) {
	
	uint8_t ReceivedByte = UDR1;
	
	if (USB_DeviceState == DEVICE_STATE_Configured) {
	
		if (CURRENT_MODE == MODE_SERIAL) {
			RingBuffer_Insert(&USARTtoUSB_Buffer, ReceivedByte);
		}
		
		else {

			if ( (ReceivedByte >> 7 ) == 1 ) {
				dCount = 0; 
				memset(&MIDI_FROM_ARDUINO, 0, sizeof(MIDI_EventPacket_t));
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
	
}