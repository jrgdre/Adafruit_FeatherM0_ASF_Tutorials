/** \file  Com_Driver_i2c_master.h

    \brief General Purpose Communication Driver implementation for an I²C bus master
 
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
#include "Com_Driver_i2c_master.h"

// =================
// private functions
// =================

/**
 * Get the current state of com_modul's send_stop property
 *
 * \return Status of the send_stop property
 * \retval true                STOP condition will be sent after a read/write
 * \retval false        NACK signal will be sent after a read/write
 *
 * \asserts com_module != NULL
 */
static bool com_driver_i2c_master_get_send_stop (
  void *const com_module        //< com_module to get the state for
){
        Assert(com_module != NULL);
        
        struct i2c_master_module *com_module_i2c = (struct i2c_master_module*) com_module;
        
        return com_module_i2c->send_stop;
};

/**
 * Set a new state for com_modul's send_stop property
 *
 * Status of the send_stop property
 * - true       STOP condition will be sent after a read/write
 * - false      NACK signal will be sent after a read/write
 *
 * \asserts com_module != NULL
 */
static void com_driver_i2c_master_set_send_stop ( 
  void *const com_module        //< com_module to set the state for
, bool  const new_state         //< new state to set
){
        Assert(com_module != NULL);
        
        struct i2c_master_module *com_module_i2c = (struct i2c_master_module*) com_module;
        
        com_module_i2c->send_stop = new_state;
};

/**
 * Send a STOP condition over i2c using the com_module
 *
 * \asserts com_module != NULL
 */
static void com_driver_i2c_master_send_stop ( 
  void *const com_module        //< com_module to send stop signal with
){
        Assert(com_module != NULL);
        
        struct i2c_master_module *com_module_i2c = (struct i2c_master_module*) com_module;
        
        i2c_master_send_stop( com_module_i2c );
};

/**
 * send a word over i2c using the com_module
 *
 * \return Status of operation.
 * \retval STATUS_OK                    If packet was write successfully
 * \retval STATUS_BUSY                  If master module is busy with a job
 * \retval STATUS_ERR_DENIED            If error on bus
 * \retval STATUS_ERR_PACKET_COLLISION  If arbitration is lost
 * \retval STATUS_ERR_BAD_ADDRESS       If slave is busy, or no slave acknowledged the address
 * \retval STATUS_ERR_TIMEOUT           If timeout occurred
 * \retval STATUS_ERR_OVERFLOW          If slave did not acknowledge last sent data,
 *                                      indicating that slave does not want more data 
 *                                      and was not able to read the last data sent.
 *
 * \asserts com_module != NULL
 * \asserts byte_array != NULL
 */
static enum status_code com_driver_i2c_master_write_wait ( 
  void     *const com_module    //< communication module the SSD1306 is connected to
, uint16_t  const address       //< I2C address the SSD1306 listens to 
, uint8_t  *const byte_array    //< array of bytes to write
, uint16_t  const cnt_bytes     //< count of bytes to write
){
        enum status_code status = STATUS_ERR_INVALID_ARG;

        Assert(com_module != NULL);
        Assert(byte_array != NULL);
                
        struct i2c_master_module *com_module_i2c = (struct i2c_master_module*) com_module;
        
        struct i2c_master_packet packet = {
                .address         = address,
                .data_length     = cnt_bytes,
                .data            = byte_array,
                .ten_bit_address = false,
                .high_speed      = false,
                .hs_master_code  = 0x0
        };
        
        if( com_module_i2c->send_stop ) {
                status = i2c_master_write_packet_wait( com_module_i2c, &packet );
        } else {
                status = i2c_master_write_packet_wait_no_stop( com_module_i2c, &packet );
        }
        
        return status;
};

// ================
// public functions
// ================

/**
 * assign the i2c specific
 * - com_modul
 * - driver functions
 * to the generic com_driver data structure
 */
void com_driver_i2c_master_init(
  struct Com_Driver        *com_driver          //< Com_Driver data structure to initialize
, struct i2c_master_module *const com_module    //< i2c_master_module (com_module) to connect to
){
        Assert(com_driver != NULL);
        
        com_driver->com_module    = com_module                         ;
        com_driver->get_send_stop = com_driver_i2c_master_get_send_stop;
        com_driver->set_send_stop = com_driver_i2c_master_set_send_stop;
        com_driver->send_stop     = com_driver_i2c_master_send_stop    ;
        com_driver->write_wait    = com_driver_i2c_master_write_wait   ;
};
