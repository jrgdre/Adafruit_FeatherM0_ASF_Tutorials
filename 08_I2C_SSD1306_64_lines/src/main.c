/** \file     main.c

    \brief    SSD1306 I2C OLED 128x64 tutorial application implementation
 
    \mainpage SSD1306 I2C OLED 128x64 tutorial application

    \license 
              MIT: The MIT License (https://opensource.org/licenses/MIT)
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
              1.0.0: 2018-04-08 jrgdre initial release

 */

#include <asf.h>                        // Atmel Software Foundation
#include "Adafruit_FeatherM0_RS232.h"   // FeatherM0 RS232
#include "Com_Driver_i2c_master.h"      // SSD1306 uses I2C to communicate with the MCU
#include "Font.h"                       // general font related declarations
#include "Font_06px.h"                  // our tiny 6px character font
#include "Font_08px.h"                  // our 8px character font
#include "Framebuffer_SSD1306.h"        // SSD1306 framebuffer
#include "SSD1306.h"                    // SSD1306 display controller driver

#define STRING_EOL    "\r\n"
#define STRING_HEADER STRING_EOL \
"-- DIT Adafruit FeatherM0 ASF Turorials - 08_I2C_SSD1306_64_lines --"STRING_EOL \
"-- Adafruit FeatherM0 --"STRING_EOL        \
"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

// ====================================================
// unit global variables
//
// these variables are global for this unit (static),
// so they can be accessed by the callback functions
// ====================================================

static               bool  display_is_on; // state of the OLED display
static struct Framebuffer *framebuffer  ; // pointer to a framebuffer for the featherWing_OLED

/** 
 * SSD1306 based displays' global configuration data structure instance
 * with default configuration for 128x64 resolution.
 */
// The initialization will cause a compiler warning, despite the code being correct.
// This is a known as GCC bug 53119.
static struct SSD1306  ssd1306 = {
                           .com_driver         = NULL,
                           .address            = 0x3C,
                           .geometry           = SSD1306_GEOMETRY_128x64,
                           .charge_pump_source = SSD1306_CHARGE_PUMP_SOURCE_INTERNAL_VCC,
                           .flip_horizontal    = true,
                           .flip_vertical      = true
                       };


// ====================================================
// some constants for drawing things on the OLED
// ====================================================

#define X_MIN 0
#define X_MAX (128 - 1)
#define X_MID (X_MAX >> 1)

#define Y_MIN 0
#define Y_MAX (64 - 1)
#define Y_MID (Y_MAX >> 1)

enum Pixel_Values {
        BLACK = 0x00,
        WHITE = 0x01
};

enum Resetscreen_Content {
        FONT_06PX = 0x00,
        FONT_08PX = 0x01
};

// ====================================================
// event handlers
// ====================================================

