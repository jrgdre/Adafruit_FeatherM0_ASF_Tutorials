/**     \file   Framebuffer_SSD1306.h

        \brief  Declarations for a 1bit per pixel framebuffer for the SSD1306 OLED controller IC
 
        \license 
                MIT: The MIT License (https://opensource.org/licenses/MIT)
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
                1.0.0: 2018-02-18 jrgdre initial release

 */
#ifndef FRAMEBUFFER_SSD1306_H
#define FRAMEBUFFER_SSD1306_H

#include <asf.h>
#include "Framebuffer.h"

typedef uint8_t framebuffer_ssd1306_tile_t; //< SSD1306 framebuffer is formed by bit-blocks -> a.k.a. "tiles", 8 pixels (=bits) per tile

/**
 * \brief Internal Data structure for managing a SSD1306 framebuffer.
 *
 *  Since the SSD1306 drives monochrome OLED displays, this framebuffer is implemented as a 1bit per pixel framebuffer.
 *
 * \note 
 *   Here we only provide the data structure, that enables a differential update.
 *   The display driver has to decide if and how he wants to implement a differential display update, based on the information
 *   provided by the framebuffer about it's state.
 *
 * \ref tiles
 *  - framebuffer is formed by bit-blocks (usually 8 bits wide) -> a.k.a. "tiles"
 *  - tiles side-by-side form a page (FeatherWing_OLED: 128 tiles per page)
 *  - one ore more pages stacked on top of each other form the display (e.g. FeatherWing_OLED: 4 pages)
 *
 * \ref tiles_dirty
 *  In one bit per tile, we remember the tiles that have been changed in the framebuffer 
 *  but not send to the display yet (are dirty), to later be able to optimize the screen update.
 *
 * \ref tiles_dirty_count
 *  Updating a tile in random access mode requires:
 *  - sending the page   start/end address command (5 bytes on the wire: i2c address + 3 byte command + getting 1 byte acknowledge back)
 *  - sending the column start/end address command (5 bytes on the wire: i2c address + 3 byte command + getting 1 byte acknowledge back)
 *  - sending the data                             (4 bytes on the wire: i2c address + 2 byte command + getting 1 byte acknowledge back)
 *  - That's 14 bytes in total.
 *  .
 *  Sending the whole framebuffer of e.g. the FeatherWing_OLED (512 tiles) over requires:
 *  - sending the page   start/end address command (  1 x 5 bytes on the wire: i2c address + 3 byte command + getting 1 byte acknowledge back)
 *  - sending the column start/end address command (  1 x 5 bytes on the wire: i2c address + 3 byte command + getting 1 byte acknowledge back)
 *  - sending the data                             (512 x 4 bytes on the wire: i2c address + 2 byte command + getting 1 byte acknowledge back)
 *  - That's 2058 bytes in total.
 *  .
 *  So for the FeatherWing_OLED it is faster to send the whole framebuffer over if more than 147 tiles have to be updated (are dirty).
 *  This threshold depends on the number of tiles the display area is made of.
 *  .
 *  To make the decision which way to go fast and effortless we count the number of tiles in \ref tiles_dirty_count.
 */
struct Framebuffer_SSD1306 {
                          uint16_t  columns;            //< number of columns (tiles side-by-side in one page)
                          uint16_t  pages;              //< number of pages (tiles stacked on top of each other)
                          uint32_t  bytes;              //< size of this framebuffer in bytes
                           uint8_t  bits_per_tile;      //< number of bits that make up a tile
        framebuffer_ssd1306_tile_t *tiles;              //< this is the pixmap that we draw to and send to the display eventually
                           uint8_t *tiles_dirty;        //< 1 bit per tile,  set if tile was changed
                          uint32_t  tiles_dirty_count;  //< number of dirty tiles
};

struct Framebuffer *framebuffer_SSD1306_create( uint32_t width, uint32_t height ); // create a new framebuffer instance for a SSD1306 OLED controlled display

#endif // FRAMEBUFFER_SSD1306_H