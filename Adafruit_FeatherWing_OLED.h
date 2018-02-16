/**	\file	Adafruit_FeatherWing_OLED.h

 	\brief	Adafruit FeatherWing OLED driver declarations
 
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
#ifndef ADAFRUIT_FEATHERWING_OLED_H_
#define ADAFRUIT_FEATHERWING_OLED_H_

#include "Com_Driver.h"			// Generic Communication Driver 
#include "Framebuffer_1bit.h"

#define ADAFRUIT_FEATHERWING_OLED_I2C_ADDRESS	0x3C	//< fixed value for the FeatherWing_OLED
#define ADAFRUIT_FEATHERWING_OLED_WIDTH			 128	//< fixed value for the FeatherWing_OLED
#define ADAFRUIT_FEATHERWING_OLED_HEIGHT		  32	//< fixed value for the FeatherWing_OLED

//* number of dirty tiles to update tile by tile, before it is more efficient to update the whole display 
#define ADAFRUIT_FEATHERWING_OLED_TILES_DIRTY_THRESHOLD  147 

// ATSAMD21G18 pins the buttons are connected to
#define BUTTON_A_PIN		PIN_PA07A_EIC_EXTINT7		// A (SW2) is on PA07, using external interrupt controller line 7
#define BUTTON_A_PIN_MUX	MUX_PA07A_EIC_EXTINT7
#define BUTTON_A_EIC_LINE	7
#define BUTTON_B_PIN		PIN_PA20A_EIC_EXTINT4		// B (SW4) is on PA20, using external interrupt controller line 4
#define BUTTON_B_PIN_MUX	MUX_PA20A_EIC_EXTINT4
#define BUTTON_B_EIC_LINE	4
#define BUTTON_C_PIN		PIN_PA15A_EIC_EXTINT15		// C (SW3) is on PA15, using external interrupt controller line 15
#define BUTTON_C_PIN_MUX	MUX_PA15A_EIC_EXTINT15
#define BUTTON_C_EIC_LINE	15

/**
 *	Prototype of a button event handler function.
 */
typedef void featherWing_OLED_button_event_handler_t( void );

/**
 *	Button event handlers management structure data type.
 */
typedef struct FeatherWing_OLED_Buttons_Event_Hhandlers
{
	featherWing_OLED_button_event_handler_t *Button_A_On_Pressed ;
	featherWing_OLED_button_event_handler_t *Button_A_On_Released;
	featherWing_OLED_button_event_handler_t *Button_B_On_Pressed ;
	featherWing_OLED_button_event_handler_t *Button_B_On_Released;
	featherWing_OLED_button_event_handler_t *Button_C_On_Pressed ;
	featherWing_OLED_button_event_handler_t *Button_C_On_Released;
} featherWing_OLED_buttons_event_handlers_t;

/**
 *	Button event handlers management structure.
 *
 *	\note	Register your event handler callbacks here.
 *
 *	\note	Make sure to unregister your event handler callbacks here,
 *			by setting the fields to NULL, when done.
 */
featherWing_OLED_buttons_event_handlers_t featherWing_OLED_buttons_event_handlers;

/**
 *	Initialize the FeatherWing_OLED display connected to the i2c SERCOM \ref module.
 *
 *	The function also initializes the \ref framebuffer
 *	- \ref columns			 = 128 (\ref ADAFRUIT_FEATHERWING_OLED_WIDTH)
 *	- \ref pages			 =   4 (\ref ADAFRUIT_FEATHERWING_OLED_HEIGHT / 8 bit per page)
 *	- \ref bytes			 = 512 (4 pages a 128 columns)
 *	- \ref bits_per_tile	 =   8
 *	- \ref tiles*			 = is the pointer to a NEW allocated pixel buffer (512 bytes)
 *	- \ref tiles_dirty*		 = is the pointer the index of dirty tiles (64 bytes)
 *	- \ref tiles_dirty_count =   0
 *
 *	\note Initial state of the display is off
 *
 *	\note The function allocates new tiles and tiles_dirty buffers on every call
 *		  allowing for multiple frame-buffers to be used at the same time.
 *
 *	\note Implementation uses 
 *	      - \ref ssd1306_init() to initialize the controller 
 *	      - \ref ssd1306_command_sequence_write_wait() to initialize the FeatherWing_OLED specific parameters
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                    If initialization was successfully
 *	\retval STATUS_BUSY                  If master module is busy
 *	\retval STATUS_ERR_DENIED            If error on bus
 *	\retval STATUS_ERR_PACKET_COLLISION  If arbitration is lost
 *	\retval STATUS_ERR_BAD_ADDRESS       If slave is busy, or no slave
 *	                                     acknowledged the address
 *	\retval STATUS_ERR_TIMEOUT           If timeout occurred
 *	\retval STATUS_ERR_OVERFLOW          If slave did not acknowledge last sent
 *	                                     data, indicating that slave do not want
 *	                                     more data
 */ 
