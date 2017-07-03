/**	\file	Adafruit_FeatherM0_RS232.h

 	\brief	DIT Adafruit FeatherM0 RS232 library declarations
 
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

#ifndef ADAFRUIT_FEATHERM0_RS232
#define ADAFRUIT_FEATHERM0_RS232

/**
 * Configures an ASF USART instance for
 * - SERCOM 0 
 * - TxD on PA10 (on a FeatherM0 connected to JP1.2)
 * - RxD on PA11 (on a FeatherM0 connected to JP1.3)
 */
void RS232_configure(
  struct usart_module *const usart_instance, 
  const uint32_t             baudrate 
);

/**
 *	Register a callback for an event on an USART
 *	and enable it.
 *
 *	\note	If callback_func == NULL, the callback will be disabled.
 */
void RS232_configure_callback ( 
  struct usart_module *const usart_instance,	//< USART instance to register the callback for
     usart_callback_t  callback_func,			//< callback function to register
  enum usart_callback  callback_type			//< type of event to register the callback for
);

/**
 *	\brief	Enable the USART
 * 
 * You need to configure RS232 before enabling it!
 * \see RS232_configure() 
 */
void RS232_enable( 
  struct usart_module *const usart_instance
);

#endif
