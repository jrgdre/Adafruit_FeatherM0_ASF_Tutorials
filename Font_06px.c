/**     \file   Font_06px.c

        \brief  A tiny 6 pixel height, proportional font.

        \license 
                MIT:        The MIT License (https://opensource.org/licenses/MIT)
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

#include "Font.h"
#include "Font_06px.h"

#define FONT_06PX_CHAR_IDX_MAX          47
#define FONT_06PX_CHAR_WIDTH_BYTE_IDX    6
#define FONT_06PX_HEIGHT                 6

/**
 * Character index = (ASCII-Code - 0x20) >> 1
 * Character left  = !(ASCII-Code - 0x20) % 2
 */
const uint8_t font_06px[FONT_06PX_CHAR_IDX_MAX + 1][FONT_06PX_HEIGHT + 1] = {
        {0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x21},     // <SPACE> !
        {0xAA, 0xBF, 0x0A, 0x1F, 0x0A, 0x00, 0x35},     // ' #
        {0x74, 0xA1, 0x72, 0x2C, 0xF1, 0x20, 0x53},     // $ % 
        {0x11, 0x29, 0x12, 0x2C, 0x28, 0x14, 0x42},     // & ´
        {0x06, 0x09, 0x09, 0x09, 0x06, 0x00, 0x22},     // ( )
        {0x48, 0x32, 0x7F, 0x32, 0x48, 0x00, 0x43},     // * +
        {0x00, 0x00, 0x07, 0x08, 0x08, 0x10, 0x23},     // , -
        {0x00, 0x01, 0x02, 0x04, 0x18, 0x00, 0x14},     // . /
        {0x1D, 0x17, 0x15, 0x15, 0x1D, 0x00, 0x32},     // 0 1
        {0x3F, 0x09, 0x3B, 0x21, 0x3F, 0x00, 0x33},     // 2 3
        {0x27, 0x2C, 0x3F, 0x09, 0x0F, 0x00, 0x33},     // 4 5
        {0x27, 0x21, 0x39, 0x29, 0x39, 0x00, 0x33},     // 6 7
        {0x3F, 0x2D, 0x3F, 0x29, 0x39, 0x00, 0x33},     // 8 9
        {0x00, 0x05, 0x00, 0x05, 0x02, 0x00, 0x12},     // : ;
        {0x08, 0x17, 0x20, 0x17, 0x08, 0x00, 0x33},     // < =
        {0x12, 0x09, 0x06, 0x08, 0x12, 0x00, 0x32},     // > ?
        {0x66, 0x99, 0xBF, 0x89, 0x69, 0x00, 0x44},     // @ A
        {0xE7, 0x98, 0xE8, 0x98, 0xE7, 0x00, 0x44},     // B C
        {0x77, 0x4C, 0x4E, 0x4C, 0x77, 0x00, 0x43},     // D E
        {0x76, 0x48, 0x6B, 0x49, 0x46, 0x00, 0x34},     // F G
        {0x4F, 0x4A, 0x7A, 0x4A, 0x4F, 0x00, 0x43},     // H I
        {0xF9, 0x1A, 0x1C, 0x9A, 0x69, 0x00, 0x44},     // J K
        {0x9B, 0x95, 0x95, 0x91, 0xF1, 0x00, 0x35},     // L M
        {0x96, 0xD9, 0xB9, 0x99, 0x96, 0x00, 0x44},     // N O
        {0xE6, 0x99, 0xE9, 0x89, 0x86, 0x01, 0x44},     // P Q
        {0xE6, 0x98, 0xE6, 0x91, 0x96, 0x00, 0x44},     // R S
        {0x79, 0x29, 0x29, 0x29, 0x26, 0x00, 0x34},     // T U
        {0xB1, 0xB1, 0xB5, 0xB5, 0x4A, 0x00, 0x35},     // V W
        {0x2D, 0x2D, 0x12, 0x2A, 0x2A, 0x00, 0x33},     // X Y
        {0x1F, 0x06, 0x0A, 0x12, 0x1F, 0x00, 0x32},     // Z [
        {0x03, 0x21, 0x11, 0x09, 0x07, 0x00, 0x42},     // \ ]
        {0x10, 0x28, 0x00, 0x00, 0x00, 0x07, 0x33},     // ^ _
        {0x20, 0x27, 0x19, 0x09, 0x07, 0x00, 0x24},     // ´ a
        {0x40, 0x73, 0x4C, 0x4C, 0x73, 0x00, 0x43},     // b c
        {0x08, 0x3B, 0x4F, 0x4C, 0x3B, 0x00, 0x43},     // d e
        {0x30, 0x27, 0x79, 0x27, 0x21, 0x0E, 0x34},     // f g
        {0x42, 0x70, 0x4E, 0x4A, 0x4F, 0x00, 0x43},     // h i
        {0x28, 0x09, 0x7E, 0x19, 0x19, 0x60, 0x34},     // j k
        {0xC0, 0x5E, 0x55, 0x55, 0x75, 0x00, 0x35},     // l m
        {0x00, 0xE6, 0x99, 0x99, 0x96, 0x00, 0x44},     // n o
        {0x00, 0xE7, 0x99, 0xE7, 0x81, 0x81, 0x44},     // p q
        {0x00, 0x0F, 0x12, 0x11, 0x13, 0x00, 0x32},     // r s
        {0x20, 0x79, 0x29, 0x29, 0x37, 0x00, 0x34},     // t u
        {0x00, 0xB1, 0xB5, 0xB5, 0x4A, 0x00, 0x35},     // v w
        {0x00, 0x99, 0x69, 0x67, 0x91, 0x0E, 0x44},     // x y
        {0x03, 0x72, 0x14, 0x22, 0x3B, 0x00, 0x43},     // z {
        {0x0E, 0x0A, 0x01, 0x0A, 0x0E, 0x00, 0x13},     // | }
        {0x2A, 0x55, 0x02, 0x00, 0x00, 0x00, 0x43}      // ~ °
};

