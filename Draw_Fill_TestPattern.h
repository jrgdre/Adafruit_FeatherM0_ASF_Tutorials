/**	\file	Draw_Fill_TestPattern.h

 	\brief	Declarations of a graphic drawing functions to draw a test-pattern for the fill operation.

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
#ifndef DRAW_FILL_TESTPATTERN_H
#define DRAW_FILL_TESTPATTERN_H

#include <asf.h>

/**
 *	Draws a fill test pattern to the screen location specified.
 *
 *	\note	The pattern is open to the x0 and the y0 sides, to allow border testing.
 *	\note	Size of the pattern is width = 30px, height = 16px.
 *	\note	The start point for draw_fill should be set to the middle of the pattern, e.g to [x=x0+14,y=y0+7].
 *
 *                               #
 *	 ############# ###### ###### #
 *	 #           # #           # #
 *   # #### #### # # ######### # #
 *	 # ##      # # # #      ## # #
 *	 # # #   # # # # # #   # # # #
 *	 # #  # #  # # # #  # #  # # #
 *	####   #   # #x# #   #   #####
 *	 # #  # #  # # # #  # #  # # #
 *	 # # #   # # # # # #   # # # #
 *	 # ##      # # # #      ## # #
 *	 # ######### # # #### #### # #
 *	 #           # #           # #
 *	 ###### ###### ############# #
 *	##############################
 *
 *	\return Status of operation.
 *	\retval STATUS_OK                   If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG		If \ref framebuffer_draw_pixel or \ref framebuffer is not assigned
 */
enum status_code draw_fill_testpattern(
      void *framebuffer_draw_pixel	//< pointer to a function that knows how to draw a pixel to the framebuffer
,     void *framebuffer				//< pointer to the framebuffer to draw the pixel to
, uint32_t  pixel_value				//< value to set for the pixels
, uint16_t  x0						//< x start position of the pattern (upper left corner)
, uint16_t  y0						//< y start position of the pattern (upper left corner)
);

#endif
