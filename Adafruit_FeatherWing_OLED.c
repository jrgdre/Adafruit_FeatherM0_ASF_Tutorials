/**	\file	Adafruit_FeatherWing_OLED.c

 	\brief	Adafruit FeatherWing OLED driver implementation
 
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
		1.1.0: 2017-12-23 jrgdre use ssd1306 library
 		1.0.0: 2017-06-22 jrgdre initial release

 */
#include "Adafruit_FeatherWing_OLED.h"
#include "Com_Driver_i2c_master.h"
#include "SSD1306.h"					// FeatherWing_OLED uses an SSD1306 OLED controller IC

/* FeatherWingOLED's SSD1306 default configuration */
static struct SSD1306 featherWing_OLED = {
	.com_driver         = NULL
, 	.address            = ADAFRUIT_FEATHERWING_OLED_I2C_ADDRESS
,	.geometry           = SSD1306_GEOMETRY_128x32
,   .charge_pump_source = SSD1306_CHARGE_PUMP_SOURCE_INTERNAL_VCC
,	.flip_horizontal    = true
,	.flip_vertical		= true
};

/************************************************************************/
/* local functions                                                      */
/************************************************************************/

static void button_A_on_detect( void ) {
	if( !port_pin_get_input_level( BUTTON_A_PIN ) ){
		if( featherWing_OLED_buttons_event_handlers.Button_A_On_Pressed != NULL ){
			featherWing_OLED_buttons_event_handlers.Button_A_On_Pressed();
		}
	} else {
		if( featherWing_OLED_buttons_event_handlers.Button_A_On_Released != NULL ){
			featherWing_OLED_buttons_event_handlers.Button_A_On_Released();
		}
	}
}

static void button_B_on_detect( void ) {
	if( !port_pin_get_input_level( BUTTON_B_PIN ) ){
		if( featherWing_OLED_buttons_event_handlers.Button_B_On_Pressed != NULL ){
			featherWing_OLED_buttons_event_handlers.Button_B_On_Pressed();
		}
	} else {
		if( featherWing_OLED_buttons_event_handlers.Button_B_On_Released != NULL ){
			featherWing_OLED_buttons_event_handlers.Button_B_On_Released();
		}
	}
}

static void button_C_on_detect( void ) {
	if( !port_pin_get_input_level( BUTTON_C_PIN ) ){
		if( featherWing_OLED_buttons_event_handlers.Button_C_On_Pressed != NULL ){
			featherWing_OLED_buttons_event_handlers.Button_C_On_Pressed();
		}
	} else {
		if( featherWing_OLED_buttons_event_handlers.Button_C_On_Released != NULL ){
			featherWing_OLED_buttons_event_handlers.Button_C_On_Released();
		}
	}
}

/************************************************************************/
/* public functions                                                     */
/************************************************************************/

/**
 *	\brief Initialize the FeatherWing_OLED display connected to the i2c SERCOM \ref module.
 */
enum status_code featherWing_OLED_init( 
	struct i2c_master_module *i2c_master_instance	//< I2C master this featherWing_OLED is connected to
){
	Assert(featherWing_OLED.com_driver != NULL);

	com_driver_i2c_master_init( &featherWing_OLED.com_driver, i2c_master_instance ); // link the generic com driver instance to the i2c bus master instance specified
	
	return ssd1306_init( featherWing_OLED ); // initialize the driver chip on the featherWing_OLED
}

/**
 *	\brief Initialize the FeatherWing_OLED buttons.
 */
enum status_code featherWing_OLED_init_buttons(
){
	  enum status_code			status;
	struct extint_chan_conf		config_extint_chan;

	// initialize buttons event handlers management structure
	featherWing_OLED_buttons_event_handlers.Button_A_On_Pressed  = NULL;
	featherWing_OLED_buttons_event_handlers.Button_A_On_Released = NULL;
	featherWing_OLED_buttons_event_handlers.Button_B_On_Pressed  = NULL;
	featherWing_OLED_buttons_event_handlers.Button_B_On_Released = NULL;
	featherWing_OLED_buttons_event_handlers.Button_C_On_Pressed  = NULL;
	featherWing_OLED_buttons_event_handlers.Button_C_On_Released = NULL;
	
	// Button A
	extint_chan_get_config_defaults(&config_extint_chan);

	config_extint_chan.gpio_pin			   = BUTTON_A_PIN;
	config_extint_chan.gpio_pin_mux		   = BUTTON_A_PIN_MUX;
	config_extint_chan.gpio_pin_pull	   = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria  = EXTINT_DETECT_BOTH;
	config_extint_chan.filter_input_signal = true;

	extint_chan_set_config( BUTTON_A_EIC_LINE, &config_extint_chan);
	
	status = extint_register_callback( &button_A_on_detect, BUTTON_A_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT );
	if( status != STATUS_OK ){
		return status;
	}
	
	status = extint_chan_enable_callback( BUTTON_A_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT );
	if( status != STATUS_OK ){
		return status;
	}
	
	// Button B
	extint_chan_get_config_defaults(&config_extint_chan);

	config_extint_chan.gpio_pin			   = BUTTON_B_PIN;
	config_extint_chan.gpio_pin_mux		   = BUTTON_B_PIN_MUX;
	config_extint_chan.gpio_pin_pull	   = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria  = EXTINT_DETECT_BOTH;
	config_extint_chan.filter_input_signal = false; // has a hardware pull-up resistor of 100k

	extint_chan_set_config( BUTTON_B_EIC_LINE, &config_extint_chan);
	
	status = extint_register_callback( &button_B_on_detect, BUTTON_B_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT );
	if( status != STATUS_OK ){
		return status;
	}
	
	status = extint_chan_enable_callback( BUTTON_B_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT );
	if( status != STATUS_OK ){
		return status;
	}
	
	// Button C
	extint_chan_get_config_defaults(&config_extint_chan);

	config_extint_chan.gpio_pin			   = BUTTON_C_PIN;
	config_extint_chan.gpio_pin_mux		   = BUTTON_C_PIN_MUX;
	config_extint_chan.gpio_pin_pull	   = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria  = EXTINT_DETECT_BOTH;
	config_extint_chan.filter_input_signal = true;

	extint_chan_set_config( BUTTON_C_EIC_LINE, &config_extint_chan);
	
	status = extint_register_callback( &button_C_on_detect, BUTTON_C_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT );
	if( status != STATUS_OK ){
		return status;
	}
	
	status = extint_chan_enable_callback( BUTTON_C_EIC_LINE, EXTINT_CALLBACK_TYPE_DETECT );
	if( status != STATUS_OK ){
		return status;
	}
	
	return STATUS_OK;
}

/**
 *	\brief Switch the state of the OLED display.
 */
enum status_code featherWing_OLED_on(
	bool  const value	//< if OLED display is to set to the ON state
){
	return ssd1306_display_on( featherWing_OLED, value );
}

/**
 *	\brief Update only the tiles that have been changed since the last OLED update
 */
enum status_code featherWing_OLED_update(
	framebuffer_t *const framebuffer	//< framebuffer to write to the FeatherWing_OLED
){
	return ssd1306_display_update( featherWing_OLED, framebuffer );
}

/**
 *	\brief Update all tiles of the OLED with the tiles of the framebuffer,
 *		   independent of there dirty state.
 */
enum status_code featherWing_OLED_update_all(
	framebuffer_t *const framebuffer	//< framebuffer to write to the FeatherWing_OLED
){
	return ssd1306_display_update_all( featherWing_OLED, framebuffer );
}

