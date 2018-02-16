/**	\file	Draw_Line_TestPattern.c

 	\brief	Implementation of a graphic drawing functions to draw a test-pattern using the \ref draw_line() function.

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

#include "Adafruit_FeatherWing_OLED.h"
#include "Draw.h"
#include "Draw_Line_TestPattern.h"


/**
 *	\brief	Draws a test pattern to the screen at the location specified,
 *			using the draw_line() function in all sectors.
 */
enum status_code draw_line_testpattern(
	    void *framebuffer_draw_pixel	//< pointer to a function that knows how to draw a pixel to the framebuffer
,	    void *framebuffer				//< pointer to the framebuffer to draw the pixel to
,	uint32_t  pixel_value				//< value to set for the pixels
,	uint16_t  x0						//< x start position of the lines
,	uint16_t  y0						//< y start position of the lines
,	uint16_t  xMin						//< x end   position of the line
,	uint16_t  yMin						//< y end   position of the line
,	uint16_t  xMax						//< x end   position of the line
,	uint16_t  yMax						//< y end   position of the line
){
 	enum status_code	status	= STATUS_OK;
 			 int32_t    x		= 0;
 			 int32_t    y		= 0;
 
 	// sector 0
 	for( y = y0; y <= yMax; y++ ) {
 		
 		status = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value,   x0,  y0, xMax,  y)
			  || featherWing_OLED_update( framebuffer );
			  
 		if( status != STATUS_OK ){
	 		return status;
 		}
			  
 	}
 
 	// sector 1
 	for( x = xMax; x >= x0 ; x-- ) {
 		
 		status = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value,   x0,  y0, x,  yMax)
			  || featherWing_OLED_update( framebuffer );
			  
 		if( status != STATUS_OK ){
	 		return status;
 		}
 		
 	}
 
 	// sector 2
 	for( x = x0; x >= xMin; x-- ) {
 		
 		status = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value,   x0,  y0, x,  yMax)
			  || featherWing_OLED_update( framebuffer );
			  
 		if( status != STATUS_OK ){
	 		return status;
 		}
 		
 	}
 
 	// sector 3
 	for( y = yMax; y >= y0; y-- ) {
 		
 		status = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value,   x0,  y0, xMin,  y)
			  || featherWing_OLED_update( framebuffer );
			  
 		if( status != STATUS_OK ){
	 		return status;
 		}
 		
 	}
 
 	// sector 4
 	for( y = y0; y >= yMin; y-- ) {
 		
 		status = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value,   x0,  y0, xMin,  y)
 			  || featherWing_OLED_update( framebuffer );
			   
 		if( status != STATUS_OK ){
	 		return status;
 		}
 		
 	}
 
 	// sector 5
 	for( x = xMin; x <= x0; x++ ) {
 
 		status = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value,   x0,  y0, x,  yMin)
 			  || featherWing_OLED_update( framebuffer );
			   
 		if( status != STATUS_OK ){
	 		return status;
 		}
 		
 	}
 
 	// sector 6
 	for( x = x0; x <= xMax; x++ ) {
 		
 		status = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value,   x0,  y0, x,  yMin)
 			  || featherWing_OLED_update( framebuffer );
			   
 		if( status != STATUS_OK ){
	 		return status;
 		}
 		
 	}
 
 	// sector 7
 	for( y = yMin; y <= y0; y++ ) {
 		
 		status = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value,   x0,  y0, xMax,  y)
 			  || featherWing_OLED_update( framebuffer );
			   
 		if( status != STATUS_OK ){
	 		return status;
 		}
 		
 	}
 
 	return status;
}
