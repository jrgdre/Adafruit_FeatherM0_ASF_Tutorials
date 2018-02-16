/**	\file	main.c

 	\brief	Adafruit FeatherWing OLED tutorial application implementation
 
 	\mainpage	Adafruit FeatherWing OLED tutorial application

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
 		1.0.0: 2017-06-22 jrgdre initial release

 */

#include <asf.h>						// Atmel Software Foundation
#include "Adafruit_FeatherM0_RS232.h"	// FeatherM0 RS232 declarations
#include "Adafruit_FeatherWing_OLED.h"	// FeatherWing_OLED declarations
#include "Draw.h"						// different graphic functions, like line(), etc.
#include "Draw_Fill_TestPattern.h"		// test pattern for the fill operation
#include "Draw_Line_TestPattern.h"		// test pattern using the draw_line() function
#include "Font.h"						// general font related declarations
#include "Font_06px.h"					// our tiny 6px character font
#include "Font_08px.h"					// our 8px character font
#include "Framebuffer_1bit.h"			// 1bit Framebuffer declarations
#include "SSD1306.h"					// SSD1306 display controller declarations
// #include "SSD1306_i2c.h"				// SSD1306 display controller i2c declarations

#include "Com_Driver.h"
#include "Com_Driver_i2c_master.h"

#define STRING_EOL    "\r\n"
#define STRING_HEADER STRING_EOL \
"-- DIT Adafruit FeatherM0 ASF Turorials - 07_I2C_SSD1306_FeatherWingOLED --"STRING_EOL \
"-- Adafruit FeatherM0 --"STRING_EOL	\
"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

/************************************************************************/
/* unit global variables                                                */
/*																		*/
/* these variables are global for this unit (static),					*/
/* so they can be accessed by the callback functions					*/
/************************************************************************/
static                     bool		display_is_on      ; // state of the OLED display
static struct       Framebuffer		framebuffer		   ; // a framebuffer for the featherWing_OLED
static struct i2c_master_module		i2c_master_instance; // an instance of the ASF SERCOM/I2C module

/************************************************************************/
/*	serial interface configuration and call backs                       */
/************************************************************************/

static void i2c_master_configure( 
	struct i2c_master_module *const module   //< SERCOM module to use for transfer
){
	enum status_code status = STATUS_OK;
	
	/* Initialize config structure and software module. */
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults( &config_i2c_master );
	
	config_i2c_master.pinmux_pad0 /* SDA */ = PINMUX_PA22C_SERCOM3_PAD0;
	config_i2c_master.pinmux_pad1 /* SCL */ = PINMUX_PA23C_SERCOM3_PAD1;
		
	config_i2c_master.buffer_timeout = 10000; // change buffer timeout to something longer
	
	printf( "i2c_configure_master: i2c_master_init"STRING_EOL );
	status = i2c_master_init( module, SERCOM3, &config_i2c_master );
	while( status != STATUS_OK ) {
		printf( "i2c_configure_master: i2c_master_init error %d"STRING_EOL, status );
		status = i2c_master_init( module, SERCOM3, &config_i2c_master );
	}

}

/************************************************************************/
/*	some constants for drawing things on the OLED                       */
/************************************************************************/
#define X_MIN 0
#define X_MAX (ADAFRUIT_FEATHERWING_OLED_WIDTH - 1)
#define X_MID (X_MAX >> 1)

#define Y_MIN 0
#define Y_MAX (ADAFRUIT_FEATHERWING_OLED_HEIGHT - 1)
#define Y_MID (Y_MAX >> 1)

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

static void button_A_on_pressed( void ) {
 	printf( "A pressed\n\r" );
 
 	display_is_on = !display_is_on;
 	
 	featherWing_OLED_on( display_is_on );
}

static void button_A_on_released( void ) {
	printf( "A released\n\r" );
}

static void button_B_on_pressed( void ) {
 	printf( "B pressed\n\r" );
 
 	draw_fill( &framebuffer_draw_pixel, &framebuffer_get_pixel, &framebuffer, BLACK, WHITE,       14,     7 ); // fill the test-pattern
 	draw_fill( &framebuffer_draw_pixel, &framebuffer_get_pixel, &framebuffer, BLACK, WHITE, X_MAX-15, Y_MAX ); // fill the test-pattern

	featherWing_OLED_update( &framebuffer );	// send it over to the screen
}

static void button_B_on_released( void ) {
 	printf( "B released\n\r" );
 
	framebuffer_clear( &framebuffer );			// reset all pixels to 0x00
 	
 	// draw the fill test-pattern
 	draw_fill_testpattern( &framebuffer_draw_pixel, &framebuffer, WHITE, X_MIN, Y_MIN );	
	 
 	// draw a circle
 	draw_circle			 ( &framebuffer_draw_pixel, &framebuffer, WHITE, X_MAX - 9, Y_MAX - 9, 10 );					// some points are out of bounds
	 
 	// draw an ellipse
 	draw_ellipse_rect	 ( &framebuffer_draw_pixel, &framebuffer, WHITE, X_MID, Y_MIN, X_MAX + X_MAX, Y_MAX + Y_MAX );  // some points are out of bounds
 
 	featherWing_OLED_update( &framebuffer );	// send it over to the screen
}

static void button_C_on_pressed( void ) {
 	printf( "C pressed\n\r" );
 
 	framebuffer_clear( &framebuffer );			// reset all pixels to 0x00
 
 	draw_rect			 ( &framebuffer_draw_pixel, &framebuffer, WHITE, X_MIN, Y_MIN, X_MAX, Y_MAX );
 	draw_line_testpattern( &framebuffer_draw_pixel, &framebuffer, WHITE, X_MID, Y_MID, X_MIN+2, Y_MIN+2, X_MAX-2, Y_MAX-2 );
 
 	featherWing_OLED_update( &framebuffer );	// send it over to the screen
}

static void button_C_on_released( void ) {
	printf( "C released\n\r" );
}

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
	struct      usart_module	usart_instance     ; //< an instance of the ASF SERCOM/USART module
	enum         status_code	status = STATUS_OK ; //< a return value
	
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
	i2c_master_configure( &i2c_master_instance );	// configure the MCU hardware used
	i2c_master_enable   ( &i2c_master_instance );	// enable the MCU hardware
	
	// ====================================================
	// DIT Adafruit_FeatherWing_OLED Library initialization
	// ====================================================

	/* initialize the OLED display (initial state is OFF) */
	status = featherWing_OLED_init( &i2c_master_instance );
 	if( status != STATUS_OK ) {
		printf( "main: initializing OLED error %d"STRING_EOL, status );
		while( true ){ // stop processing
		}
	}
	display_is_on = false;

	/* initialize the Wing_OLED buttons A, B and C */
	status = featherWing_OLED_init_buttons();
	if( status != STATUS_OK ) {
		printf( "main: initializing OLED buttons error %d"STRING_EOL, status );
		while( true ){ // stop processing
		}
	}
	featherWing_OLED_buttons_event_handlers.Button_A_On_Pressed  = &button_A_on_pressed ;
	featherWing_OLED_buttons_event_handlers.Button_A_On_Released = &button_A_on_released;
	featherWing_OLED_buttons_event_handlers.Button_B_On_Pressed  = &button_B_on_pressed ;
	featherWing_OLED_buttons_event_handlers.Button_B_On_Released = &button_B_on_released;
	featherWing_OLED_buttons_event_handlers.Button_C_On_Pressed  = &button_C_on_pressed ;
	featherWing_OLED_buttons_event_handlers.Button_C_On_Released = &button_C_on_released;

	// ==========================
	// Framebuffer initialization
	// ==========================	
	
	framebuffer_init( &framebuffer, ADAFRUIT_FEATHERWING_OLED_WIDTH, ADAFRUIT_FEATHERWING_OLED_HEIGHT );
	
	// =================
	// application logic
	// =================

	framebuffer_clear( &framebuffer );					// reset all pixels in the framebuffer to 0x00
	
	status = featherWing_OLED_update( &framebuffer );	// send the framebuffer over to the screen

	featherWing_OLED_on( true );				// switch the OLED on
	display_is_on = true;						// and remember the state

 	on_reset( FONT_08PX );								// write our initial screen content to the framebuffer
	status = featherWing_OLED_update( &framebuffer );	// send the framebuffer over to the screen
	
	while ( true ) {
		/* nothing to do */
	}
}
