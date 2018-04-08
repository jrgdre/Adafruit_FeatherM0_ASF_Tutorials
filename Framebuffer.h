/**     \file   Framebuffer.h

        \brief  Declaration of a generic framebuffer.

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
                DIT: 2017-2018; Drechsler Information Technologies; www.drechsler-it.de
 
        \authors
                jrgdre: Joerg Drechsler; DIT
 
        \versions
                2.0.0: 2018-02-18 jrgdre, refactor interface
                1.0.0: 2017-06-22 jrgdre, initial release

 */
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <asf.h> // this is an Atmel Software Foundation project

struct Framebuffer;

typedef enum status_code Framebuffer_Clear     ( struct Framebuffer *framebuffer );                                                //< reset framebuffer content to clear state
typedef enum status_code Framebuffer_Destroy   ( struct Framebuffer *framebuffer );                                                //< release all framebuffer allocated resources
typedef enum status_code Framebuffer_Get_Pixel ( struct Framebuffer *framebuffer, uint32_t x, uint32_t y, uint32_t *pixel_value ); //< get the value of a pixel
typedef enum status_code Framebuffer_Set_Pixel ( struct Framebuffer *framebuffer, uint32_t x, uint32_t y, uint32_t  pixel_value ); //< set the value of a pixel

/**
        \brief Generic Framebuffer data type
        
        This data type should be used in all functions that want to access a framebuffer independent of it's specific characteristics.
        
 */
struct Framebuffer {
        uint32_t               width    ; //< width  of the framebuffer in pixel
        uint32_t               height   ; //< height of the framebuffer in pixel
        Framebuffer_Clear     *clear    ; //< pointer to clear    () implementation to use for a specific framebuffer instance
        Framebuffer_Destroy   *destroy  ; //< pointer to destroy  () implementation to use for a specific framebuffer instance
        Framebuffer_Get_Pixel *get_pixel; //< pointer to get_pixel() implementation to use for a specific framebuffer instance
        Framebuffer_Set_Pixel *set_pixel; //< pointer to set_pixel() implementation to use for a specific framebuffer instance
        void                  *user_data; //< pointer to user defined data or data structure                
};

#endif // FRAMEBUFFER_H
