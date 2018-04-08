/**     \file   Framebuffer_SSD1306.c

        \brief  Implementation of a 1bit per pixel framebuffer for the SSD1306 OLED controller IC
 
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
                DIT: 2018; Drechsler Information Technologies; www.drechsler-it.de
 
        \authors
                jrgdre: Joerg Drechsler; DIT
 
        \versions
                1.0.0: 2018-02-18 jrgdre initial release

 */
#include <string.h>                     // memory functions
#include "Framebuffer.h"                // generic framebuffer interface
#include "Framebuffer_SSD1306.h"        // SSD1306 framebuffer interface

// ===========================================================================
//  private
// ===========================================================================

enum Framebuffer_Pixel_Mode {
        PIXEL_CLEAR       = 0x00,       //< set pixel's value to 0
        PIXEL_SET         = 0x01,       //< set pixel's value to 1
        PIXEL_TRANSPARENT = 0x02        //< transparent mode, if pixel_value = 1 set pixel to 1, if pixel_value = 0 keep current value of pixel
};

/**
 * \brief Set every pixel in framebuffer to 0x0
 *
 * \asserts fb            != NULL
 * \asserts fb->user_data != NULL
 */
static enum status_code framebuffer_ssd1306_clear (
        struct Framebuffer *fb                  //< framebuffer to clear
){
        Assert( fb            != NULL );
        Assert( fb->user_data != NULL );
        
        struct Framebuffer_SSD1306 *fb_ssd1306 = (struct Framebuffer_SSD1306 *)fb->user_data;
        
        Assert( fb_ssd1306->tiles       != NULL );
        Assert( fb_ssd1306->tiles_dirty != NULL );

        memset( (void *)fb_ssd1306->tiles      , 0x00, fb_ssd1306->bytes      );        // reset all pixel to 0x00
        memset( (void *)fb_ssd1306->tiles_dirty, 0xFF, fb_ssd1306->bytes >> 3 );        // mark all tiles as dirty
        fb_ssd1306->tiles_dirty_count = fb_ssd1306->bytes;                              // set the dirty tiles count to the number of tiles
        
        return STATUS_OK;
};

/**
 * \brief Wipe and release a framebuffer's memory
 */
static enum status_code framebuffer_ssd1306_destroy (
        struct Framebuffer *fb
){
        if( fb == NULL ){
                goto done;
        }
        
        if( fb->user_data != NULL ){
                
                struct Framebuffer_SSD1306 *fb_ssd1306 = (struct Framebuffer_SSD1306 *)fb->user_data;
                
                if( fb_ssd1306->tiles != NULL ){
                        memset( (void *)fb_ssd1306->tiles      , 0x00, fb_ssd1306->bytes      );        // wipe pixel memory
                        free( fb_ssd1306->tiles );                                                      // release pixel memory
                }
                
                if( fb_ssd1306->tiles_dirty != NULL ){
                        memset( (void *)fb_ssd1306->tiles_dirty, 0x00, fb_ssd1306->bytes >> 3 );        // wipe dirty tiles memory
                        free( fb_ssd1306->tiles_dirty );                                                // release dirty tiles memory
                }
                
                memset( (void *)fb_ssd1306, 0x00, sizeof( *fb_ssd1306 ) );                              // wipe fb_ssd1306 memory
                free( fb_ssd1306 );                                                                     // release fb_ssd1306 memory

        }
        
        memset( fb, 0x00, sizeof( *fb ) );                                                              // wipe fb memory
        free( fb );                                                                                     // release fb memory
        
done:
        fb = NULL;
        return STATUS_OK;
};        

/**
 * \brief Initialize a SSD1306 framebuffer structure
 *
 * \asserts fb_ssd1306 != NULL
 */
static enum status_code framebuffer_ssd1306_init (
        struct Framebuffer_SSD1306 *fb_ssd1306  //< SSD1306 framebuffer structure to initialize
,                   uint32_t const  width       //< width of the display the buffer is intended for in pixels
,                   uint32_t const  height      //< height of the display the buffer is intended for in pixels
){
        Assert( fb_ssd1306 != NULL );
        
        fb_ssd1306->columns           = width;                                                          // number of columns, depending on display width
        fb_ssd1306->bits_per_tile     = sizeof( framebuffer_ssd1306_tile_t ) << 3;                      // in bits
        fb_ssd1306->pages             = height / fb_ssd1306->bits_per_tile;                             // number of pages, depending on display height and tile height
        fb_ssd1306->bytes             = fb_ssd1306->columns * fb_ssd1306->pages;                        // overall number of bytes for the fb_ssd1306
        fb_ssd1306->tiles             = (framebuffer_ssd1306_tile_t *)malloc( fb_ssd1306->bytes      ); // allocate a new fb_ssd1306 bitmap (1 bit per pixel)
        fb_ssd1306->tiles_dirty       = (                   uint8_t *)malloc( fb_ssd1306->bytes >> 3 ); // allocate a new dirty tiles index  (1 bit per tile in an 8 bit bitmap)
        fb_ssd1306->tiles_dirty_count = 0;
        
        if(( fb_ssd1306->tiles       == NULL ) 
        || ( fb_ssd1306->tiles_dirty == NULL ) 
        ){
                return STATUS_ERR_NO_MEMORY;
        }
        
        return STATUS_OK;
};

/**
 * Get the tile and bit index for for a pixel in the framebuffer 
 * based on the x,y coordinates of that pixel.
 *
 * \return Status of operation.
 * \retval STATUS_OK                   If operation was successfully
 * \retval STATUS_ERR_INVALID_ARG      If \ref x or \ref y out of bounds
 */
static enum status_code get_framebuffer_indizes (
  struct Framebuffer_SSD1306 const  *fb_ssd1306   //< [in]  pointer to the SSD1306 framebuffer to get the indices for
,                   uint32_t const  x             //< [in]  pixel x position
,                   uint32_t const  y             //< [in]  pixel y position
,                   uint32_t       *tile_idx      //< [out] index of the tile the pixel is in
,                    uint8_t       *bit_idx       //< [out] index of bit in the tile the pixel is in
){
        enum status_code  status  ; // return value of an operation
        
        if( ( tile_idx == NULL ) || ( bit_idx == NULL ) ){
                status = STATUS_ERR_INVALID_ARG;
                goto done;
        }

        // make sure pixel is in bounds
        if(( x >= fb_ssd1306->columns )
        || ( y >= fb_ssd1306->pages * fb_ssd1306->bits_per_tile )
        ){
                status = STATUS_ERR_INVALID_ARG;
                goto done;
        };
        
        uint16_t  page_idx = y / ( sizeof( framebuffer_ssd1306_tile_t ) << 3 ); // index of page we have to draw pixel to
                 *bit_idx  = y % ( sizeof( framebuffer_ssd1306_tile_t ) << 3 ); // index of bit in page we have to draw pixel to
                 *tile_idx = ( page_idx * fb_ssd1306->columns ) + x;            // tile the pixel is in

        status = STATUS_OK;
        
done:
        return status;        
};

/**
 * \brief Get pixel_value for the pixel at [x,y] in framebuffer
 *
 * \asserts ( fb            != NULL )
 * \asserts ( fb->user_data != NULL );
 */
static enum status_code framebuffer_ssd1306_get_pixel (
        struct Framebuffer *fb          //< framebuffer to write the pixel to
,           uint32_t const  x           //< pixel x position
,           uint32_t const  y           //< pixel y position
,           uint32_t       *pixel_value //< value of the pixel
){
        Assert( fb            != NULL );
        Assert( fb->user_data != NULL );
        
        struct Framebuffer_SSD1306 *fb_ssd1306 = (struct Framebuffer_SSD1306 *)fb->user_data;
        
        Assert( fb_ssd1306->tiles );
        
        enum status_code  status  ; // return value of an operation
                uint32_t  tile_idx; // tile the pixel is in
                 uint8_t  bit_idx ; // bit index of the pixel within a tile
        
        status = get_framebuffer_indizes( fb_ssd1306, x, y, &tile_idx, &bit_idx );
        if( status != STATUS_OK ){
                *pixel_value = 0x00;
        } else {
                *pixel_value = ( fb_ssd1306->tiles[ tile_idx ] & ( 0x1 << bit_idx ) ) >> bit_idx;        // current pixel value
        }

        return status;
};

/**
 * \brief Set pixel_value for the pixel at [x,y] in framebuffer
 *
 * \asserts ( fb            != NULL )
 * \asserts ( fb->user_data != NULL );
 */
