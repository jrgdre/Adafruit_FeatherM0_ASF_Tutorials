/**	\file	Adafruit_FeatherM0_RS232.c

 	\brief	DIT Adafruit FeatherM0 RS232 library implementation
 
 	\license 
 		MIT:	The MIT License (https://opensource.org/licenses/MIT)
 		.
 		Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 		and associated documentation files (the "Software"), to deal in the Software without restriction, 
 		including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 		and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
 		subject to the following conditions:
 		.
 		The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 		.
 		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 		INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 		IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 		WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
 		THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 	\copyright
 		DIT: 2017; Drechsler Information Technologies; www.drechsler-it.de
 
 	\authors
 		jrgdre: Joerg Drechsler; DIT
 
 	\versions
		2.0.0: 2017-07-03 jrgdre Make the usage of callbacks an option
 		1.0.0: 2017-06-22 jrgdre initial release

 */

#include <asf.h>
#include "Adafruit_FeatherM0_RS232.h"

/**
 * Configures an ASF USART instance for
 * - SERCOM 0 
 * - TxD on PA10 (on a FeatherM0 connected to JP1.2)
 * - RxD on PA11 (on a FeatherM0 connected to JP1.3)
 */
void RS232_configure(
  struct usart_module *const usart_instance, 
  const uint32_t             baudrate 
){
	struct usart_config config_usart;                     // USART configuration data structure
	
	usart_get_config_defaults( &config_usart );           // initialize with default values
	
	config_usart.baudrate    = baudrate;                  // set baudrate desired
	config_usart.mux_setting = USART_RX_3_TX_2_XCK_3;     // RxD on Pad_3, TxD on Pad_2 (usart.h)
	config_usart.pinmux_pad0 = PINMUX_UNUSED;
	config_usart.pinmux_pad1 = PINMUX_UNUSED;
	config_usart.pinmux_pad2 = PINMUX_PA10C_SERCOM0_PAD2; // connect SERCOM0_PAD2 (TxD) to PA10 (samd21g18a.h)
	config_usart.pinmux_pad3 = PINMUX_PA11C_SERCOM0_PAD3; // connect SERCOM0_PAD3 (RxD) to PA11 (samd21g18a.h)

	// try until our usart_module has been successfully initialized
	while( usart_init( usart_instance, SERCOM0, &config_usart ) != STATUS_OK ) {
	}

	#ifdef STDIO_SERIAL_H_INCLUDED
	  stdio_serial_init( usart_instance, SERCOM0, &config_usart );
	#endif
}

/**
 *	\brief	Register a callback for an event on an USART
 */
void RS232_configure_callback ( 
  struct usart_module *const usart_instance,	//< usart instance to register the callback for
     usart_callback_t  callback_func,			//< callback function to register
  enum usart_callback  callback_type			//< type of event to register the callback for
){
	if( callback_func ) { 
		usart_register_callback( usart_instance, callback_func, callback_type ); 
		usart_enable_callback  ( usart_instance, callback_type );
	} else {
		usart_disable_callback ( usart_instance, callback_type );
	}
}

/**
 *	\brief	Enable the USART
 */
void RS232_enable( 
  struct usart_module *const usart_instance
){
	usart_enable( usart_instance );
}