static void on_reset ( enum Resetscreen_Content rsc )
{
        uint8_t string_width;
        uint8_t x_pos = 0;

        switch( rsc ){
                case FONT_08PX: {
                        font_08px_draw_string( framebuffer, "Hello World!", WHITE, x_pos, 1, FONT_BACKGROUND_OPAQUE, &string_width);
                        font_06px_draw_string( framebuffer, "-73db 192.168.1.1 Drechsler-EXT", WHITE, x_pos,  Y_MAX-6, FONT_BACKGROUND_OPAQUE, &string_width);
                        break;
                }
                default: {
                        font_08px_draw_string( framebuffer, "Hello World!"                   , WHITE, x_pos, Y_MIN     , FONT_BACKGROUND_OPAQUE, &string_width);
                        font_06px_draw_string( framebuffer, "abcdefghijklmnopqrstuvwxyz"     , WHITE, x_pos, Y_MIN +  9, FONT_BACKGROUND_OPAQUE, &string_width);
                        font_06px_draw_string( framebuffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"     , WHITE, x_pos, Y_MIN + 16, FONT_BACKGROUND_OPAQUE, &string_width);
                        font_06px_draw_string( framebuffer, "0123456789 @!\"§$\\/"           , WHITE, x_pos, Y_MIN + 23, FONT_BACKGROUND_OPAQUE, &string_width);
                        font_08px_draw_string( framebuffer, "abcdefghijklmnopq"              , WHITE, x_pos, Y_MIN + 30, FONT_BACKGROUND_OPAQUE, &string_width);
                        font_08px_draw_string( framebuffer, "ABCDEFGHIJKLMNOP"               , WHITE, x_pos, Y_MIN + 39, FONT_BACKGROUND_OPAQUE, &string_width);
                        font_08px_draw_string( framebuffer, "0123456789 @!\"§$\\/"           , WHITE, x_pos, Y_MIN + 48, FONT_BACKGROUND_OPAQUE, &string_width);
                        font_06px_draw_string( framebuffer, "-73db 192.168.1.1 Drechsler-EXT", WHITE, x_pos, Y_MAX -  6, FONT_BACKGROUND_OPAQUE, &string_width);
                        break;
                }
        }
}

// ====================================================
// OLED initialization
// ====================================================

static enum status_code oled_init(
  struct i2c_master_module *i2c_master_instance
){
        enum status_code status;
        
        // link the generic com driver instance to the i2c bus master instance specified
        com_driver_i2c_master_init( &ssd1306.com_driver, i2c_master_instance );
        
        status = ssd1306_init( &ssd1306 ); // initialize the driver chip on the featherWing_OLED
        if( status != STATUS_OK ) {
                return status;
        };
        
        display_is_on = false;

        return STATUS_OK;
}

// ====================================================
// i2c interface configuration
// ====================================================

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

// ===========================================================================
// program entry point
// ===========================================================================

int main ( void )
{
        struct usart_module       usart_instance    ;           // an instance of the ASF SERCOM/USART module
        struct i2c_master_module  i2c_master_instance;          // an instance of the ASF SERCOM/I2C module
        enum   status_code        status = STATUS_OK ;          // a return value
        
        // --------------------------------------------
        // board, driver and service initialization [1]
        // --------------------------------------------
        
        system_init();                                          // board initialization
        system_interrupt_enable_global();                       // enable global interrupt system for USART callbacks to work
        
        // ---------------------------------------------
        // DIT Adafruit_FeatherM0 Library initialization
        // ---------------------------------------------

        /* configure the debug message terminal */
        RS232_configure( &usart_instance, 115200 );             // configure USART
        RS232_enable   ( &usart_instance );                     // enable USART and stdio_serial (if included)

        printf(STRING_HEADER);

        // --------------------------------------------
        // board, driver and service initialization [2]
        // --------------------------------------------

        /* configure an i2c hardware device and enable it */
        i2c_master_configure( &i2c_master_instance );           // configure the MCU hardware used
        i2c_master_enable   ( &i2c_master_instance );           // enable the MCU hardware
        
        // ----------------------------------------------------
        // DIT Adafruit_FeatherWing_OLED Library initialization
        // ----------------------------------------------------
        
        /* initialize the OLED display (initial state is OFF) */
        status = oled_init( &i2c_master_instance );
        if( status != STATUS_OK ) {
                printf( "main: initializing OLED error %d"STRING_EOL, status );
                while( true ){ // stop processing
                }
        }

        // --------------------
        // Framebuffer creation
        // --------------------
        
        framebuffer = framebuffer_SSD1306_create( X_MAX+1, Y_MAX+1 );

        // -----------------
        // application logic
        // -----------------

        framebuffer->clear( framebuffer );                              // reset all pixels in the framebuffer to 0x00
        status = ssd1306_display_update( &ssd1306, framebuffer );       // send the framebuffer over to the screen
        
        ssd1306_display_on( &ssd1306, true );                           // switch the OLED on
        display_is_on = true;                                           // and remember the state

        on_reset( FONT_06PX );                                          // write our initial screen content to the framebuffer
        status = ssd1306_display_update( &ssd1306, framebuffer );       // send the framebuffer over to the screen
        
        while ( true ) {
                /* nothing to do */
        }
}
