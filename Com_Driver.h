/** \file  Com_Driver.h

    \brief A Generic Communication Driver interface
 
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
#ifndef COM_DRIVER_H
#define COM_DRIVER_H

#include <asf.h>

typedef             bool  Get_Send_Stop( void *const com_module );
typedef             void  Set_Send_Stop( void *const com_module, bool const new_state );
typedef             void  Send_Stop    ( void *const com_module );
typedef enum status_code  Write_Wait   ( void *const com_module, uint16_t const address, uint8_t *const byte_array, uint16_t const cnt_bytes );

/**
 * \brief Generic communication driver interface
 *
 * The struct allows to "overload" the generic Com_Driver functions
 * with specific functions.
 */
struct Com_Driver {
        void          *com_module   ; //< specific com channel hardware module reference
        Get_Send_Stop *get_send_stop;
        Set_Send_Stop *set_send_stop;
        Send_Stop     *send_stop    ;
        Write_Wait    *write_wait   ;
};

#endif // COM_DRIVER_H
