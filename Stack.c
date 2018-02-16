/**	\file	Stack.c

 	\brief	A library implementing a simple stack.

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
 		1.0.0: 2017-06-27 jrgdre initial release

 */
#include <string.h>
#include "Stack.h"

/**
 *	The stack management data structure.
 */
struct Stack {
	uint32_t  capacity;	// number of entries that fit on the stack [read only!]
	uint32_t  count;	// number of entries on the stack [read only!]
	Entry    *entries;	// array of entries
};


/**
 *	\brief	Allocate memory for a stack with an initial_capacity.
 */
uint8_t stack_allocate(
	Stack   **stack						// stack structure to allocate
,	uint8_t   const initial_capacity	// initial capacity of the stack
){
	if( stack == NULL	){
		return STATUS_ERR_INVALID_ARG;
	}
	
	(*stack) = (Stack *)malloc( sizeof ( Stack ) ); // allocate memory for the management structure
	if( (*stack) == NULL ){
		return STATUS_ERR_NO_MEMORY;
	}
	
	(*stack)->capacity = initial_capacity < 1 ? 1 : initial_capacity; // do not report this invalid argument, but fix it
	(*stack)->count    = 0;
	(*stack)->entries  = (Entry *)malloc( sizeof( Entry ) * (*stack)->capacity );	// allocate the initial stack memory
	if( (*stack)->entries == NULL ){
		(*stack)->capacity = 0;
		return STATUS_ERR_NO_MEMORY;
	}
	
	return STATUS_OK;
}

/**
 *	\brief	Deallocate the memory the stack uses.
 */
uint8_t stack_deallocate(
	Stack **stack	// stack structure to deallocate
){
	if( stack == NULL ){
		return STATUS_ERR_INVALID_ARG;
	}
	
	(*stack)->capacity = 0;
	(*stack)->count    = 0;
	(*stack)->entries  = realloc( (*stack)->entries, 0 ); // release the stack memory and set pointer to NULL
	(*stack)           = realloc( (*stack)         , 0 ); // release the memory of the management structure and set pointer to NULL
	
	return STATUS_OK;
}

/**
 *	\brief	Return and remove the top entry from the stack.
 */
uint8_t stack_pop(
	Stack *const stack	// stack to pop entry from
,	Entry *const entry	// value retrieved from stack
){
	if(	   ( stack == NULL )
		|| ( entry == NULL )
	){
		return STATUS_ERR_INVALID_ARG;
	}
	
	if( stack->count < 1 ) {
		return STATUS_ERR_NOT_FOUND;
	}
	
	stack->count--;
	*entry = stack->entries[stack->count];
	
	return STATUS_OK;
}

/**
 *	\brief	Put an entry on top of the stack.
 */
uint8_t stack_push(
	Stack *const stack	// stack to push entry to
,	Entry  const entry	// value to push to the stack
){
	if( stack == NULL ){
		return STATUS_ERR_INVALID_ARG;
	}
	
	uint8_t ret = STATUS_OK;
	
	stack->count++;
	if( stack->count > stack->capacity ){
		// stack to small
		ret = stack_realloc( stack, stack->capacity << 1 ); // try to double stack size
		if( ret != STATUS_OK ) {
			return ret;
		}
	}
	stack->entries[stack->count - 1] = entry; // put new entry on top of stack
	
	return STATUS_OK;
}

/**
 *	\brief	Change the capacity of the stack.
 */
uint8_t stack_realloc( 
	Stack   *stack				// stack structure to allocate
,	uint8_t  const new_capacity	// new capacity of the stack 
){
	if( stack == NULL ){
		return STATUS_ERR_INVALID_ARG;
	}
	if( new_capacity < 1 ){
		return stack_deallocate( &stack );
	}
	
	Entry *ptr;	// pointer to new allocated stack->entry memory
	
	ptr = (Entry *)realloc( stack->entries, sizeof( Entry ) * ( new_capacity ) );
	if( ptr == NULL ){
		return STATUS_ERR_NO_MEMORY;
	}
	stack->entries  = ptr;
	stack->capacity = new_capacity;
	
	return STATUS_OK;
}

