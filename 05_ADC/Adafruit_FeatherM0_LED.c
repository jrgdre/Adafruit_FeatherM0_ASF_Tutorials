/**	\file	Adafruit_FeatherM0_LED.c

 	\brief	DIT Adafruit FeatherM0 LED library implementation
 
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

#include <asf.h>
#include "Adafruit_FeatherM0_LED.h"

/**
 * Configures an LED pin as output.
 *
 * \param pin Id of pin LED is connected to
 */
void LED_configure( const uint8_t pin )
{
	struct port_config config_port_pin;                // pin configuration data structure

	port_get_config_defaults( &config_port_pin );      // initialize data structure with default value
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;   // set pin data direction to output
	port_pin_set_config( pin, &config_port_pin );      // set the new pin configuration for the port
}