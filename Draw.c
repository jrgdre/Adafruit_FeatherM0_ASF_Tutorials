/**	\file	draw.h

 	\brief	Implementation of a library of graphic drawing functions.

	\note	All functions draw to the \ref framebuffer specified using the \ref framebuffer_draw_pixel function specified.
			So the implementation is independent of the actual \ref framebuffer properties.

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
#include "Draw.h"
#include "Framebuffer.h"
#include "Stack.h"

/**
 *	\brief Draw a circle around [x0,y0] with radius
 */
uint8_t draw_circle(
      void *framebuffer_draw_pixel	//< pointer to a function that knows how to draw a pixel to the framebuffer
,     void *framebuffer				//< pointer to the framebuffer to draw the pixel to
, uint32_t  pixel_value				//< value to set for the pixels
, uint16_t  x0						//< x start position of the line
, uint16_t  y0						//< y start position of the line
, uint16_t  radius					//< radius in pixel to draw circle with
){
	if(    ( framebuffer_draw_pixel == NULL )
		|| ( framebuffer == NULL )
	){
		return STATUS_ERR_INVALID_ARG;
	}
	
	Framebuffer_Draw_Pixel draw_pixel = (Framebuffer_Draw_Pixel)framebuffer_draw_pixel;
	
    int x	= radius;
    int y	= 0;
    int err = 0;

    while ( x >= y )
    {
	    draw_pixel( framebuffer, x0 + x, y0 + y, pixel_value );
	    draw_pixel( framebuffer, x0 + y, y0 + x, pixel_value );
	    draw_pixel( framebuffer, x0 - y, y0 + x, pixel_value );
	    draw_pixel( framebuffer, x0 - x, y0 + y, pixel_value );
	    draw_pixel( framebuffer, x0 - x, y0 - y, pixel_value );
	    draw_pixel( framebuffer, x0 - y, y0 - x, pixel_value );
	    draw_pixel( framebuffer, x0 + y, y0 - x, pixel_value );
	    draw_pixel( framebuffer, x0 + x, y0 - y, pixel_value );

	    y += 1;
	    if (err <= 0)
	    {
		    err += ( y << 1 ) + 1;
		} else {
		    x -= 1;
		    err += ( ( y - x ) << 1 ) + 1;
	    }
    }
		
	return STATUS_OK;
}

/**
 *	\brief Draw an ellipse inside the rectangle specified by [x0,y0], [x1,y1]
 */
uint8_t draw_ellipse_rect(
      void *framebuffer_draw_pixel	//< pointer to a function that knows how to draw a pixel to the framebuffer
,     void *framebuffer				//< pointer to the framebuffer to draw the pixel to
, uint32_t  pixel_value				//< value to set for the pixels
, uint16_t  x0						//< x start position of the enclosing rectangle
, uint16_t  y0						//< y start position of the enclosing rectangle
, uint16_t  x1						//< x end   position of the enclosing rectangle
, uint16_t  y1						//< y end   position of the enclosing rectangle
){
	if(    ( framebuffer_draw_pixel == NULL )
	|| ( framebuffer == NULL )
	){
		return STATUS_ERR_INVALID_ARG;
	}
	
	Framebuffer_Draw_Pixel draw_pixel = (Framebuffer_Draw_Pixel)framebuffer_draw_pixel;
	
	// values of diameter
	int  a   = abs( x1 - x0 ), 
		 b   = abs( y1 - y0 ), 
		 b1  = b&1           ; 
	// error increment
	long dx  = ( ( 1  - a ) * b*b ) << 2, 
	     dy  = ( ( b1 + 1 ) * a*a ) << 2; 
	 // error of 1.step
	long err = dx + dy + b1 * a*a,
	     e2;

	if (x0 > x1) { x0 = x1; x1 += a; } // if called with swapped points
	if (y0 > y1) { y0 = y1;          } // .. exchange them 
		
	// starting pixel
	y0 += ( b + 1 ) >> 1; 
	y1  = y0 - b1;   
	a  *= a << 3; 
	b1  = ( b*b ) << 3;

	do {
		draw_pixel( framebuffer, x1, y0, pixel_value); /*   I. Quadrant */
		draw_pixel( framebuffer, x0, y0, pixel_value); /*  II. Quadrant */
		draw_pixel( framebuffer, x0, y1, pixel_value); /* III. Quadrant */
		draw_pixel( framebuffer, x1, y1, pixel_value); /*  IV. Quadrant */
		e2 = err << 1;
		if( e2 <= dy ){ 
			// y step 
			y0++; 
			y1--; 
			err += dy += a; 
		}  
		if( ( e2 >= dx ) || ( err << 1 ) > dy ){ 
			// x step
			x0++; 
			x1--; 
			err += dx += b1; 
		} 
	} while( x0 <= x1 );
	
	while( y0-y1 < b ) {  /* too early stop of flat ellipses a=1 */
		// finish tip of ellipse
		draw_pixel( framebuffer, x0-1, y0  , pixel_value); 
		draw_pixel( framebuffer, x1+1, y0++, pixel_value);
		draw_pixel( framebuffer, x0-1, y1  , pixel_value);
		draw_pixel( framebuffer, x1+1, y1--, pixel_value);
	}
	
	return STATUS_OK;
}

