/**	\file	Draw.h

 	\brief	Declaration of framebuffer access functions.

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
 		1.0.0: 2017-06-22 jrgdre initial release

 */
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/**
 *	Prototype of a pointer to a function that understands how to:
 *	- set \ref pixel_value
 *	- at position \ref x, \ref y
 *	- in \ref framebuffer
 */
typedef uint8_t ( *Framebuffer_Draw_Pixel )( 
      void *framebuffer	//< framebuffer to modify
, uint16_t  x			//< x position of pixel to change
, uint16_t  y			//< y position of pixel to change
, uint32_t  pixel_value	//< value to set pixel to
);

/**
 *	Prototype of a pointer to a function that understands how to:
 *	- get \ref pixel_value
 *	- of pixel at position \ref x, \ref y
 *	- in \ref framebuffer
 */
typedef uint8_t ( *Framebuffer_Get_Pixel )( 
      void *framebuffer	//< framebuffer to modify
, uint16_t  x			//< x position of pixel to change
, uint16_t  y			//< y position of pixel to change
, uint32_t *pixel_value	//< value of pixel at [x,y]
);

#endif
