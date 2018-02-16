/**	\file	Framebuffer_1bit.h

 	\brief	Declarations for a 1bit per pixel framebuffer
 
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
#ifndef FRAMEBUFFER_1BIT_H
#define FRAMEBUFFER_1BIT_H

#include <asf.h>	// Its an Atmel Software Foundation project

typedef uint8_t framebuffer_tile_t; //< framebuffer is formed by bit-blocks -> a.k.a. "tiles", 8 pixels (=bits) per tile

typedef enum Framebuffer_Pixel_Mode {
	PIXEL_CLEAR			= 0x00,	//< set pixel's value to 0
	PIXEL_SET			= 0x01,	//< set pixel's value to 1
	PIXEL_TRANSPARENT	= 0x02	//< transparent mode, if pixel_value = 1 set pixel to 1, if pixel_value = 0 keep current value of pixel
} framebuffer_pixel_mode_t;

/**
 *	\brief Data structure for managing a framebuffer.
 *
 *	\ref tiles
 *  - framebuffer is formed by bit-blocks (usually 8 bits wide) -> a.k.a. "tiles"
 *	- tiles side-by-side form a page (FeatherWing_OLED: 128 tiles per page)
 *	- one ore more pages stacked on top of each other form the display (FeatherWing_OLED: 4 pages)
 *
 *	\ref tiles_dirty
 *	In one bit per tile, we remember the tiles that have been changed in the framebuffer 
 *	but not send to the display yet (are dirty), to later be able to optimize the screen update.
 *
 *	\ref tiles_dirty_count
 *	Updating a tile in random access mode requires:
 *	- sending the page   start/end address command (5 bytes on the wire: i2c address + 3 byte command + getting 1 byte acknowledge back)
 *	- sending the column start/end address command (5 bytes on the wire: i2c address + 3 byte command + getting 1 byte acknowledge back)
 *	- sending the data                             (4 bytes on the wire: i2c address + 2 byte command + getting 1 byte acknowledge back)
 *	- That's 14 bytes in total.
 *	.
 *	Sending the whole framebuffer of e.g. the FeatherWing_OLED (512 tiles) over requires:
 *	- sending the page   start/end address command (  1 x 5 bytes on the wire: i2c address + 3 byte command + getting 1 byte acknowledge back)
 *	- sending the column start/end address command (  1 x 5 bytes on the wire: i2c address + 3 byte command + getting 1 byte acknowledge back)
 *	- sending the data                             (512 x 4 bytes on the wire: i2c address + 2 byte command + getting 1 byte acknowledge back)
 *	- That's 2058 bytes in total.
 *	.
 *	So for the FeatherWing_OLED it is faster to send the whole framebuffer over if more than 147 tiles have to be updated (are dirty).
 *	This threshold depends on the number of tiles the display area is made of.
 *	.
 *	To make the decision which way to go fast and effortless we count the number of tiles in \ref tiles_dirty_count.
 */
typedef struct Framebuffer {
			  uint16_t  columns;			//< number of columns (tiles side-by-side in one page)
			  uint16_t  pages;				//< number of pages (tiles stacked on top of each other)
			  uint32_t	bytes;				//< size of this framebuffer in bytes
			   uint8_t	bits_per_tile;		//< number of bits that make up a tile
	framebuffer_tile_t *tiles;			    //< this is the pixmap that we draw to and send to the display eventually
			   uint8_t *tiles_dirty;	    //< 1 bit per tile,  set if tile was changed
			  uint32_t  tiles_dirty_count;	//< number of dirty tiles
} framebuffer_t;						

/**
 *	This function 
 *	- allocates new memory for tiles 
 *	- allocates new memory for tiles_dirty
 *	- initializes the fields with the default values
 *	for the display parameters given.
 *
 *	\note	New memory is allocated for \ref tiles and \ref tiles_dirty on each call.
 *			This allows for multiple buffers to be used at the same time 
 *			(e.g. for double buffer or multi display applications).
 *			But it also means, that you loose access to the memory allocated before,
 *			if you call this function more than once for the same framebuffer variable,
 *			possibly creating memory leaks.
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                   If initialization was successfully
 *	\retval STATUS_ERR_NO_MEMORY		If there was not enough memory left to allocate one of the buffers
 */
uint8_t framebuffer_init(
  framebuffer_t *framebuffer	//< framebuffer structure to initialize
,      uint16_t  display_width	//< width of the display the buffer is intended for in pixels
,      uint16_t  display_height	//< height of the display the buffer is intended for in pixels
);

/**
 *	The function resets the whole framebuffer to 0x00, clearing it's content.
 *
 *	After that all the tiles are marked as dirty.
 */
void framebuffer_clear(
	void *framebuffer	//< framebuffer to clear
);

/**
 *	This function knows how to write a pixel to the framebuffer.
 *
 *	It updates the \ref tiles_dirty index and the \ref tiles_diryt_count.
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                   If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG		If \ref framebuffer not assigned or \ref x or \ref y out of bounds
 */
uint8_t framebuffer_draw_pixel(
      void *framebuffer	//< framebuffer to write the pixel to
, uint16_t  const x		//< pixel x position
, uint16_t  const y		//< pixel y position
, uint32_t  pixel_value	//< value to set for the pixel
);

/**
 *	This function knows how to get the value of a pixel from the framebuffer.
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                   If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG		If \ref framebuffer not assigned or \ref x or \ref y out of bounds
 */
uint8_t framebuffer_get_pixel(
      void *framebuffer	//< framebuffer to write the pixel to
, uint16_t  const x		//< pixel x position
, uint16_t  const y		//< pixel y position
, uint32_t *pixel_value	//< value of the pixel
);

#endif