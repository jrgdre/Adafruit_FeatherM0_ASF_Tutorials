/**	\file	Framebuffer_1bit.h

 	\brief	Implementation of a 1bit per pixel framebuffer
 
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
#include <string.h>				// memory functions
#include "Framebuffer_1bit.h"

/************************************************************************/
/* internal functions                                                   */
/************************************************************************/

/**
 *	Debug helper function that draws a test-pattern to the framebuffer 
 *	without the need for calculating any positions, etc.
 */
static uint8_t draw_testpattern(
      void *framebuffer	//< framebuffer to write the pixel to
){
	if( framebuffer == NULL ) {
		return STATUS_ERR_INVALID_ARG;
	}
	framebuffer_t *fb;
	fb = (framebuffer_t *)framebuffer;
	
	fb->tiles[0x00] = 0b00000001;  // page 0, col 0
	fb->tiles[0x01] = 0b00000010;
	fb->tiles[0x02] = 0b00000100;
	fb->tiles[0x03] = 0b00001000;
	fb->tiles[0x04] = 0b00010000;
	fb->tiles[0x05] = 0b00100000;
	fb->tiles[0x06] = 0b01000000;
	fb->tiles[0x07] = 0b10000000;
	fb->tiles[0x08] = 0b00000001;
	fb->tiles[0x09] = 0b00000011;
	fb->tiles[0x0A] = 0b00000111;
	fb->tiles[0x0B] = 0b00001111;
	fb->tiles[0x0C] = 0b00011111;
	fb->tiles[0x0D] = 0b00111111;
	fb->tiles[0x0E] = 0b01111111;
	fb->tiles[0x0F] = 0b11111111;  // page 0, col 15
	
	return STATUS_OK;
};

/**
 *	Get the tile and bit index for for a pixel in the framebuffer 
 *  based on the x,y coordinates of that pixel.
  *
  *	\return Status of operation.
  *	\retval STATUS_OK                   If operation was successfully
  *	\retval STATUS_ERR_INVALID_ARG		If \ref x or \ref y out of bounds
*/
static uint8_t get_framebuffer_indizes(
  framebuffer_t  const framebuffer	//< framebuffer to write the pixel to
,      uint16_t  const x			//< [in] pixel x position
,      uint16_t  const y			//< [in] pixel y position
,      uint32_t *tile_idx			//< [out] index of the tile the pixel is in
, 	    uint8_t *bit_idx			//< [out] index of bit in the tile the pixel is in
){
	if( ( tile_idx == NULL ) || ( bit_idx == NULL ) ){
		return STATUS_ERR_INVALID_ARG;
	}

	uint16_t  page_idx  = y / ( sizeof( framebuffer_tile_t ) << 3 );	// index of page we have to draw pixel to
		     *bit_idx	= y % ( sizeof( framebuffer_tile_t ) << 3 );	// index of bit in page we have to draw pixel to
			 *tile_idx  = ( page_idx * framebuffer.columns ) + x;		// tile the pixel is in

	return STATUS_OK;	
}

/************************************************************************/
/* public functions                                                     */
/************************************************************************/

/**
 *	\brief Initialize a framebuffer structure
 */
uint8_t framebuffer_init(
  framebuffer_t *framebuffer	//< framebuffer structure to initialize
,      uint16_t  display_width	//< width of the display the buffer is intended for in pixels
,      uint16_t  display_height	//< height of the display the buffer is intended for in pixels
){
	framebuffer->columns			= display_width;											// number of columns, depending on display width
	framebuffer->bits_per_tile		= sizeof( framebuffer_tile_t ) << 3;						// in bits
	framebuffer->pages				= display_height / framebuffer->bits_per_tile;				// number of pages, depending on display height and tile height
	framebuffer->bytes				= framebuffer->columns * framebuffer->pages;				// overall number of bytes for the framebuffer
	framebuffer->tiles				= (framebuffer_tile_t *)malloc( framebuffer->bytes      );	// allocate a new framebuffer bitmap (1 bit per pixel)
	framebuffer->tiles_dirty		= (           uint8_t *)malloc( framebuffer->bytes >> 3 );	// allocate a new dirty tiles index  (1 bit per tile in an 8 bit bitmap)
	framebuffer->tiles_dirty_count	= 0;
	
	if(    ( framebuffer->tiles == NULL ) 
        || ( framebuffer->tiles_dirty == NULL ) 
	){
		return STATUS_ERR_NO_MEMORY;
	}
	
	return STATUS_OK;
};

/**
 * \brief Set every pixel in framebuffer to 0x0
 */
void framebuffer_clear(
	void *framebuffer	//< framebuffer to clear
){
	if( framebuffer == NULL ) {
		return;
	}
	framebuffer_t *fb;
	fb = (framebuffer_t *)framebuffer;

	memset( (void *)fb->tiles      , 0x00, fb->bytes      );	// reset all pixel to 0x00
	memset( (void *)fb->tiles_dirty, 0xFF, fb->bytes >> 3 );	// mark all tiles as dirty
	fb->tiles_dirty_count = fb->bytes;							// set the dirty tiles count to the number of tiles
};

/**
 * \brief Set pixel_value for the pixel at [x,y] in framebuffer
 */
uint8_t framebuffer_draw_pixel(
      void *framebuffer	//< framebuffer to write the pixel to
, uint16_t  const x		//< pixel x position
, uint16_t  const y		//< pixel y position
, uint32_t  pixel_value	//< value to set for the pixel
){
	if( framebuffer == NULL ) {
		return STATUS_ERR_INVALID_ARG;
	}

// 	draw_testpattern(framebuffer);	return STATUS_OK;	// draw a test-pattern and return (use this only if something is really messed up)
	(void)draw_testpattern;	// create a function pointer to suppress the unused warning (this does NOT run the draw_testpattern code!)
	
	framebuffer_t *fb;
	fb = (framebuffer_t *)framebuffer;
	
	// make sure pixel is in bounds
	if(    ( x >= fb->columns ) 
		|| ( y >= fb->pages * fb->bits_per_tile )
	){
		return STATUS_ERR_INVALID_ARG;
	};
	
	// pixel position and status in the framebuffer
	uint16_t page_idx     = y / ( sizeof( framebuffer_tile_t ) << 3 );						// index of page we have to draw pixel to
	uint16_t y_bit_idx    = y % ( sizeof( framebuffer_tile_t ) << 3 );						// index of bit in page we have to draw pixel to
	uint32_t tile_idx     = ( page_idx * fb->columns ) + x;									// tile the pixel is in

	uint8_t  curr_pix_val = ( fb->tiles[ tile_idx ] & ( 0x1 << y_bit_idx ) ) >> y_bit_idx;	// current pixel value

	// set pixel value
	if( curr_pix_val == pixel_value ){
		return STATUS_OK;  // nothing to do
	};

	if( pixel_value > 0x00 ){
		fb->tiles[ tile_idx ] |=  ( 0x1 << y_bit_idx ); // set pixel
	} else {
		fb->tiles[ tile_idx ] &= ~( 0x1 << y_bit_idx ); // clear pixel
	};
	
	// update dirty tiles index and count
	uint16_t tiles_dirty_byte_idx = tile_idx >> 3;														// 1 bit per tile in tiles_dirty
	uint8_t  tiles_dirty_bit_idx  = tile_idx % (tiles_dirty_byte_idx << 3);								// bit position of tile in tiles_dirty index byte
	bool	 tile_is_dirty		  = ( fb->tiles_dirty[ tiles_dirty_byte_idx ] & ( 0x1 << tiles_dirty_bit_idx ) ) >> tiles_dirty_bit_idx; // current value
	
	if( !tile_is_dirty ){ // tile not already marked as dirty
		fb->tiles_dirty[ tiles_dirty_byte_idx ] |= ( 0x1 << tiles_dirty_bit_idx );	// mark tile as dirty
		fb->tiles_dirty_count++;													// increase dirty tiles count
	}
	
	return STATUS_OK;
}

/**
 * \brief Get pixel_value for the pixel at [x,y] in framebuffer
 */
uint8_t framebuffer_get_pixel(
      void *framebuffer	//< framebuffer to write the pixel to
, uint16_t  const x		//< pixel x position
, uint16_t  const y		//< pixel y position
, uint32_t *pixel_value	//< value of the pixel
){
	framebuffer_t *fb;
	fb = (framebuffer_t *)framebuffer;
	
	// make sure pixel is in bounds
	if(    ( x >= fb->columns )
		|| ( y >= fb->pages * fb->bits_per_tile )
	){
		return STATUS_ERR_INVALID_ARG;
	};
	
	// pixel position and status in the framebuffer
	uint8_t  ret     ;	// return value of an operation
	uint32_t tile_idx;	// tile the pixel is in
	uint8_t  bit_idx ;	// bit index of the pixel within a tile
	
	ret = get_framebuffer_indizes( *fb, x, y, &tile_idx, &bit_idx );
	if( ret != STATUS_OK ){
		return ret;
	}
	
	*pixel_value = ( fb->tiles[ tile_idx ] & ( 0x1 << bit_idx ) ) >> bit_idx;	// current pixel value
	
	return STATUS_OK;
};