enum status_code featherWing_OLED_init(
	struct i2c_master_module *			//< I2C master this featherWing_OLED is connected to
);

/**
 *	Initialize the FeatherWing_OLED buttons.
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                    If operation was successfully
 */
enum status_code featherWing_OLED_init_buttons( void );

/**
 *	Switch the state of the OLED display.
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                    If initialization was successfully
 *	\retval STATUS_BUSY                  If master module is busy
 *	\retval STATUS_ERR_DENIED            If error on bus
 *	\retval STATUS_ERR_PACKET_COLLISION  If arbitration is lost
 *	\retval STATUS_ERR_BAD_ADDRESS       If slave is busy, or no slave
 *	                                     acknowledged the address
 *	\retval STATUS_ERR_TIMEOUT           If timeout occurred
 *	\retval STATUS_ERR_OVERFLOW          If slave did not acknowledge last sent
 *	                                     data, indicating that slave do not want
 *	                                     more data
 */
enum status_code featherWing_OLED_on(
	bool  const value	//< if OLED display is to set to the ON state
);

/**
 *	Update the tiles of the OLED with the tiles of the framebuffer,
 *	depending on there dirty state in the framebuffer.
 *
 *	If the number of tiles dirty is bigger than the ADAFRUIT_FEATHERWING_OLED_TILES_DIRTY_THRESHOLD
 *	this implementation calls \ref featherWing_OLED_update_all().
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                    If update was successfully
 *	\retval STATUS_BUSY                  If master module is busy
 *	\retval STATUS_ERR_DENIED            If error on bus
 *	\retval STATUS_ERR_PACKET_COLLISION  If arbitration is lost
 *	\retval STATUS_ERR_BAD_ADDRESS       If slave is busy, or no slave
 *	                                     acknowledged the address
 *	\retval STATUS_ERR_TIMEOUT           If timeout occurred
 *	\retval STATUS_ERR_OVERFLOW          If slave did not acknowledge last sent
 *	                                     data, indicating that slave do not want
 *	                                     more data
 *	\retval STATUS_ERR_INVALID_ARG		 Usually happens if module is NULL or framebuffer NULL or not initialized
 *	\retval STATUS_ERR_NOT_INITIALIZED	 If update could not be initialized
 */
enum status_code featherWing_OLED_update(
	framebuffer_t *const framebuffer	//< framebuffer to write to the FeatherWing_OLED
);

/**
 *	Update all tiles of the OLED with the tiles of the framebuffer,
 *	independent of there dirty state in the framebuffer.
 *
 *	\return Status of operation. 
 *	\retval STATUS_OK                    If update was successfully
 *	\retval STATUS_BUSY                  If master module is busy
 *	\retval STATUS_ERR_DENIED            If error on bus
 *	\retval STATUS_ERR_PACKET_COLLISION  If arbitration is lost
 *	\retval STATUS_ERR_BAD_ADDRESS       If slave is busy, or no slave
 *	                                     acknowledged the address
 *	\retval STATUS_ERR_TIMEOUT           If timeout occurred
 *	\retval STATUS_ERR_OVERFLOW          If slave did not acknowledge last sent
 *	                                     data, indicating that slave do not want
 *	                                     more data
 *	\retval STATUS_ERR_INVALID_ARG		 Usually happens if module is NULL or framebuffer NULL or not initialized
 *	\retval STATUS_ERR_NOT_INITIALIZED	 If update could not be initialized
 */
enum status_code featherWing_OLED_update_all(
	framebuffer_t *const framebuffer	//< framebuffer to write to the FeatherWing_OLED
);

#endif /* _ADAFRUIT_FEATHERWING_OLED_H_ */
