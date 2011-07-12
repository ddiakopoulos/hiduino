/** \file
 */
 
#ifndef _CDC_DESCRIPTORS_H_
#define _CDC_DESCRIPTORS_H_

	/* Includes: */

		#include <avr/pgmspace.h>
		#include <LUFA/Drivers/USB/Class/CDC.h>
		#include <LUFA/Drivers/USB/USB.h>
		
	/* Macros: */ 
		
		/** Endpoint number of the CDC device-to-host notification IN endpoint. */
		#define CDC_NOTIFICATION_EPNUM         1
		
		/** Size in bytes of the CDC device-to-host notification IN endpoint. */
		#define CDC_NOTIFICATION_EPSIZE        8

		/** Endpoint number of the CDC device-to-host data IN endpoint. */
		#define CDC_TX_EPNUM                   2	

		/** Endpoint number of the CDC host-to-device data OUT endpoint. */
		#define CDC_RX_EPNUM                   3	

		/** Size in bytes of the CDC data IN and OUT endpoints. */
		#define CDC_TXRX_EPSIZE                64	

	/* Type Defines: */
	
		typedef struct {
		
			USB_Descriptor_Configuration_Header_t    Config;
			USB_Descriptor_Interface_t               CDC_CCI_Interface;
			CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC_Functional_IntHeader;
			CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC_Functional_CallManagement;
			CDC_FUNCTIONAL_DESCRIPTOR(1)             CDC_Functional_AbstractControlManagement;
			CDC_FUNCTIONAL_DESCRIPTOR(2)             CDC_Functional_Union;
			USB_Descriptor_Endpoint_t                CDC_NotificationEndpoint;
			USB_Descriptor_Interface_t               CDC_DCI_Interface;
			USB_Descriptor_Endpoint_t                CDC_DataOutEndpoint;
			USB_Descriptor_Endpoint_t                CDC_DataInEndpoint;
	
		} Serial_Descriptor_Configuration_t;

	/* Function Prototypes: */
		uint16_t Serial_GetDescriptor(const uint16_t wValue,
		                                    const uint8_t wIndex,
		                                    const void** const DescriptorAddress)
		                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif

