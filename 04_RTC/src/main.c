/**	\file	main.c

 	\brief	Real Time Clock (RTC) tutorial application for Adafruit FeatherM0 (Atmel ATSAMD21G18A) using ASF

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
 		1.0.0: 2017-07-05 jrgdre initial release

 */

/**
* \mainpage FeatherM0 ASF Real Time Clock (RTC) application documentation
*
* \par Button tutorial application for Adafruit FeatherM0 (Atmel SAMD21G18A) using ASF
*
* \par Content
*
* -# Use ASF Wizard with a custom board template to import the modules:
* - Generic board support (driver)
* - PORT - GPIO Pin Control (driver)
* - RRTC - Real Timer Countter Driver (driver)
* - SERCOM USART - Serial Communications (driver) callback
* - SYSTEM - Core System Driver (driver)
* - Standard serial I/O (stdio) (driver)
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
"-- DIT Adafruit FeatherM0 ASF Turorials - 04_RTC --"STRING_EOL \
"-- Adafruit FeatherM0 --"STRING_EOL	\
"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

/* needs to be global, so the callbacks can access it */
struct rtc_module rtc_instance;	//< an instance of an RTC module

/**
 *	\brief	Function called when the compare value 0 is reached
 */
static void rtc_callback_0( void ){
	
	port_pin_toggle_output_level( LED_PIN );	// LED will be toggled every three seconds
	
	printf( "compare[0]"STRING_EOL );
	rtc_count_clear_compare_match( &rtc_instance, RTC_COUNT_COMPARE_0 );
}

/**
 *	\brief	Function called when the compare value 1 is reached
 */
static void rtc_callback_1( void ){
	printf( "compare[1]"STRING_EOL );
	rtc_count_clear_compare_match( &rtc_instance, RTC_COUNT_COMPARE_1 );
}

/**
 *	\brief	Function called when the end value for the period is reached
 */
static void rtc_callback_overflow( void ){
	printf( "overflow"STRING_EOL );
	rtc_count_clear_overflow( &rtc_instance );
}

/** 
 *	\brief	Configure the RTC counter
 */
static void rtc_count_configure( void ){
	struct rtc_count_config	config_rtc_count;	//< an instance of the rtc_count configuration data structure
		             int8_t	ret = STATUS_OK;	//< a return value

	rtc_count_get_config_defaults(&config_rtc_count);
	config_rtc_count.prescaler			 = RTC_COUNT_PRESCALER_DIV_1;
	config_rtc_count.mode				 = RTC_COUNT_MODE_16BIT;
	config_rtc_count.continuously_update = true;	
	config_rtc_count.compare_values[0]	 = 1000;	// call first  callback after 1sec in period
	config_rtc_count.compare_values[1]	 = 2000;	// call second callback after 2sec in period
	rtc_count_init( &rtc_instance, RTC, &config_rtc_count );

	/* configure and enable the callback for the first compare value */	 
	do {
		ret = rtc_count_register_callback( &rtc_instance, &rtc_callback_0, RTC_COUNT_CALLBACK_COMPARE_0 );
	}
	while( ret != STATUS_OK );
	rtc_count_enable_callback(  &rtc_instance, RTC_COUNT_CALLBACK_COMPARE_0  );
	
	/* configure and enable the callback for the second compare value */
	do {
		ret = rtc_count_register_callback( &rtc_instance, &rtc_callback_1, RTC_COUNT_CALLBACK_COMPARE_1 );
	}
	while( ret != STATUS_OK );
	rtc_count_enable_callback(  &rtc_instance, RTC_COUNT_CALLBACK_COMPARE_1  );
	
	/* configure and enable the callback for the end of the period*/
	do {
		ret = rtc_count_register_callback( &rtc_instance, &rtc_callback_overflow, RTC_COUNT_CALLBACK_OVERFLOW );
	}
	while( ret != STATUS_OK );
	rtc_count_enable_callback(  &rtc_instance, RTC_COUNT_CALLBACK_OVERFLOW  );

	/* period needs to be set after configuration */	
	rtc_count_set_period( &rtc_instance, 3000 ); // three seconds
}

/************************************************************************/
/*	program entry point                                                 */
/************************************************************************/
int main (void)
{
	struct usart_module	usart_instance ;			//< an instance of the ASF SERCOM/USART module
	
	// ============================================
	// board, driver and service initialization [1]
	// ============================================
	
	system_init();									// board initialization
	system_interrupt_enable_global();				// enable global interrupt system for USART callbacks to work
	
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
	
	/* configure real time clock counter for call-back mode */
	rtc_count_configure( );							// initialize the real time clock counter
	rtc_count_enable   ( &rtc_instance );
	
	// =================
	// application logic
	// =================
 
	while( true ) {
		/* noting to do here */
	}
}