static enum status_code framebuffer_ssd1306_set_pixel (
        struct Framebuffer *fb          //< framebuffer to write the pixel to
,           uint32_t const  x           //< pixel x position
,           uint32_t const  y           //< pixel y position
,           uint32_t        pixel_value //< value to set for the pixel
){
        Assert( fb            != NULL );
        Assert( fb->user_data != NULL );
        
        struct Framebuffer_SSD1306 *fb_ssd1306 = (struct Framebuffer_SSD1306 *)fb->user_data;
        
        Assert( fb_ssd1306->tiles );

                 uint8_t  bit_idx     ; // bit index of the pixel within a tile
                uint32_t  curr_pix_val; // current pixel value
        enum status_code  status      ; // return value of an operation
                uint32_t  tile_idx    ; // tile the pixel is in
        
        status = get_framebuffer_indizes( fb_ssd1306, x, y, &tile_idx, &bit_idx );
        if( status != STATUS_OK ){
                goto done;
        }
         
        status = framebuffer_ssd1306_get_pixel( fb, x, y, &curr_pix_val );
        if( status != STATUS_OK ){
                goto done;
        }
                       
        // set pixel value
        if( curr_pix_val == pixel_value ){
                status = STATUS_OK;  // nothing to do
                goto done;
        };

        if( pixel_value > 0x00 ){
                fb_ssd1306->tiles[ tile_idx ] |=  ( 0x1 << bit_idx ); // set pixel
        } else {
                fb_ssd1306->tiles[ tile_idx ] &= ~( 0x1 << bit_idx ); // clear pixel
        };
        
        // update dirty tiles index and count
        uint16_t  tiles_dirty_byte_idx = tile_idx >> 3;                                                  // 1 bit per tile in tiles_dirty
         uint8_t  tiles_dirty_bit_idx  = tile_idx % (tiles_dirty_byte_idx << 3);                         // bit position of tile in tiles_dirty index byte
            bool  tile_is_dirty        = ( fb_ssd1306->tiles_dirty[ tiles_dirty_byte_idx ] & ( 0x1 << tiles_dirty_bit_idx ) ) >> tiles_dirty_bit_idx; // current value
        
        if( !tile_is_dirty ){ // tile not already marked as dirty
                fb_ssd1306->tiles_dirty[ tiles_dirty_byte_idx ] |= ( 0x1 << tiles_dirty_bit_idx );      // mark tile as dirty
                fb_ssd1306->tiles_dirty_count++;                                                        // increase dirty tiles count
        }
        
        status = STATUS_OK;
        
done:
        return status;
};

// ===========================================================================
//  public
// ===========================================================================

/**
 * \brief Create a new framebuffer instance for a SSD1306 OLED controlled display
 * 
 * \asserts height > 0
 * \asserts width  > 0 
 */
struct Framebuffer *framebuffer_SSD1306_create ( 
    uint32_t width      //< framebuffer width  in pixel
,   uint32_t height     //< framebuffer height in pixel
){
        Assert( height > 0 );
        Assert( width  > 0 );
        
        struct Framebuffer *fb;
        enum   status_code  status;
        
        fb = (struct Framebuffer *) malloc( sizeof( struct Framebuffer ) );     // create a new Framebuffer instance
        if( fb == NULL ){
                goto done;
        }                
        fb->width     = width ;
        fb->height    = height;
        fb->clear     = &framebuffer_ssd1306_clear    ;
        fb->destroy   = &framebuffer_ssd1306_destroy  ;
        fb->get_pixel = &framebuffer_ssd1306_get_pixel;
        fb->set_pixel = &framebuffer_ssd1306_set_pixel;
        
        struct Framebuffer_SSD1306 *fb_SSD1306;
        
        fb_SSD1306 = (struct Framebuffer_SSD1306 *) malloc( sizeof ( struct Framebuffer_SSD1306 ) );    // create a new Framebuffer_SSD1306 instance
        if ( fb_SSD1306 == NULL ){
                framebuffer_ssd1306_destroy( fb );
                goto done;
        }
        fb->user_data = fb_SSD1306;
        
        status = framebuffer_ssd1306_init( fb_SSD1306, width, height );
        if( status != STATUS_OK ){
                framebuffer_ssd1306_destroy( fb );
                goto done;
        }
        
done:
        return fb;
};
