/**	\file	Draw.h

 	\brief	Declarations for a library of graphic drawing functions.

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
#ifndef DRAW_H
#define DRAW_H

#include <asf.h>	                // This is an Atmel Software Foundation implementation
#include "Framebuffer.h"

/**
 *	Implements the midpoint algorithm 
 *	to set \ref pixel_value on a circle of pixels with midpoint [\ref x0,\ref y0] and \ref radius.
 *
 *	\see https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 *
 *	\return Status of operation.
 *	\retval STATUS_OK               If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG  If \ref framebuffer is not assigned
 */
uint8_t draw_circle(
  struct Framebuffer *framebuffer       //< pointer to the framebuffer to draw the pixel to
,           uint32_t  pixel_value	//< value to set for the pixels
,           uint16_t  x0		//< x start position of the line
,           uint16_t  y0		//< y start position of the line
,           uint16_t  radius            //< radius in pixel to draw circle with
);

/**
 *	Draw an ellipse inside the rectangle specified by [x0,y0], [x1,y1].
 *
 *	\see http://members.chello.at/~easyfilter/bresenham.html
 *
 *	\return Status of operation.
 *	\retval STATUS_OK               If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG	If \ref framebuffer is not assigned
 */
uint8_t draw_ellipse_rect(
  struct Framebuffer *framebuffer       //< pointer to the framebuffer to draw the pixel to
,           uint32_t  pixel_value	//< value to set for the pixels
,           uint16_t  x0		//< x start position of the enclosing rectangle
,           uint16_t  y0		//< y start position of the enclosing rectangle
,           uint16_t  x1		//< x end   position of the enclosing rectangle
,           uint16_t  y1		//< y end   position of the enclosing rectangle
);

/**
 *	Flood fill an area starting from point [x,y] with pixel_value_new.
 *
 *	\return Status of operation.
 *	\retval STATUS_OK               If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG	If \ref framebuffer is not assigned.
 *					Start point [x,y] is out of screen bounds.
 *	\retval STATUS_ERR_NO_MEMORY	If stack could not be allocated.
 */
uint8_t draw_fill(
  struct Framebuffer *framebuffer	//< pointer to the framebuffer to draw the pixel to
,           uint32_t  pixel_value	//< current value of pixels to change
,           uint32_t  pixel_value_new   //< new value pixels are set to
,           uint16_t  x			//< x start point
,           uint16_t  y			//< y start point
);

/**
 *	Implements Bresenham's line algorithm 
 *	to set \ref pixel_value for a line of pixels form [\ref x0,\ref y0] to [\ref x1,\ref y1].
 *
 *	\see http://members.chello.at/~easyfilter/bresenham.html
 *
 *	\return Status of operation.
 *	\retval STATUS_OK               If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG	If \ref framebuffer is not assigned
 */
uint8_t draw_line(
  struct Framebuffer *framebuffer       //< pointer to the framebuffer to draw the pixel to
,           uint32_t  pixel_value	//< value to set for the pixels
,           uint16_t  x0		//< x start position of the line
,           uint16_t  y0		//< y start position of the line
,           uint16_t  x1		//< x end   position of the line
,           uint16_t  y1		//< y end   position of the line
);

/**
 *	Draw a rectangle form [\ref x0,\ref y0] to [\ref x1,\ref y1]
 *	using \ref draw_line.
 *
 *	\return Status of operation.
 *	\retval STATUS_OK               If operation was successfully
 *	\retval STATUS_ERR_INVALID_ARG	If \ref framebuffer is not assigned
 */
uint8_t draw_rect(
  struct Framebuffer *framebuffer       //< pointer to the framebuffer to draw the pixel to
,           uint32_t  pixel_value	//< value to set for the pixels
,           uint16_t  x0		//< x start position of the line
,           uint16_t  y0		//< y start position of the line
,           uint16_t  x1		//< x end   position of the line
,           uint16_t  y1		//< y end   position of the line
);

#endif