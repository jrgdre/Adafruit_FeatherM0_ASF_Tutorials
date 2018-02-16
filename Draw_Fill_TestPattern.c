/**	\file	Draw_Fill_TestPattern.c

 	\brief	Implementation of a graphic drawing functions to draw a test-pattern for the fill operation.

	\note	All functions draw to the \ref framebuffer specified using the \ref framebuffer_draw_pixel function specified.

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
 		1.0.0: 2017-06-29 jrgdre initial release

 */

#include "Draw.h"
#include "Draw_Fill_TestPattern.h"

/**
 *	\brief	Draws a fill test pattern to the screen location specified.
 */
enum status_code draw_fill_testpattern(
      void *framebuffer_draw_pixel	//< pointer to a function that knows how to draw a pixel to the framebuffer
,     void *framebuffer				//< pointer to the framebuffer to draw the pixel to
, uint32_t  pixel_value				//< value to set for the pixels
, uint16_t  x0						//< x start position of the pattern (upper left corner)
, uint16_t  y0						//< y start position of the pattern (upper left corner)
){
	if(    ( framebuffer_draw_pixel == NULL )
		|| ( framebuffer            == NULL )
	){
		return STATUS_ERR_INVALID_ARG;
	}
	
	return 
		// outer box open to screen border
		draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 29, y0 +  0, x0 + 29, y0 + 15 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 29, y0 + 15, x0 +  0, y0 + 15 )
	
		// left outer box
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 +  6, y0 + 13, x0 +  1, y0 + 13 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 +  1, y0 + 13, x0 +  1, y0 +  1 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 +  1, y0 +  1, x0 + 13, y0 +  1 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 13, y0 +  1, x0 + 13, y0 + 13 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 13, y0 + 13, x0 +  8, y0 + 13 )
	
		// left inner box
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 +  8, y0 +  3, x0 + 11, y0 +  3 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 11, y0 +  3, x0 + 11, y0 + 11 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 11, y0 + 11, x0 +  3, y0 + 11 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 +  3, y0 + 11, x0 +  3, y0 +  3 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 +  3, y0 +  3, x0 +  6, y0 +  3 )
	
		// left cross
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 +  4, y0 +  4, x0 +  9, y0 +  9 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 +  4, y0 + 10, x0 +  9, y0 +  5 )
	
		// left wall
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 +  0, y0 +  7, x0 +  2, y0 +  7 )
	
		// right outer box
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 22, y0 +  1, x0 + 27, y0 +  1 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 27, y0 +  1, x0 + 27, y0 + 13 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 27, y0 + 13, x0 + 15, y0 + 13 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 15, y0 + 13, x0 + 15, y0 +  1 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 15, y0 +  1, x0 + 20, y0 +  1 )
	
		// right inner box
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 20, y0 + 11, x0 + 17, y0 + 11 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 17, y0 + 11, x0 + 17, y0 +  3 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 17, y0 +  3, x0 + 25, y0 +  3 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 25, y0 +  3, x0 + 25, y0 + 11 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 25, y0 + 11, x0 + 22, y0 + 11 )
	
		// right cross
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 19, y0 +  5, x0 + 24, y0 + 10 )
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 19, y0 +  9, x0 + 24, y0 +  4 )
	
		// right wall
	||	draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0 + 26, y0 +  7, x0 + 28, y0 +  7 );
	
}
