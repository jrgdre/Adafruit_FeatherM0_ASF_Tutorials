/**	\file	main.c

 	\brief	Analog Digital Converter (ADC) tutorial application for Adafruit FeatherM0 (Atmel ATSAMD21G18A) using ASF

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
 		1.0.0: 2017-07-09 jrgdre initial release

 */

/**
* \mainpage FeatherM0 ASF Analog Digital Converter (ADC) application documentation
*
* \par Analog Digital Converter (ADC) tutorial application for Adafruit FeatherM0 (Atmel SAMD21G18A) using ASF
*
* \par Content
*
* -# Use ASF Wizard with a custom board template to import the modules:
*    - Generic board support (driver)
*    - ADC - Analog-to-Digital-Converter (driver)
*    - PORT - GPIO Pin Control (driver)
*    - RTC - Real Timer Counter Driver (driver)
*    - SERCOM USART - Serial Communications (driver) callback
*    - SYSTEM - Core System Driver (driver)
*    - Standard serial I/O (stdio) (driver)
* -# Include the ASF header files (asf.h)
* -# Include the FatherM0 RS232 declarations (Adafruit_FeatherM0_RS232.h)
* -# Add a LINK to Adafruit_FeatherM0_RS232.c on project level
*/

#include <asf.h>						// Atmel Software Foundation
#include "Adafruit_FeatherM0_RS232.h"	// FeatherM0 RS232 declarations

#define STRING_EOL    "\r\n"
#define STRING_HEADER STRING_EOL \
STRING_EOL"-- DIT Adafruit FeatherM0 ASF Turorials - 05_ADC --" \
STRING_EOL"-- Adafruit FeatherM0 --" \
STRING_EOL"-- Compiled: "__DATE__ " "__TIME__ " --"

#define ADC_NUM_SAMPLES		1			//< number of samples to take for each ADC job
#define ADC_MAX_VALUE		(1<<12)		//< at 12 bit resolution (2^12) 
#define BAT_MAX_VOLTAGE     4.2			//< ~ maximal battery voltage at full charge
#define BAT_MIN_VOLTAGE     3.2			//< ~ minimal battery voltage before the protection unit shuts down the FeatherM0
#define VCC					3.3			//< supply voltage

#define ADC_USE_EXT_REF_A				// comment out to us internal reference

#ifdef ADC_USE_EXT_REF_A
	// use external reference A
	#define ADC_REFERENCE	ADC_REFERENCE_AREFA
	#define ADC_REF_VALUE	2.1			//< voltage @ input = input_max (2.1V = 1/2 BAT_MAX_VOLTAGE)
#else
	// use internal reference
	#define ADC_REFERENCE	ADC_REFERENCE_INTVCC0
	#define ADC_REF_VALUE	VCC/1.48	//< voltage @ input = input_max 
#endif

/* We make these variables global, so the callbacks can access them */
uint16_t adc_buffer[ADC_NUM_SAMPLES];	//< a buffer for ADC results
  struct adc_module adc_instance;		//< an instance of an ADC module
  struct rtc_module rtc_instance;		//< an instance of an RTC module

/**
 *	\brief	Function called when the ADC has finished it's conversion
 */
static void adc_complete_callback( struct adc_module *const module )
{
 	uint16_t  input = adc_buffer[0] << 1;							// *2, because of 100/100k voltage divider on battery
	float    result = ADC_REF_VALUE * (float)input / (float)ADC_MAX_VALUE; 
	uint8_t percent = (uint8_t)( (result - BAT_MIN_VOLTAGE) / (BAT_MAX_VOLTAGE - BAT_MIN_VOLTAGE) * 100 );

	// we have not floating point printf in the imported libraries,
	// so we split the float into two integers
 	uint8_t a, b;
	a = (uint8_t)result;
	b = (uint8_t)((result-a)*100);
	printf( "\r%d.%02dV (%d%%)", a, b, percent );
}

/** 
 *	\brief	Configure the ADC
 */
static void adc_configure( void )
{
	struct adc_config config_adc;

	adc_get_config_defaults( &config_adc );
	config_adc.reference		  = ADC_REFERENCE;	
	config_adc.positive_input	  = ADC_POSITIVE_INPUT_PIN7;
	config_adc.accumulate_samples = ADC_AVGCTRL_SAMPLENUM_8;
	config_adc.divide_result	  = ADC_DIVIDE_RESULT_8;	
	
	adc_init( &adc_instance, ADC, &config_adc );
}

/**
 *	\brief	Function called when the end value for the period is reached
 */
static void rtc_callback_overflow( void ){
	uint8_t ret;

	do {
		ret = adc_read_buffer_job( &adc_instance, adc_buffer, ADC_NUM_SAMPLES);
	} while( ret != STATUS_OK );

	rtc_count_clear_overflow( &rtc_instance );
}

/** 
 *	\brief	Configure the RTC counter
 */
static void rtc_count_configure( void ){
	struct rtc_count_config	config_rtc_count;	//< an instance of the rtc_count configuration data structure
		             int8_t	ret = STATUS_OK;	//< a return value

	rtc_count_get_config_defaults(&config_rtc_count);
	config_rtc_count.prescaler			 = RTC_COUNT_PRESCALER_DIV_1024;
	config_rtc_count.mode				 = RTC_COUNT_MODE_16BIT;
	config_rtc_count.continuously_update = true;	
	rtc_count_init( &rtc_instance, RTC, &config_rtc_count );

	/* configure and enable the callback for the end of the period*/
	do {
		ret = rtc_count_register_callback( &rtc_instance, &rtc_callback_overflow, RTC_COUNT_CALLBACK_OVERFLOW );
	}
	while( ret != STATUS_OK );
	rtc_count_enable_callback(  &rtc_instance, RTC_COUNT_CALLBACK_OVERFLOW  );

	/* period needs to be set after configuration */	
	rtc_count_set_period( &rtc_instance, 1 ); // s
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

	/* configure the debug message terminal */
	RS232_configure( &usart_instance, 115200 );		// configure USART
	RS232_enable   ( &usart_instance);				// enable USART and stdio_serial (if included)
	
	printf(STRING_HEADER);							// print welcome message
	printf(STRING_EOL   );							// 

	// ============================================
	// board, driver and service initialization [2]
	// ============================================
	
	adc_configure( );
	adc_enable   ( &adc_instance );

	adc_register_callback( &adc_instance, adc_complete_callback, ADC_CALLBACK_READ_BUFFER );
	adc_enable_callback  ( &adc_instance, ADC_CALLBACK_READ_BUFFER );

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