/**
 * \asserts ( framebuffer != NULL )
 * \asserts ( framebuffer->set_pixel != NULL )
 */
enum status_code  font_06px_draw_character (
  struct Framebuffer *framebuffer       //< pointer to the framebuffer to draw the bitmap to
,               char  character         //< character to draw
,           uint32_t  pixel_value       //< value to set for the pixels
,           uint16_t  x0                //< x start position of character bitmap in framebuffer
,           uint16_t  y0                //< y start position of character bitmap in framebuffer
,            uint8_t  font_background   //< how to draw font background
,            uint8_t *char_width        //< [out] width of character drawn in pixel
){
        Assert( framebuffer != NULL );
        Assert( framebuffer->set_pixel != NULL );
        
        Framebuffer_Set_Pixel *set_pixel = framebuffer->set_pixel;

        int_fast16_t  char_idx;         // index of the shared character byte in the font
        uint_fast8_t  char_offset;      // offset of char in the shared character byte
        uint_fast8_t  char_left_width;  // width of the left char in the shared character byte
        uint_fast8_t  char_right_width; // width of the right char in the shared character byte
                bool  is_char_left;     // true if we want to use the left char in the shared character byte
                bool  is_set_pixel;     // true if we want to set pixel_value for that pixel
        int_fast16_t  residue;          // residue of the char index calculation 
        uint_fast8_t  scanline;         // a single x line of bits 
        uint_fast8_t  x, y;             // current x and y pixel position to draw
        
        // find out which entry of the font array to use
        char_idx = (character - 0x20) >> 1;
        if( ( char_idx >= 0 ) && ( char_idx <= FONT_06PX_CHAR_IDX_MAX ) ) {
                residue      = (character - 0x20) % 2;
                is_char_left = residue > 0 ? false : true; // if there is no residue, its a left char
        } else {
                // print <SPACE> for unknown characters
                char_idx         = 1;
                is_char_left = true;
        }
                
        char_left_width  = ( font_06px[char_idx][FONT_06PX_CHAR_WIDTH_BYTE_IDX] >> 4 ) & 0xF;
        char_right_width = ( font_06px[char_idx][FONT_06PX_CHAR_WIDTH_BYTE_IDX]      ) & 0xF;
        char_offset      = is_char_left ? char_right_width : 0x00;
       *char_width       = is_char_left ? char_left_width : char_right_width;        

        // for convenience of calculation we paint forward in y-direction and backwards in x-direction
        // - y is the scanline index for the character bitmap (we got from top to bottom)
        // - for the pixel information we use the scanline byte like a shift register and evaluate the 2^0 bit
        y = 0;        // y to start with
        do {
                x = *char_width;        // x to start with
                scanline = font_06px[char_idx][y] >> char_offset; // push out the right char, if the left one is wanted
                while( x > 0 ){
                        is_set_pixel = scanline & 0x01; // evaluate the bit at 2^0
                        if( is_set_pixel ){
                                set_pixel( framebuffer, x0+x-1, y0+y, pixel_value );       // always draw pixel if to set
                        } else {
                                if( font_background == FONT_BACKGROUND_OPAQUE ){
                                        set_pixel( framebuffer, x0+x-1, y0+y, 0x00 );      // only clear background pixel, if mode is opaque
                                }
                        }
                        scanline = scanline >> 1;
                        x--;
                }
                y++;
        } while ( y < FONT_06PX_HEIGHT );
                
        return STATUS_OK;
}

/**
 * \asserts ( framebuffer != NULL )
 * \asserts ( string      != NULL )
 */
enum status_code font_06px_draw_string (
  struct Framebuffer *framebuffer       //< pointer to the framebuffer to draw the bitmap to
,         const char *string            //< string to draw
,           uint32_t  pixel_value       //< value to set for the pixels
,           uint16_t  x0                //< x start position of character bitmap in framebuffer
,           uint16_t  y0                //< y start position of character bitmap in framebuffer
,            uint8_t  font_background   //< how to draw font background
,            uint8_t *string_width      //< [out] width of character drawn in pixel
){
        Assert( framebuffer != NULL );
        Assert( string      != NULL );

        *string_width = 0;
        
        uint_fast8_t  x_pos      = 0;
             uint8_t  char_width = 0;
                char  curr;
        
        for( uint_fast8_t idx = 0; idx < 256; idx++ ) {
                curr = string[idx];
                if( curr == 0x00 ){
                        break;
                }
                font_06px_draw_character( framebuffer, string[idx], pixel_value, x0+x_pos, y0, FONT_BACKGROUND_OPAQUE, &char_width); 
                x_pos += char_width + 1;
                *string_width += char_width + 1;
        }
        if( *string_width > 0 ){
                (*string_width)--;
        }

        return STATUS_OK;
}