/**
 * \brief	Flood fill an area starting from point [x,y] with pixel_new_value
 */
uint8_t draw_fill(
      void *framebuffer_draw_pixel	//< pointer to a function that knows how to draw a pixel to the framebuffer
,     void *framebuffer_get_pixel	//< pointer to a function that knows how to get a pixel value from the framebuffer
,     void *framebuffer				//< pointer to the framebuffer to draw the pixel to
, uint32_t  pixel_value				//< current value of pixels to change
, uint32_t  pixel_value_new			//< new value pixels are set to
, uint16_t  x						//< x start point
, uint16_t  y						//< y start point
){
	if(    ( framebuffer_draw_pixel == NULL )
		|| ( framebuffer_get_pixel  == NULL )
		|| ( framebuffer == NULL )
	){
		return STATUS_ERR_INVALID_ARG;
	}
	
	Framebuffer_Draw_Pixel draw_pixel = (Framebuffer_Draw_Pixel)framebuffer_draw_pixel;
	Framebuffer_Get_Pixel  get_pixel  = (Framebuffer_Get_Pixel )framebuffer_get_pixel ;

	Stack	*stack; // a (self growing) stack for position of pixel we want to remember
	
	uint8_t  ret		= STATUS_OK,			// status of this function
			 ret_above	= STATUS_OK,			// status of the pixel above test
			 ret_below	= STATUS_OK,			// status of the pixel below test
			 ret_draw	= STATUS_OK;			// status of the draw operation
			 
	uint16_t x_curr		= x,					// current x position
			 y_curr		= y;					// current y position
			 
	uint32_t current_pixel_value	= 0,		// value of the current pixel
	         pixel_value_above		= 0,		// value of the pixel above the current pixel
			 pixel_value_below		= 0;		// value of the pixel below the current pixel
			 
	bool	 push_next_valid_above	= true,		// the next valid pixel above should be pushed to the stack
			 push_next_valid_below	= true;		// the next valid pixel below should be pushed to the stack
	
	// check the validity of the initial position
	ret = get_pixel( framebuffer, x, y, &current_pixel_value);
	if( ret == STATUS_ERR_INVALID_ARG ){
		return ret; // x or y out of bounds
	}
	
	// allocate a stack for x and y values to remember
	ret = stack_allocate( &stack, 1 ); // stack is self re-allocating, so we start with an initial_capcity of one
	if( ret != STATUS_OK ){
		return ret;
	}
	
	do{
		// move left until we find a pixel with a value != pixel_value or reach the end of the screen
		while(    ( current_pixel_value == pixel_value ) // pixel has the correct value
			   && ( ret != STATUS_ERR_INVALID_ARG      ) // not yet reached the end of the screen
		){
			x_curr--;
			ret = get_pixel( framebuffer, x_curr, y_curr, &current_pixel_value);
		}
		// push back to the last pixel valid for fill
		x_curr++;
		ret = get_pixel( framebuffer, x_curr, y_curr, &current_pixel_value);
		
		// now move right until we find a pixel with a value != pixel_value or reach the end of the screen
		while(    ( ret != STATUS_ERR_INVALID_ARG      ) // pixel is within screen limits
		       && ( current_pixel_value == pixel_value ) // pixel has the correct value
		){
			// set the new value for the current pixel
			ret_draw = draw_pixel( framebuffer, x_curr, y_curr, pixel_value_new );
			if( ret_draw != STATUS_OK){
				break; // abort all fill operations
			}
			
			// test the pixel above the current pixel
			ret_above = get_pixel( framebuffer, x_curr, y_curr - 1, &pixel_value_above);
			if(	   ( ret_above == STATUS_OK )			// pixel is within screen limits
				&& ( pixel_value_above == pixel_value ) // pixel as the correct value
			){	// pixel above is valid for fill
				if( push_next_valid_above ){
					// we only push the first pixel in a consecutive row of pixels valid for fill
					stack_push( stack, y_curr - 1 );
					stack_push( stack, x_curr     );
					push_next_valid_above = false;
				}
			} else {
				// pixel above is not valid for fill
				push_next_valid_above = true; // the next valid one above is to be pushed again
			}
			
			// test the pixel below the current pixel
			ret_below = get_pixel( framebuffer, x_curr, y_curr + 1, &pixel_value_below);
			if(    ( ret_below == STATUS_OK )			// pixel is within screen limits
			    && ( pixel_value_below == pixel_value ) // pixel has the correct value
			){	// pixel below is valid for fill
				if( push_next_valid_below ){
					// we only push the first pixel in a consecutive row of pixels valid for fill
					stack_push( stack, y_curr + 1 );
					stack_push( stack, x_curr     );
					push_next_valid_below = false;
				}
			} else {
				// pixel below is not valid for fill
				push_next_valid_below = true; // the next valid one below is to be pushed again
			}
			
			// move to next pixel in line and get it's value
			x_curr++;	
			ret = get_pixel( framebuffer, x_curr, y_curr, &current_pixel_value);
			
		} // end while
		
		if( ret_draw == STATUS_OK ){
			// check stack for more eligible pixels to fill
			ret = stack_pop( stack, &x_curr );
			if( ret != STATUS_OK ){
				break; // looks like we are done
			}
			ret = stack_pop( stack, &y_curr );
			if( ret != STATUS_OK ){
				break; // looks like we are done
			}
			// we found another pixel
			current_pixel_value	  = pixel_value; // that's why we stored it in the first place
			push_next_valid_above = true;		 // the next valid one above is to be pushed again
			push_next_valid_below = true;		 // the next valid one below is to be pushed again
		}
		
	} while( ret_draw == STATUS_OK );
	
	// always deallocate the stack
	ret = stack_deallocate( &stack );
	if( ret != STATUS_OK ){
		return ret;
	}

	return STATUS_OK;
}

