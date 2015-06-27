/***********************************************************************
 *  arduino_midi firmware, 02.01.2015
 *  by Dimitri Diakopoulos (http://www.dimitridiakopoulos.com)
 *  Based on the LUFA low-level midi example by Dean Camera 
 *  (http://www.fourwalledcubicle.com/LUFA.php)
 *  Compiled against LUFA-140928
 ***********************************************************************/

#include "arduino_midi.h"

uint16_t tx_ticks = 0; 
uint16_t rx_ticks = 0; 
const uint16_t TICK_COUNT = 5000; 

int main(void)
{
	SetupHardware();

	GlobalInterruptEnable();

	sei();

	for (;;)
	{
		if (tx_ticks > 0) 
		{
			tx_ticks--;
		}
		else if (tx_ticks == 0)
		{
			LEDs_TurnOffLEDs(LEDS_LED2);
		}
									
		if (rx_ticks > 0)
		{
			rx_ticks--;
		}
		else if (rx_ticks == 0)
		{
			LEDs_TurnOffLEDs(LEDS_LED1);
		}
			
		MIDI_To_Arduino();
		MIDI_To_Host();

		USB_USBTask();
	}
}

void SetupHardware(void)
{

	// Disable watchdog if enabled by bootloader/fuses
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	Serial_Init(31250, false);

	// Start the flush timer so that overflows occur rapidly to
	// push received bytes to the USB interface
	TCCR0B = (1 << CS02);
			
	// Serial Interrupts
	UCSR1B = 0;
	UCSR1B = ((1 << RXCIE1) | (1 << TXEN1) | (1 << RXEN1));

	// https://github.com/ddiakopoulos/hiduino/issues/13
	/* Target /ERASE line is active HIGH: there is a mosfet that inverts logic */
	// These are defined in the makefile... 
	AVR_ERASE_LINE_PORT |= AVR_ERASE_LINE_MASK;
	AVR_ERASE_LINE_DDR |= AVR_ERASE_LINE_MASK; 

	// Disable clock division
	clock_prescale_set(clock_div_1);

	LEDs_Init();
	USB_Init();
}

///////////////////////////////////////////////////////////////////////////////
// USB Configuration
///////////////////////////////////////////////////////////////////////////////

// Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs.
void EVENT_USB_Device_Connect(void)
{
	/* Indicate USB enumerating */
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

// Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
// the status LEDs, disables the sample update and PWM output timers and stops the USB and MIDI management tasks.
void EVENT_USB_Device_Disconnect(void)
{
	/* Indicate USB not ready */
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

// Event handler for the USB_ConfigurationChanged event. This is fired when the host set the current configuration
// of the USB device after enumeration - the device endpoints are configured and the MIDI management task started.
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup MIDI Data Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(MIDI_STREAM_IN_EPADDR, EP_TYPE_BULK, MIDI_STREAM_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(MIDI_STREAM_OUT_EPADDR, EP_TYPE_BULK, MIDI_STREAM_EPSIZE, 1);

	/* Indicate endpoint configuration success or failure */
	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

///////////////////////////////////////////////////////////////////////////////
// MIDI Worker Functions
///////////////////////////////////////////////////////////////////////////////

// From Arduino/Serial to USB/Host 
void MIDI_To_Host(void)
{
	// Device must be connected and configured for the task to run
	if (USB_DeviceState != DEVICE_STATE_Configured) return;

	// Select the MIDI IN stream
	Endpoint_SelectEndpoint(MIDI_STREAM_IN_EPADDR);

	if (Endpoint_IsINReady())
	{
		if (mPendingMessageValid == true)
		{
			mPendingMessageValid = false;

			// Write the MIDI event packet to the endpoint
			Endpoint_Write_Stream_LE(&mCompleteMessage, sizeof(mCompleteMessage), NULL);

			// Clear out complete message
			memset(&mCompleteMessage, 0, sizeof(mCompleteMessage)); 

			// Send the data in the endpoint to the host
			Endpoint_ClearIN();

			LEDs_TurnOnLEDs(LEDS_LED2);
			tx_ticks = TICK_COUNT; 
		}
	}

}

// From USB/Host to Arduino/Serial
void MIDI_To_Arduino(void)
{
	// Device must be connected and configured for the task to run
	if (USB_DeviceState != DEVICE_STATE_Configured) return;

	// Select the MIDI OUT stream
	Endpoint_SelectEndpoint(MIDI_STREAM_OUT_EPADDR);

	/* Check if a MIDI command has been received */
	if (Endpoint_IsOUTReceived())
	{
		MIDI_EventPacket_t MIDIEvent;

		/* Read the MIDI event packet from the endpoint */
		Endpoint_Read_Stream_LE(&MIDIEvent, sizeof(MIDIEvent), NULL);

		// Passthrough to Arduino
		Serial_SendByte(MIDIEvent.Data1);
		Serial_SendByte(MIDIEvent.Data2); 
		Serial_SendByte(MIDIEvent.Data3); 

		LEDs_TurnOnLEDs(LEDS_LED1);
		rx_ticks = TICK_COUNT;

		/* If the endpoint is now empty, clear the bank */
		if (!(Endpoint_BytesInEndpoint()))
		{
			/* Clear the endpoint ready for new packet */
			Endpoint_ClearOUT();
		}
	}

}

// Parse via Arduino/Serial
ISR(USART1_RX_vect, ISR_BLOCK) 
{
	// Device must be connected and configured for the task to run
	if (USB_DeviceState != DEVICE_STATE_Configured) return;

	const uint8_t extracted = UDR1;

	// Borrowed + Modified from Francois Best's Arduino MIDI Library
	// https://github.com/FortySevenEffects/arduino_midi_library
    if (mPendingMessageIndex == 0)
    {
        // Start a new pending message
        mPendingMessage[0] = extracted;

        // Check for running status first
        if (isChannelMessage(getTypeFromStatusByte(mRunningStatus_RX)))
        {
            // Only these types allow Running Status

            // If the status byte is not received, prepend it to the pending message
            if (extracted < 0x80)
            {
                mPendingMessage[0]   = mRunningStatus_RX;
                mPendingMessage[1]   = extracted;
                mPendingMessageIndex = 1;
            }
            // Else we received another status byte, so the running status does not apply here.
            // It will be updated upon completion of this message.
        }

        switch (getTypeFromStatusByte(mPendingMessage[0]))
        {
            // 1 byte messages
            case Start:
            case Continue:
            case Stop:
            case Clock:
            case ActiveSensing:
            case SystemReset:
            case TuneRequest:
                // Handle the message type directly here.
            	mCompleteMessage.Event 	 = MIDI_EVENT(0, getTypeFromStatusByte(mPendingMessage[0]));
                mCompleteMessage.Data1   = mPendingMessage[0];
                mCompleteMessage.Data2   = 0;
                mCompleteMessage.Data3   = 0;
                mPendingMessageValid  	 = true;

                // We still need to reset these
                mPendingMessageIndex = 0;
                mPendingMessageExpectedLength = 0;

                return;
                break;

            // 2 bytes messages
            case ProgramChange:
            case AfterTouchChannel:
            case TimeCodeQuarterFrame:
            case SongSelect:
                mPendingMessageExpectedLength = 2;
                break;

            // 3 bytes messages
            case NoteOn:
            case NoteOff:
            case ControlChange:
            case PitchBend:
            case AfterTouchPoly:
            case SongPosition:
                mPendingMessageExpectedLength = 3;
                break;

            case SystemExclusive:
                break;

            case InvalidType:
            default:
                // Something bad happened
                break;
        }

        if (mPendingMessageIndex >= (mPendingMessageExpectedLength - 1))
        {
            // Reception complete
            mCompleteMessage.Event = MIDI_EVENT(0, getTypeFromStatusByte(mPendingMessage[0]));
            mCompleteMessage.Data1 = mPendingMessage[0]; // status = channel + type
 			mCompleteMessage.Data2 = mPendingMessage[1];

            // Save Data3 only if applicable
            if (mPendingMessageExpectedLength == 3)
                mCompleteMessage.Data3 = mPendingMessage[2];
            else
                mCompleteMessage.Data3 = 0;

            mPendingMessageIndex = 0;
            mPendingMessageExpectedLength = 0;
            mPendingMessageValid = true;
            return;
        }
        else
        {
            // Waiting for more data
            mPendingMessageIndex++;
        }
    }
    else
    {
        // First, test if this is a status byte
        if (extracted >= 0x80)
        {
            // Reception of status bytes in the middle of an uncompleted message
            // are allowed only for interleaved Real Time message or EOX
            switch (extracted)
            {
                case Clock:
                case Start:
                case Continue:
                case Stop:
                case ActiveSensing:
                case SystemReset:

                    // Here we will have to extract the one-byte message,
                    // pass it to the structure for being read outside
                    // the MIDI class, and recompose the message it was
                    // interleaved into. Oh, and without killing the running status..
                    // This is done by leaving the pending message as is,
                    // it will be completed on next calls.
           		 	mCompleteMessage.Event = MIDI_EVENT(0, getTypeFromStatusByte(extracted));
            		mCompleteMessage.Data1 = extracted;
                    mCompleteMessage.Data2 = 0;
                    mCompleteMessage.Data3 = 0;
                   	mPendingMessageValid   = true;
                    return;
                    break;
                default:
                    break;
            }
        }

        // Add extracted data byte to pending message
        mPendingMessage[mPendingMessageIndex] = extracted;

        // Now we are going to check if we have reached the end of the message
        if (mPendingMessageIndex >= (mPendingMessageExpectedLength - 1))
        {

        	mCompleteMessage.Event = MIDI_EVENT(0, getTypeFromStatusByte(mPendingMessage[0]));
            mCompleteMessage.Data1 = mPendingMessage[0];
            mCompleteMessage.Data2 = mPendingMessage[1];

            // Save Data3 only if applicable
            if (mPendingMessageExpectedLength == 3)
                mCompleteMessage.Data3 = mPendingMessage[2];
            else
                mCompleteMessage.Data3 = 0;

            // Reset local variables
            mPendingMessageIndex = 0;
            mPendingMessageExpectedLength = 0;
            mPendingMessageValid = true;

            // Activate running status (if enabled for the received type)
            switch (getTypeFromStatusByte(mPendingMessage[0]))
            {
                case NoteOff:
                case NoteOn:
                case AfterTouchPoly:
                case ControlChange:
                case ProgramChange:
                case AfterTouchChannel:
                case PitchBend:
                    // Running status enabled: store it from received message
                    mRunningStatus_RX = mPendingMessage[0];
                    break;

                default:
                    // No running status
                    mRunningStatus_RX = InvalidType;
                    break;
            }
            return;
        }
        else
        {
            // Not complete? Then update the index of the pending message.
            mPendingMessageIndex++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// MIDI Utility Functions
///////////////////////////////////////////////////////////////////////////////

MidiMessageType getStatus(MidiMessageType inType, uint8_t inChannel) 
{
    return ((uint8_t)inType | ((inChannel - 1) & 0x0f));
}

MidiMessageType getTypeFromStatusByte(uint8_t inStatus)
{
    if ((inStatus  < 0x80) ||
        (inStatus == 0xf4) ||
        (inStatus == 0xf5) ||
        (inStatus == 0xf9) ||
        (inStatus == 0xfD))
    {
        // Data bytes and undefined.
        return InvalidType;
    }

    if (inStatus < 0xf0)
    {
        // Channel message, remove channel nibble.
        return (inStatus & 0xf0);
    }

    return inStatus;
}

uint8_t getChannelFromStatusByte(uint8_t inStatus)
{
	return (inStatus & 0x0f) + 1;
}

bool isChannelMessage(uint8_t inType)
{
    return (inType == NoteOff           ||
            inType == NoteOn            ||
            inType == ControlChange     ||
            inType == AfterTouchPoly    ||
            inType == AfterTouchChannel ||
            inType == PitchBend         ||
            inType == ProgramChange);
}
