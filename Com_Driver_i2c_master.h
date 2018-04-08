/** \file  Com_Driver_i2c_master.h

    \brief Declarations for the General Purpose Communication Driver for an I²C bus master
 
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
           1.0.0: 2017-12-19 jrgdre initial release

 */
#ifndef COM_DRIVER_I2C_MASTER_H
#define COM_DRIVER_I2C_MASTER_H

#include <asf.h>                // it's an Atmel Software Foundation project
#include "Com_Driver.h"         // based on the generic communication driver interface

/**
 * Initialize a general purpose Com_Driver data structure 
 * with the specific i2c com driver settings.
 *
 * \asserts com_driver != NULL
 */
void com_driver_i2c_master_init(
  struct Com_Driver        *com_driver          //< communication driver data structure to initialize
, struct i2c_master_module *const com_module    //< i2c communication module being used
);

#endif /* COM_DRIVER_I2C_MASTER_H */
