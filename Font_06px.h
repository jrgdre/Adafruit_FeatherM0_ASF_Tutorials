/**	\file	Font_06px.h

 	\brief	A very tiny 6 pixel height font.

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
#ifndef FONT_06PX_H
#define FONT_06PX_H

#include <asf.h>

/**
 *	Draw the bitmap of a single character to the \ref framebuffer
 *	using \ref pixel_value starting with the upper left corner at [\ref x0,\ref y0].
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                   If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG		If \ref framebuffer_draw_pixel or \ref framebuffer is not assigned
 */
uint8_t font_06px_draw_character(
      void *framebuffer_draw_pixel	//< pointer to a function that knows how to draw a pixel to the framebuffer
,     void *framebuffer				//< pointer to the framebuffer to draw the bitmap to
,     char  character				//< character to draw
, uint32_t  pixel_value				//< value to set for the pixels
, uint16_t  x0						//< x start position of character bitmap in framebuffer
, uint16_t  y0						//< y start position of character bitmap in framebuffer
,  uint8_t	font_background			//< how to draw font background
,  uint8_t *char_width				//< [out] width of character drawn in pixel
);

/**
 *	Draw the bitmap of a string of characters to the \ref framebuffer
 *	using \ref pixel_value starting with the upper left corner at [\ref x0,\ref y0].
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                   If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG		If \ref framebuffer_draw_pixel, \ref framebuffer or \ref string is not assigned
 */
uint8_t font_06px_draw_string(
	    void *framebuffer_draw_pixel	//< pointer to a function that knows how to draw a pixel to the framebuffer
,       void *framebuffer				//< pointer to the framebuffer to draw the bitmap to
, const char *string					//< string to draw
,   uint32_t  pixel_value				//< value to set for the pixels
,   uint16_t  x0						//< x start position of character bitmap in framebuffer
,   uint16_t  y0						//< y start position of character bitmap in framebuffer
,    uint8_t  font_background			//< how to draw font background
,    uint8_t *string_width				//< [out] width of character drawn in pixel
);

#endif