/**
 *	\brief Set pixel_value for a line of pixels form [x0,y0] to [x1,y1]
 */
uint8_t draw_line(
      void *framebuffer_draw_pixel	//< pointer to a function that knows how to draw a pixel to the framebuffer
,     void *framebuffer				//< pointer to the framebuffer to draw the pixel to
, uint32_t  pixel_value				//< value to set for the pixels
, uint16_t  x0						//< x start position of the line
, uint16_t  y0						//< y start position of the line
, uint16_t  x1						//< x end   position of the line
, uint16_t  y1						//< y end   position of the line
){
	if(    ( framebuffer_draw_pixel == NULL ) 
		|| ( framebuffer == NULL )
	){
		return STATUS_ERR_INVALID_ARG;
	}

	Framebuffer_Draw_Pixel draw_pixel = (Framebuffer_Draw_Pixel)framebuffer_draw_pixel;
	
	int32_t dx =  abs( x1 - x0 ), sx = x0 < x1 ? 1 : -1;
	int32_t dy = -abs( y1 - y0 ), sy = y0 < y1 ? 1 : -1;
	int32_t err = dx + dy, e2; /* error value e_xy */
   
	for(;;){  /* loop */
		draw_pixel( framebuffer, x0, y0, pixel_value );
		if ( x0 == x1 && y0 == y1 ) break;
		e2 = err << 1;
		if( e2 >= dy ) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if( e2 <= dx ) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
	
	return STATUS_OK;
}

/**
 *	\brief Draw a rectangle form [x0,y0] to [x1,y1]
 */
uint8_t draw_rect(
      void *framebuffer_draw_pixel	//< pointer to a function that knows how to draw a pixel to the framebuffer
,     void *framebuffer				//< pointer to the framebuffer to draw the pixel to
, uint32_t  pixel_value				//< value to set for the pixels
, uint16_t  x0						//< x start position of the line
, uint16_t  y0						//< y start position of the line
, uint16_t  x1						//< x end   position of the line
, uint16_t  y1						//< y end   position of the line
){
	uint8_t result = STATUS_OK;
	
	result = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0, y0, x1, y0); if( result != STATUS_OK ){ return result; }
	result = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x1, y0, x1, y1); if( result != STATUS_OK ){ return result; }
	result = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x1, y1, x0, y1); if( result != STATUS_OK ){ return result; }
	result = draw_line( framebuffer_draw_pixel, framebuffer, pixel_value, x0, y1, x0, y0); if( result != STATUS_OK ){ return result; }
	
	return result;
}

