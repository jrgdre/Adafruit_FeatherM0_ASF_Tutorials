/**	\file	main.c

 	\brief	Button tutorial application for Adafruit FeatherM0 (Atmel ATSAMD21G18A) using ASF

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
 		1.0.0: 2017-07-04 jrgdre initial release

 */

/**
* \mainpage FeatherM0 ASF button application documentation
*
* \par Button tutorial application for Adafruit FeatherM0 (Atmel SAMD21G18A) using ASF
*
* \par Content
*
* -# Use ASF Wizard with a custom board template to import the modules:
* - Generic board support (driver)
* - Delay routines (service)
* - PORT - GPIO Pin Control (driver)
* - SERCOM USART - Serial Communications (driver) callback
* - SYSTEM - Core System Driver (driver)
* -# Include the ASF header files (asf.h)
* -# Include the FatherM0 LED declarations (Adafruit_FeatherM0_LED.h)
* -# Include the FatherM0 RS232 declarations (Adafruit_FeatherM0_RS232.h)
* -# Add a LINK to Adafruit_FeatherM0_LED.c on project level
* -# Add a LINK to Adafruit_FeatherM0_RS232.c on project level
*/

#include <asf.h>						// Atmel Software Foundation
#include "Adafruit_FeatherM0_LED.h"		// FeatherM0 LED declarations
#include "Adafruit_FeatherM0_RS232.h"	// FeatherM0 RS232 declarations

#define STRING_EOL    "\r\n"
#define STRING_HEADER STRING_EOL \
"-- DIT Adafruit FeatherM0 ASF Turorials - 03_Button --"STRING_EOL \
"-- Adafruit FeatherM0 --"STRING_EOL	\
"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

#define BUTTON_PIN		PIN_PA15A_EIC_EXTINT15	//< MCU pin the button is connected to
#define BUTTON_PIN_MUX	MUX_PA15A_EIC_EXTINT15	//< pin multiplex setting to have the pad connected to the pin
#define BUTTON_EIC_LINE	15						//< external interrupt channel line

/**
 *	\brief	Button state changed call-back function
 */
static void button_on_detect( void ){
	// button input is active low, so...
	if( port_pin_get_input_level( BUTTON_PIN ) == 0 ){
		printf("button pressed"STRING_EOL);
	} else {
		printf("button released"STRING_EOL);
	}
}

/** 
 *	\brief	Configure button (digital) I/O on BUTTON_PIN
 */
static void button_init( void ){
	struct  extint_chan_conf config_extint_chan;	//< an instance of the external interrupt channel configuration data structure

	extint_chan_get_config_defaults( &config_extint_chan );			// load the default configuration
	config_extint_chan.gpio_pin			   = BUTTON_PIN;			// configure pin to use
	config_extint_chan.gpio_pin_mux		   = BUTTON_PIN_MUX;		// configure pad/pin connection
	config_extint_chan.gpio_pin_pull	   = EXTINT_PULL_UP;		// enable the internal pull-up resistor
	config_extint_chan.detection_criteria  = EXTINT_DETECT_BOTH;	// enable interrupts for button push and button release
	config_extint_chan.filter_input_signal = true;					// enable best of 3 measurements filter to filter out button bounces

	extint_chan_set_config( BUTTON_EIC_LINE, &config_extint_chan);	// set the new configuration
}

/************************************************************************/
/*	program entry point                                                 */
/************************************************************************/
int main (void)
{
	struct usart_module	usart_instance ;			//< an instance of the ASF SERCOM/USART module
	             int8_t ret = STATUS_OK;			//< a return value
	
	// ============================================
	// board, driver and service initialization [1]
	// ============================================
	
	system_init();									// board initialization
	system_interrupt_enable_global();				// enable global interrupt system for USART callbacks to work
	delay_init();									// initialize the delay service
	
	// =============================================
	// DIT Adafruit_FeatherM0 Library initialization
	// =============================================

	/* configure the LED */
	LED_configure  ( LED_PIN );						// LED pin configuration
	
	/* configure the debug message terminal */
	RS232_configure( &usart_instance, 115200 );		// configure USART
	RS232_enable   ( &usart_instance);				// enable USART and stdio_serial (if included)
	
	printf(STRING_HEADER);							// print welcome message

	// ============================================
	// board, driver and service initialization [2]
	// ============================================
	
	/* configure button (digital) I/O for call-back mode */
	button_init();									// initialize the button pin
	
	do {
		ret = extint_register_callback( &button_on_detect, BUTTON_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT );
	}
	while( ret != STATUS_OK );
	
	do {
		ret = extint_chan_enable_callback( BUTTON_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT );
	} 
	while( ret != STATUS_OK );
	
	// =================
	// application logic
	// =================

	while( true ) {
		// we let the LED blink to show that the MCU is ready 
		// and stays responsive, even if the button is pressed or released
		port_pin_set_output_level( LED_PIN, LED_ON  );
		delay_ms( 500 );
		port_pin_set_output_level( LED_PIN, LED_OFF );
		delay_ms( 500 );
		// everything else is handled by interrupt handlers and call-back functions
	}
}
