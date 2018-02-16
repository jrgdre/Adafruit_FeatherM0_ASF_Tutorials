/**	\file	Draw_Line_TestPattern.h

 	\brief	Declarations of a graphic drawing functions to draw a test-pattern using the draw_line function.

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
#ifndef DRAW_LINE_TESTPATTERN_H
#define DRAW_LINE_TESTPATTERN_H

#include <asf.h>

/**
 *	Draw a test pattern using the \ref draw_line() function.
 *
 *	Use this function to check the correct implementatiion of the draw_line() function.
 *
 *	\note	Lines are drawn to all sectors from [\ref x0, \ref yo], 
 *			filling the rectangle from [\ref xMin, \ref yMin] to [\ref xMax, \ref yMax].
 *
 *	\note	If a reference to the i2c_master_module, the OLED is connected to, is provided,
 *          the display will be updated after each line drawn.
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                   If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG		If \ref framebuffer_draw_pixel or \ref framebuffer is not assigned
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
);

#endif
