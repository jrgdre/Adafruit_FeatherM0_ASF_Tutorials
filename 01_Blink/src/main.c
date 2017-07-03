/**	\file	main.c

 	\brief	 Blink tutorial application for Adafruit FeatherM0 (Atmel SAMD21G18A) using ASF

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
 		1.0.0: 2017-06-01 jrgdre initial release

 */

/**
 * \mainpage FeatherM0 ASF Blink application documentation
 *
 * \par Blink application for Adafruit FeatherM0 (Atmel SAMD21G18A) using ASF
 *
 * \par Content
 *
 * -# Use ASF Wizard with a custom board template to import the modules:
 *   - Generic board support (driver)
 *   - Delay routines (service)
 *   - PORT - GPIO Pin Control (driver)
 *   - SYSTEM - Core System Driver (driver)
 * -# Include the ASF header files (asf.h)
 * -# Include the FatherM0 LED declarations (Adafruit_FeatherM0_LED.h)
 * -# Add a LINK to Adafruit_FeatherM0_LED.c on project level
 */

#include <asf.h>                     // Include header files for all drivers that have been imported from Atmel Software Framework (ASF).
#include "Adafruit_FeatherM0_LED.h"  // Include the FatherM0 LED declarations

int main (void)
{
	/* Board, driver and service initialization */
	system_init();
	system_clock_init();
	delay_init();

	LED_configure( LED_PIN );        // declared in Adafruit_FeatherM0_LED.h, implemented in Adafruit_FeatherM0_LED.c
	
	/* Application code */
	while( true ) {
		port_pin_set_output_level( LED_PIN, LED_ON  );
		delay_ms( 500 );
		port_pin_set_output_level( LED_PIN, LED_OFF );
		delay_ms( 500 );
	}
}
