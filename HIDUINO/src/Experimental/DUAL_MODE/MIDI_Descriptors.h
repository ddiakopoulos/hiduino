/** \file
 */
 
#ifndef _MIDI_DESCRIPTORS_H_
#define _MIDI_DESCRIPTORS_H_

		/* Includes: */

		#include <avr/pgmspace.h>
		#include <LUFA/Drivers/USB/Class/CDC.h>
		#include <LUFA/Drivers/USB/USB.h>
		
	/* Macros: */
	
		/** Endpoint number of the MIDI streaming data OUT endpoint, for host-to-device data transfers. */
		#define MIDI_STREAM_OUT_EPNUM       1
		
		/** Endpoint number of the MIDI streaming data IN endpoint, for device-to-host data transfers. */
		#define MIDI_STREAM_IN_EPNUM        2

		/** Endpoint size in bytes of the Audio isochronous streaming data IN and OUT endpoints. */
		#define MIDI_STREAM_EPSIZE          64

	/* Type Defines: */
	
		typedef struct {
		
			USB_Descriptor_Configuration_Header_t     Config;
			USB_Descriptor_Interface_t                Audio_ControlInterface;
			USB_Audio_Descriptor_Interface_AC_t       Audio_ControlInterface_SPC;
			USB_Descriptor_Interface_t                Audio_StreamInterface;
			USB_MIDI_Descriptor_AudioInterface_AS_t   Audio_StreamInterface_SPC;
			USB_MIDI_Descriptor_InputJack_t           MIDI_In_Jack_Emb;
			USB_MIDI_Descriptor_InputJack_t           MIDI_In_Jack_Ext;
			USB_MIDI_Descriptor_OutputJack_t          MIDI_Out_Jack_Emb;
			USB_MIDI_Descriptor_OutputJack_t          MIDI_Out_Jack_Ext;
			USB_Audio_Descriptor_StreamEndpoint_Std_t MIDI_In_Jack_Endpoint;
			USB_MIDI_Descriptor_Jack_Endpoint_t       MIDI_In_Jack_Endpoint_SPC;
			USB_Audio_Descriptor_StreamEndpoint_Std_t MIDI_Out_Jack_Endpoint;
			USB_MIDI_Descriptor_Jack_Endpoint_t       MIDI_Out_Jack_Endpoint_SPC;
				
		} MIDI_Descriptor_Configuration_t;

	/* Function Prototypes: */
		uint16_t MIDI_GetDescriptor(const uint16_t wValue,
		                                    const uint8_t wIndex,
		                                    const void** const DescriptorAddress)
		                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif

