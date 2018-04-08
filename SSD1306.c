/**     \file   SSD1306.c

        \brief  SSD1306 display controller driver implementation
 
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
                1.0.0: 2017-06-21 jrgdre initial release

 */

#include <string.h>   // we use memset()
#include "SSD1306.h"
#include "Framebuffer_SSD1306.h"

/**     
 * \brief  Write a sequence of bytes to the SSD1306
 *
 * \asserts (ssd1306  != NULL)
 * \asserts (sequence != NULL)
 */
static enum status_code ssd1306_sequence_write (
         struct SSD1306 *const ssd1306          //< data structure of the SSD1306 controller to write the command sequence to
, enum SSD1306_Datatype  const datatype         //< type of data to write
,               uint8_t *const sequence         //< byte sequence to write
,              uint16_t  const sequence_length  //< length of sequence
) {
        enum status_code status;
        
        Assert( ssd1306  != NULL );
        Assert( sequence != NULL );
        
        if ( sequence_length < 1 ){
                status = STATUS_OK;
                goto done;
        }
        
        struct Com_Driver  com_driver    = ssd1306->com_driver;
        void              *com_module    = com_driver.com_module;
        bool               send_stop_org = com_driver.get_send_stop( com_module ); // save module's original setting

        com_driver.set_send_stop( com_module, false );  // do not release the i2c bus until finished sending the whole sequence

        uint8_t bytes[2];
        
        bytes[0] = datatype;
        
        // send the command sequence byte by byte
        for( uint_fast32_t idx = 0; idx < sequence_length; idx++ ) {
                bytes[1] = sequence[idx];
                status = com_driver.write_wait( com_module, ssd1306->address, bytes, 2 );
                if( status != STATUS_OK ) {
                        goto done;
                }
        }

done:
        ssd1306->com_driver.set_send_stop( com_module, send_stop_org ); // restore driver's original setting
        ssd1306->com_driver.send_stop    ( com_module );                // release the i2c bus
        return status;
};

/**
 * \asserts (ssd1306  != NULL)
 * \asserts (sequence != NULL)
 */
enum status_code ssd1306_command_sequence_write (
  struct SSD1306 *const ssd1306                 //< data structure of the SSD1306 controller to write the command sequence to
,        uint8_t *const command_sequence        //< command sequence to write
,       uint16_t  const sequence_length         //< length of command sequence
) {
        Assert( ssd1306  != NULL );
        Assert( command_sequence != NULL );
        
        return ssd1306_sequence_write( ssd1306, SSD1306_COMMAND, command_sequence, sequence_length );
};

/**
 * \asserts (ssd1306  != NULL)
 * \asserts (sequence != NULL)
 */
enum status_code ssd1306_data_sequence_write (
  struct SSD1306 *const ssd1306                 //< data structure of the SSD1306 controller to write the command sequence to
,        uint8_t *const data_sequence           //< data sequence to write
,       uint16_t  const sequence_length         //< length of data sequence
) {
        Assert( ssd1306  != NULL );
        Assert( data_sequence != NULL );
        
        return ssd1306_sequence_write( ssd1306, SSD1306_DATA, data_sequence, sequence_length );
};

/**
 * \asserts (ssd1306 != NULL)
 */
enum status_code ssd1306_init (
      struct SSD1306 *ssd1306 
){
        Assert( ssd1306 != NULL );
        
        enum status_code  status = STATUS_OK               ;
        
                 uint8_t  width  = 128                     ;
                 uint8_t  height = ssd1306->geometry       ;  // see declaration of geometry, why this works
                 uint8_t  pages  = (ssd1306->geometry >> 3);
        
                 uint8_t  contrast ;
                 uint8_t  precharge;
                 uint8_t  hw_conf  ;
        
        switch (ssd1306->geometry) {
                case SSD1306_GEOMETRY_128x32:
                        hw_conf = SSD1306_COM_PIN_CONFIGURATION_SEQUENTIAL | SSD1306_COM_PIN_REMAP_DISABLE;
                        break;
                case SSD1306_GEOMETRY_128x64:
                        hw_conf = SSD1306_COM_PIN_CONFIGURATION_ALTERNATIVE;
                        break;
        };
        switch (ssd1306->charge_pump_source) {
                case SSD1306_CHARGE_PUMP_SOURCE_EXTERNAL_VCC:
                        precharge = 0x22;
                        contrast  = 0x9F;
                        break;
                case SSD1306_CHARGE_PUMP_SOURCE_INTERNAL_VCC:
                        precharge = 0xF1;
                        contrast  = 0xCF;
                        break;
        }
        
        #define SSD1306_INIT_SEQUENCE_LENGTH 25

        uint8_t init_sequence[SSD1306_INIT_SEQUENCE_LENGTH] = {
                // display control
                SSD1306_DISPLAY_OFF                         ,                               // turn display off for reconfiguration
                // hardware configuration
                SSD1306_SET_CHARGE_PUMP_SOURCE              , ssd1306->charge_pump_source , // configure and enable charge pump
                SSD1306_SET_MULTIPLEX_RATIO                 , height - 1                  , // value is based on display height
                SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION , hw_conf                     , //
                // timing and driving scheme configuration
                SSD1306_SET_DISPLAY_OSC_FREQ_AND_CLOCK_DIV  , 0x80                        ,
                SSD1306_SET_PRECHARGE_PERIODE               , precharge                   ,
                SSD1306_SET_VCOMH_DESELECT_LEVEL            , SSD1306_VCOMH_0p77          ,
                // addressing mode configuration
                SSD1306_SET_MEMORY_ADRESSING_MODE           , SSD1306_ADDR_MODE_HORIZONTAL, // page 0 from col 0 to col 127, than page 1 from col 0 to col127 ->->-> (auto increment col, page)
                SSD1306_SET_PAGE_ADRESSES                   , 0x00, pages - 1             , // initial page-range to send data to: start 
                SSD1306_SET_COLUMN_ADRESSES                 , 0x00, width - 1             , // initial column to send data to
                // fundamentals
                SSD1306_SET_DISPLAY_RESUME_RAM              ,                               // show GDDRAM content on DISPLAY_ON
                SSD1306_SET_DISPLAY_NORMAL                  ,                               // normal picture
                SSD1306_SET_CONTRAST                        , contrast                    , // set an initial contrast value
        };

        status = ssd1306_command_sequence_write( ssd1306, init_sequence, SSD1306_INIT_SEQUENCE_LENGTH );
        if( status != STATUS_OK ){
                goto done;
        }
        
        if( ssd1306->flip_horizontal ){
                #define SSD1306_HFLIP_SEQUENCE_LENGTH 1
                
                uint8_t hflip_sequence[SSD1306_HFLIP_SEQUENCE_LENGTH] = {
                        SSD1306_SET_COM_OUTPUT_DIRECTION_REMAP
                };
                
                status = ssd1306_command_sequence_write( ssd1306, hflip_sequence, SSD1306_HFLIP_SEQUENCE_LENGTH );
                if( status != STATUS_OK ){
                        goto done;
                }
        }
        
        if( ssd1306->flip_vertical ){
                #define SSD1306_VFLIP_SEQUENCE_LENGTH 1
                
                uint8_t vflip_sequence[SSD1306_VFLIP_SEQUENCE_LENGTH] = {
                        SSD1306_SET_SEGMENT_REMAP
                };
                
                status = ssd1306_command_sequence_write( ssd1306, vflip_sequence, SSD1306_VFLIP_SEQUENCE_LENGTH );
                if( status != STATUS_OK ){
                        goto done;
                }
        }

done:        
        return status;
};

/**
 * \asserts (ssd1306 != NULL)
 */
enum status_code ssd1306_display_on (
  struct SSD1306 *const ssd1306                 //< data structure of the SSD1306 controller to write the command  to
,           bool  const value                   //< display on state to set (true: ON; false: OFF)
){
        Assert( ssd1306 != NULL );
        
        #define SSD1306_DISPLAY_STATE_SEQUENCE_LENGTH 1

        uint8_t sequence[SSD1306_DISPLAY_STATE_SEQUENCE_LENGTH] = { 
                SSD1306_DISPLAY_ON 
        };
        
        if( !value ) {
                sequence[0] = SSD1306_DISPLAY_OFF;
        }

        return ssd1306_command_sequence_write( ssd1306, sequence, SSD1306_DISPLAY_STATE_SEQUENCE_LENGTH );
};

/**
 * \brief Send the page and column addresses of the next tile to write data to, to the FeatherWing
 *
 * \asserts (ssd1306 != NULL)
 */
static enum status_code set_tile_next (
  struct SSD1306 *const ssd1306                 //< data structure of the SSD1306 controller to write the command  to
,        uint8_t  const page_start              //< first column to write next data byte to
,        uint8_t  const column_start            //< first column to write next data byte to
){
        Assert( ssd1306 != NULL );
        
        uint8_t page_end   = (ssd1306->geometry >> 3) - 1;
        uint8_t column_end = 127;
        
        return ssd1306_set_page_range  ( ssd1306, page_start  , page_end   )
            || ssd1306_set_column_range( ssd1306, column_start, column_end );
};

/**
 * \brief Returns the tiles dirty threshold base on the display geometry
 */
inline static uint32_t get_tiles_dirty_threshold( 
  enum SSD1306_Geometry geometry 
){
        uint32_t result;
        
        switch ( geometry ) {
                case SSD1306_GEOMETRY_128x32:
                        result = 147;
                        break;
                case SSD1306_GEOMETRY_128x64:
                        result = 294;
                        break;
        }

        return result;
}

/**
 * \asserts ssd1306                       != NULL
 * \asserts ssd1306.com_driver            != NULL
 * \asserts ssd1306.com_driver.com_module != NULL
 *
 * \asserts framebuffer                          != NULL
 * \asserts framebuffer->user_data               != NULL
 * \asserts framebuffer->user_data->bytes        >  0
 * \asserts framebuffer->user_data->tiles        != NULL
 * \asserts framebuffer->user_data->tiles _dirty != NULL
 */
enum status_code ssd1306_display_update_all (
  struct SSD1306     *const ssd1306             //< data structure of the SSD1306 controller to write the update to
, struct Framebuffer *const framebuffer         //< framebuffer holding the data to display on the OLED
){
        Assert( ssd1306                       != NULL );
        Assert( ssd1306.com_driver            != NULL );
        Assert( ssd1306.com_driver.com_module != NULL );
        
        struct Com_Driver com_driver = ssd1306->com_driver  ;
        void             *com_module = com_driver.com_module;
        
        Assert( framebuffer->user_data  != NULL );
        
        struct Framebuffer_SSD1306 *fb_ssd1306 = (struct Framebuffer_SSD1306 *)framebuffer->user_data;
        
        Assert( fb_ssd1306->bytes       >  0    );
        Assert( fb_ssd1306->tiles       != NULL );
        Assert( fb_ssd1306->tiles_dirty != NULL );
        
                    bool  send_stop_org = com_driver.get_send_stop( com_module );
        enum status_code  status        = STATUS_ERR_NOT_INITIALIZED;
                uint32_t  tile_idx      = 0;

        com_driver.set_send_stop( com_module, false );  // do not release the i2c bus until finished sending all the tiles

        // we use a while loop to break if there is an error and still be able to perform some cleanup
        while( status != STATUS_OK ) {
                // initialize the [page,column] start point for the update
                status = set_tile_next( ssd1306, 0x00, 0x00 );
                if( status != STATUS_OK ){
                        goto done;
                }
                // send the fb_ssd1306 byte by byte
                for( uint_fast16_t page=0; page < fb_ssd1306->pages; page++ ) {
                        for( uint_fast16_t column=0; column < fb_ssd1306->columns; column++ ) {
                                tile_idx = (page*fb_ssd1306->columns) + column;
                                status   = ssd1306_data_sequence_write( ssd1306, &fb_ssd1306->tiles[tile_idx], 1);
                                if( status != STATUS_OK ) {
                                        goto done;
                                }
                        }
                }
                memset( (void *)fb_ssd1306->tiles_dirty, 0x00, fb_ssd1306->bytes >> 3 ); // mark all tiles as clean
                fb_ssd1306->tiles_dirty_count = 0;
        }

done:
        com_driver.send_stop    ( com_module );                 // release i2c bus
        com_driver.set_send_stop( com_module, send_stop_org );  // restore original setting
        
        return status;
};

/**
 * \asserts ssd1306                       != NULL
 * \asserts ssd1306.com_driver            != NULL
 * \asserts ssd1306.com_driver.com_module != NULL
 *
 * \asserts framebuffer                          != NULL
 * \asserts framebuffer->user_data               != NULL
 * \asserts framebuffer->user_data->bytes        >  0
 * \asserts framebuffer->user_data->tiles        != NULL
 * \asserts framebuffer->user_data->tiles _dirty != NULL
 */
enum status_code ssd1306_display_update_differential (
  struct SSD1306     *const ssd1306             //< data structure of the SSD1306 controller to write the update to
, struct Framebuffer *const framebuffer         //< framebuffer holding the data to display on the OLED
){
        Assert( ssd1306                       != NULL );
        Assert( ssd1306.com_driver            != NULL );
        Assert( ssd1306.com_driver.com_module != NULL );
        
        struct Com_Driver com_driver = ssd1306->com_driver  ;
        void             *com_module = com_driver.com_module;
        
        Assert( framebuffer->user_data   != NULL );
        
        struct Framebuffer_SSD1306 *fb_ssd1306 = (struct Framebuffer_SSD1306 *)framebuffer->user_data;
        
        Assert( fb_ssd1306->bytes       >  0    );
        Assert( fb_ssd1306->tiles       != NULL );
        Assert( fb_ssd1306->tiles_dirty != NULL );
        
                    bool  send_stop_org        = com_driver.get_send_stop( com_module );
        enum status_code  status               = STATUS_ERR_NOT_INITIALIZED;
        
                uint16_t  tiles_dirty_byte_idx = 0;                                             // 1 bit per tile in tiles_dirty
                 uint8_t  tiles_dirty_bit_idx  = 0;                                             // bit position of tile in tiles_dirty index byte
                uint32_t  tile_idx             = 0;                                             // tile the pixel is in
                uint32_t  tile_idx_prev        = 0;                                             // idx of the tile update last
                    bool  tile_is_dirty        = false;                                         // current value
                    bool  error                = false;                                         // true if an error occurred during processing

        com_driver.set_send_stop( com_module, false ); // do not release the i2c bus until finished sending all the tiles

        // we use a while loop to break if there is an error and still be able to perform some cleanup
        while( status != STATUS_OK ) {
                // send the dirty tiles byte by byte
                for( uint_fast16_t page=0; page < fb_ssd1306->pages; page++ ) {
                        for( uint_fast16_t column=0; column < fb_ssd1306->columns; column++ ) {
                                tile_idx             = ( page * fb_ssd1306->columns ) + column; // tile the pixel is in
                                tiles_dirty_byte_idx = tile_idx >> 3;                           // 1 bit per tile in tiles_dirty
                                tiles_dirty_bit_idx  = tile_idx % (tiles_dirty_byte_idx << 3);  // bit position of tile in tiles_dirty_idx byte
                                
                                tile_is_dirty        = \
                                        ( fb_ssd1306->tiles_dirty[ tiles_dirty_byte_idx ] & ( 0x1 << tiles_dirty_bit_idx ) ) >> tiles_dirty_bit_idx; // current value
                                        
                                if( tile_is_dirty ){                                                    // tile marked as dirty
                                        if(( tile_idx      == 0          )                              // first tile
                                        || ( tile_idx_prev != tile_idx-1 )                              // we skipped one or more tiles in the meantime
                                        ){
                                                status = set_tile_next( ssd1306, page, column );        // tell the ssd1306 which tile comes next
                                                if( status != STATUS_OK ){
                                                        goto done;
                                                }
                                        }
                                        status = ssd1306_data_sequence_write( ssd1306, &fb_ssd1306->tiles[(page*fb_ssd1306->columns) + column], 1);
                                        if( status != STATUS_OK ) {
                                                goto done;
                                        }
                                        fb_ssd1306->tiles_dirty[ tiles_dirty_byte_idx ] &= ~( 0x1 << tiles_dirty_bit_idx ); // tile is clean now
                                        fb_ssd1306->tiles_dirty_count--;                                                    // decrease dirty tiles count
                                        if( fb_ssd1306->tiles_dirty_count < 1 ){
                                                goto done;                                                                  // nothing left to do
                                        }
                                        tile_idx_prev = tile_idx;
                                }
                        }
                        if(( error )
                        || ( fb_ssd1306->tiles_dirty_count < 1 )        // nothing left to do
                        ){
                                break;
                        }
                }
        }

done:
        com_driver.send_stop    ( com_module );                 // release i2c bus
        com_driver.set_send_stop( com_module , send_stop_org ); // restore original setting
                
        return status;
};
  
/**
 * \asserts ssd1306                              != NULL
 *
 * \asserts framebuffer                          != NULL
 * \asserts framebuffer->user_data               != NULL
 * \asserts framebuffer->user_data->bytes        >  0
 * \asserts framebuffer->user_data->tiles        != NULL
 * \asserts framebuffer->user_data->tiles _dirty != NULL
 */
enum status_code ssd1306_display_update (
  struct SSD1306     *const ssd1306             //< data structure of the SSD1306 controller to write the update to
, struct Framebuffer *const framebuffer         //< framebuffer holding the data to display on the OLED
){
        Assert( ssd1306                  != NULL );
        
        Assert( framebuffer              != NULL );
        Assert( framebuffer->user_data   != NULL );
        
        struct Framebuffer_SSD1306 *fb_ssd1306 = (struct Framebuffer_SSD1306 *)framebuffer->user_data; // in user_data we should find the pointer to the SSD1306 framebuffer
        
        Assert( fb_ssd1306->bytes       >  0    );
        Assert( fb_ssd1306->tiles       != NULL );
        Assert( fb_ssd1306->tiles_dirty != NULL );
        
        enum status_code status;
        
        if ( fb_ssd1306->tiles_dirty_count < 1 ){               // nothing to do
                status = STATUS_OK; 
                goto done;              
        }
        
        uint32_t tiles_dirty_threshold = get_tiles_dirty_threshold( ssd1306->geometry );
        
        if( fb_ssd1306->tiles_dirty_count > tiles_dirty_threshold ) {                   
                status = ssd1306_display_update_all         ( ssd1306, framebuffer );
        } else {                                                                        // if the number of tiles dirty is greater than the threshold
                status = ssd1306_display_update_differential( ssd1306, framebuffer );   // it is faster to send the whole buffer
        }

done:
        return status;
};

/**
 * \asserts (ssd1306 != NULL)
 */
enum status_code ssd1306_set_column_range (
  struct SSD1306 *const ssd1306               //< data structure of the SSD1306 controller to write the command  to
,        uint8_t  const column_start          //< first column to write next data byte to
,        uint8_t  const column_end            //< last column to write a data byte to 
){
        Assert( ssd1306 != NULL );

        #define SSD1306_SET_COLUMN_RANGE_SEQUENCE_LENGTH 3

        uint8_t sequence[SSD1306_SET_COLUMN_RANGE_SEQUENCE_LENGTH] = {
                SSD1306_SET_COLUMN_ADRESSES, column_start, column_end
        };
        
        return ssd1306_command_sequence_write( ssd1306, sequence, SSD1306_SET_COLUMN_RANGE_SEQUENCE_LENGTH );
}

/**
 * \asserts (ssd1306 != NULL)
 */
enum status_code ssd1306_set_page_range (
  struct SSD1306 *const ssd1306                 //< data structure of the SSD1306 controller to write the command  to
,        uint8_t  const page_start              //< first column to write next data byte to
,        uint8_t  const page_end                //< last column to write a data byte to 
){
        Assert( ssd1306 != NULL );
        
        #define SSD1306_SET_PAGE_ADDRESS_SEQUENCE_LENGTH 3

        uint8_t sequence[SSD1306_SET_PAGE_ADDRESS_SEQUENCE_LENGTH] = {
                SSD1306_SET_PAGE_ADRESSES, page_start, page_end
        };
        
        return ssd1306_command_sequence_write( ssd1306, sequence, SSD1306_SET_PAGE_ADDRESS_SEQUENCE_LENGTH );
}
