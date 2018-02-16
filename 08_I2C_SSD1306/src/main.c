/**	\file	main.c

 	\brief	Adafruit FeatherM0 SSD1306 OLED tutorial application implementation
 
 	\mainpage	Adafruit FeatherM0 OLED tutorial application

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
 		1.0.0: 2017-12-25 jrgdre initial release

 */

#include <asf.h>						// Atmel Software Foundation

#include "Com_Driver.h"					// generic bus communication driver
#include "Com_Driver_i2c_master.h"		// i2c bus communication driver
#include "SSD1306.h"					// SSD1306 display controller driver

#include "Draw.h"						// different graphic functions, like line(), etc.
#include "Font.h"						// general font related declarations
#include "Font_06px.h"					// our tiny 6px character font
#include "Font_08px.h"					// our 8px character font
#include "Framebuffer_1bit.h"			// 1bit Framebuffer

#define STRING_EOL    "\r\n"
#define STRING_HEADER STRING_EOL \
"-- DIT Adafruit FeatherM0 ASF Turorials - 08_SSD1306_I2C --"STRING_EOL \
"-- Adafruit FeatherM0 --"STRING_EOL	\
"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

/************************************************************************/
/* unit global variables                                                */
/*																		*/
/* these variables are global for this unit (static),					*/
/* so they can be accessed by the callback functions					*/
/************************************************************************/
static struct i2c_master_module i2c_master_instance; // an instance of the ASF SERCOM/I2C module
static struct       Framebuffer framebuffer		   ; // a framebuffer for the featherWing_OLED
static                     bool display_is_on      ; // state of the OLED display

/************************************************************************/
/*	serial interface configuration and call backs                       */
/************************************************************************/

static void i2c_master_configure( 
	struct i2c_master_module *const module   //< SERCOM module to use for transfer
){
	uint8_t ret = STATUS_OK;
	
	/* Initialize config structure and software module. */
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults( &config_i2c_master );
	
	config_i2c_master.pinmux_pad0 /* SDA */ = PINMUX_PA22C_SERCOM3_PAD0;
	config_i2c_master.pinmux_pad1 /* SCL */ = PINMUX_PA23C_SERCOM3_PAD1;
		
	config_i2c_master.buffer_timeout = 10000; // change buffer timeout to something longer
	
	printf( "i2c_configure_master: i2c_master_init"STRING_EOL );
	ret = i2c_master_init( module, SERCOM3, &config_i2c_master );
	while( ret != STATUS_OK ) {
		printf( "i2c_configure_master: i2c_master_init error %d"STRING_EOL, ret );
		ret = i2c_master_init( module, SERCOM3, &config_i2c_master );
	}

}

/************************************************************************/
/*	some constants for drawing things on the OLED                       */
/************************************************************************/
// #define X_MIN 0
// #define X_MAX (ADAFRUIT_FEATHERWING_OLED_WIDTH - 1)
// #define X_MID (X_MAX >> 1)
// 
// #define Y_MIN 0
// #define Y_MAX (ADAFRUIT_FEATHERWING_OLED_HEIGHT - 1)
// #define Y_MID (Y_MAX >> 1)

typedef enum {
	BLACK = 0x00,
	WHITE = 0x01
} pixel_values;

typedef enum {
	FONT_06PX = 0x00,
	FONT_08PX = 0x01	
} resetscreen_content;

/************************************************************************/
/*	FeatherWing_OLED button event handlers                              */
/************************************************************************/

static void on_reset( resetscreen_content rsc ) {
	uint8_t string_width;
	uint8_t x_pos = 0;

	switch( rsc ){
		case FONT_08PX: {
			font_08px_draw_string( &framebuffer_draw_pixel, &framebuffer, "Hello World!", WHITE, x_pos, 1, FONT_BACKGROUND_OPAQUE, &string_width);
			font_06px_draw_string( &framebuffer_draw_pixel, &framebuffer, "-73db 192.168.1.1 Drechsler-EXT", WHITE, x_pos,  25, FONT_BACKGROUND_OPAQUE, &string_width);
			break;
		}
		default: {
			font_06px_draw_string( &framebuffer_draw_pixel, &framebuffer, "Hello World!", WHITE, x_pos, 1, FONT_BACKGROUND_OPAQUE, &string_width);
			font_06px_draw_string( &framebuffer_draw_pixel, &framebuffer, "abcdefghijklmnopqrstuvwxyz", WHITE, x_pos,   7, FONT_BACKGROUND_OPAQUE, &string_width);
			font_06px_draw_string( &framebuffer_draw_pixel, &framebuffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", WHITE, x_pos,  13, FONT_BACKGROUND_OPAQUE, &string_width);
			font_06px_draw_string( &framebuffer_draw_pixel, &framebuffer, "abcdefghijklmnopqrstuvwxyz", WHITE, x_pos,  19, FONT_BACKGROUND_OPAQUE, &string_width);
			font_06px_draw_string( &framebuffer_draw_pixel, &framebuffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", WHITE, x_pos,  25, FONT_BACKGROUND_OPAQUE, &string_width);
			break;
		}
	}
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
	
	// =============================================
	// DIT Adafruit_FeatherM0 Library initialization
	// =============================================

	/* configure the debug message terminal */
	RS232_configure( &usart_instance, 115200 );     // configure USART
 	RS232_enable   ( &usart_instance );				// enable USART and stdio_serial (if included)

	printf(STRING_HEADER);

	// ============================================
	// board, driver and service initialization [2]
	// ============================================

	/* configure an i2c hardware device and enable it */
	i2c_master_configure         ( &i2c_master_instance );				// configure the MCU hardware used
	i2c_master_enable            ( &i2c_master_instance );				// enable the MCU hardware
	
	struct Com_Driver com_driver;										// create a new generic com driver instance
	
	com_driver_i2c_master_init( &com_driver, &i2c_master_instance );	// link the generic com driver instance to a specific i2c bus master instance
	
	/* define the OLED controller specifics */
	struct SSD1306 ssd1306 = {
		.com_driver			= com_driver								// the bus the SSD1306 is connected to
	,	.address			= 0x3C										// SSD1306's address on this bus
	,	.geometry			= SSD1306_GEOMETRY_128x64					// this SSD1306 drives a 128 columns by 32 rows display
	,	.charge_pump_source = SSD1306_CHARGE_PUMP_SOURCE_INTERNAL_VCC	// the charge pump source used by this SSD1306
	};
	
	ret = ssd1306_init( ssd1306 );
 	if( ret != STATUS_OK ) {
	 	printf( "main: initializing OLED error %d"STRING_EOL, ret );
	 	while( true ){ // stop processing
	 	}
 	}
 	display_is_on = false;
	
	framebuffer_init( &framebuffer, 128, ssd1306.geometry );
	
	// =================
	// application logic
	// =================

	framebuffer_clear     ( &framebuffer          );	// reset all pixels in the framebuffer to 0x00
	ssd1306_display_update( ssd1306, &framebuffer );    // send the framebuffer over to the screen

	ssd1306_display_on    ( ssd1306, true         );
	display_is_on = true;

 	on_reset( FONT_08PX );								// write our initial screen content to the framebuffer
	ssd1306_display_update( ssd1306, &framebuffer );	// send the framebuffer over to the screen
	
	while ( true ) {
			/* nothing to do */
	}
}
