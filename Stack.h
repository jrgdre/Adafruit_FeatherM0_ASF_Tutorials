/** \file  Stack.h

    \brief Declaration of a simple stack.

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
           1.0.0: 2017-06-27 jrgdre initial release

 */
#ifndef STACK_H
#define STACK_H

#include <asf.h>

/**
 * Type of the entries on the stack.
 */
#define Entry uint16_t        

/**
 * Forward declaration of the stack struct.
 */
typedef struct Stack Stack;

/**
 * Allocate memory for a stack with an \ref initial_capacity.
 * and initialize stack status variables.
 *
 * \note The stack is self growing (see \ref stack_push()).
 *       Start with a reasonable low estimate for the initial capacity.
 *
 * \return Status of operation.
 * \retval STATUS_OK                    If operation was successfully
 * \retval STATUS_ERR_INVALID_ARG       If \ref stack is not assigned or initial_capacity < 1
 * \retval STATUS_ERR_NO_MEMORY         If \ref stack had to be reallocated, but there was no memory
 */
uint8_t stack_allocate(
  Stack   **stack                       //< [out] pointer to a stack structure
, uint8_t   const initial_capacity      //< [in]  initial capacity of the stack (> 0)
);

/**
 * Deallocate the memory the stack uses 
 * and reset the stack status variables.
 *
 * \return Status of operation.
 * \retval STATUS_OK                    If operation was successfully
 * \retval STATUS_ERR_INVALID_ARG       If \ref stack is not assigned 
 */
uint8_t stack_deallocate(
  Stack **stack                         //< [in/out] pointer to the stack structure to deallocate
);

/**
 * Return and remove the top entry from the stack
 *
 * \return Status of operation.
 * \retval STATUS_OK                    If operation was successfully
 * \retval STATUS_ERR_INVALID_ARG       If \ref stack or entry is not assigned
 * \retval STATUS_ERR_NOT_FOUND         If \ref stack has no more entries
 */
uint8_t stack_pop(
  Stack *const stack                    //< [in]  stack to pop entry from
, Entry *const entry                    //< [out] value retrieved from stack
);

/**
 * Put an \ref entry on top of the stack.
 *
 * If the stack is not large enough for another entry
 * this function tries to allocate more memory.
 *
 * \return Status of operation.
 * \retval STATUS_OK                    If operation was successfully
 * \retval STATUS_ERR_INVALID_ARG       If \ref stack is not assigned
 * \retval STATUS_ERR_NO_MEMORY         If \ref stack had to be reallocated, but there was no memory
 */
uint8_t stack_push(
  Stack *const stack                    //< [in] stack to push entry to
, Entry  const entry                    //< [in] value to push to stack
);

/**
 * Change the capacity of the stack.
 *
 * \note If \ref new_capacity < 1 this function calls \ref stack_deallocate().
 *
 * \return Status of operation.
 * \retval STATUS_OK                    If operation was successfully
 * \retval STATUS_ERR_INVALID_ARG       If \ref stack is not assigned
 * \retval STATUS_ERR_NO_MEMORY         If \ref stack had to be reallocated, but there was no memory
 */
uint8_t stack_realloc(
  Stack   *stack                        //< [in] stack structure to allocate
, uint8_t  const new_capacity           //< [in] new capacity of the stack
);

#endif