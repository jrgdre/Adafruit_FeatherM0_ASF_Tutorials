/**	\file	main.c

 	\brief	RS232 tutorial application for Adafruit FeatherM0 (Atmel SAMD21G18A) using ASF

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
 		1.0.1: 2017-07-03 Use the updated version 2.0.0 of the RS232 library
 		1.0.0: 2017-06-22 jrgdre initial release

 */

/**
 * \mainpage FeatherM0 ASF RS232 application documentation
 *
 * \par RS232 application for Adafruit FeatherM0 (Atmel SAMD21G18A) using ASF
 *
 * \par Content
 *
 * -# Use ASF Wizard with a custom board template to import the modules:
 *   - Generic board support (driver)
 *   - Delay routines (service)
 *   - PORT - GPIO Pin Control (driver)
 *   - SERCOM USART - Serial Communications (driver) callback
 *   - SYSTEM - Core System Driver (driver)
 * -# Include the ASF header files (asf.h)
 * -# Include the FatherM0 LED declarations (Adafruit_FeatherM0_LED.h)
 * -# Include the FatherM0 RS232 declarations (Adafruit_FeatherM0_RS232.h)
 * -# Add a LINK to Adafruit_FeatherM0_LED.c on project level
 * -# Add a LINK to Adafruit_FeatherM0_RS232.c on project level
 */

#include <asf.h>                      // include header files for all drivers that have been imported from Atmel Software Framework (ASF).
#include "Adafruit_FeatherM0_LED.h"   // include the FatherM0 LED declarations
#include "Adafruit_FeatherM0_RS232.h" // include the FatherM0 RS232 declarations

#define MAX_RX_BUFFER_LENGTH  1        // size of receive buffer 

// rx_buffer is global,   so it can be accessed by the callback functions.
// rx_buffer is volatile, so it is not optimized by the compiler.
volatile uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH]; // a buffer for chars received

/**
 * USART received event handler
 *
 * Called by the USART module, whenever rx_buffer is filled.
 *
 * Writes the incoming chars back to the terminal that sent them.
 *
 * \param usart_instance The ASF USART instance that received a complete rx_buffer.
 */
static void usart_on_received( struct usart_module *const usart_instance )
{
	usart_write_buffer_job( usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH );
}

/**
 * USART transmitted event handler
 *
 * Called by the USART module, whenever a complete buffer was sent.
 *
 * Toggles the LED state for each buffer transmitted.
 *
 * \param usart_instance The ASF USART instance that finished transmitting a buffer.
 */
static void usart_on_transmitted( struct usart_module *const usart_instance )
{
	port_pin_toggle_output_level( LED_PIN );
}

/**
 * Application entry point
 *
 * The application 
 * - Initializes the system
 * - Writes a welcome message
 * - Reads incoming chars from the terminal into the rx_buffer
 * firing event handlers for received and transmitted buffers.
 */
int main( void )
{
	struct usart_module usart_instance; // an instance of the ASF SERCOM/USART module
	
	// ========================================
	// board, driver and service initialization
	// ========================================
	
	system_init();                      // board initialization
	system_interrupt_enable_global();   // enable global interrupt system for USART callbacks to work
	
	// =============================================
	// DIT Adafruit_FeatherM0 Library initialization
	// =============================================
	
	LED_configure  ( LED_PIN );                 // LED pin configuration
	
	RS232_configure         ( &usart_instance, 115200 ); // configure USART
	RS232_configure_callback( &usart_instance, &usart_on_received   , USART_CALLBACK_BUFFER_RECEIVED    ); // conf and enable received callback
	RS232_configure_callback( &usart_instance, &usart_on_transmitted, USART_CALLBACK_BUFFER_TRANSMITTED ); // conf and enable transmitted callback
	RS232_enable            ( &usart_instance ); // enable USART and stdio_serial (if included)

    // =================
	// application logic
    // =================

	uint8_t string[] = "\n\rHello World!";                               // welcome message
	usart_write_buffer_job( &usart_instance, string, sizeof( string ) ); // write welcome message
	
	while (true) {
		usart_read_buffer_job( &usart_instance, (uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH ); // read incoming characters
	}
}
