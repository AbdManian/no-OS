/***************************************************************************//**
 *   @file   list.h
 *   @brief  List library header
 *   @author Mihail Chindris (mihail.chindris@analog.com)
********************************************************************************
 *   @copyright
 * Copyright 2020(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
 *
 *  @section list_details Library description
 *   This library handles double linked lists and it expose inseart,
 *   read, get and delete functions. \n
 *   It also can be accesed using it member functions which wrapp function for
 *   usual list types.\n
 *  @subsection example Sample code
 *   @code{.c}
 *	// -- Use a generic list
 *	struct list_desc *list1;
 *	struct iterator  *it;
 *	uint32_t	 a;
 *	// Create list list1
 *	list_init(&list1, LIST_DEFAULT, NULL);
 *	// Add items to the list
 *	list_add_last(list1, 1);
 *	list_add_last(list1, 2);
 *	list_add_last(list1, 3);
 *	// Here the list will be: 1 -> 2 - > 3
 *
 *	list_read_last(list1, &a);
 *	printf("Last: %d\n", a);
 *	// 3 will be printed
 *	// Create an iterator on the end of the list
 *	iterator_init(&it, list1, 0);
 *	// Move the iterator backword with one position
 *	iterator_move(it, -1);
 *	// Read the data at the current position
 *	iterator_read(it, &a);
 *	printf("Current: %d\n", a);
 *	// 2 will be printed
 *	iterator_remove(it);
 *	list_remove(list1);
 *
 *	// -- Use a popular list
 *	struct list_desc *stack;
 *	// Create a FIFO list
 *	list_init(&stack, LIST_STACK, NULL);
 *	// Put elements in the list
 *	stack->push(stack, 1);
 *	stack->push(stack, 2);
 *	stack->push(stack, 3);
 *	// Read from the stack
 *	stack->pop(stack, &a);
 *	printf("Last: %d\n", a);
 *	// 3 will be printed
 *	list_remove(stack);
 *    @endcode
*******************************************************************************/

#ifndef LIST_H
#define LIST_H

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/******************************************************************************/
/*************************** Types Declarations *******************************/
/******************************************************************************/

/**
 * @struct list_desc
 * @brief Structure storing the list and function wrapper for usual list types
 *
 * With this structure the funtionalities of usual list types
 * ( \ref adapter_type ) can be accesed with the functions referenced in this
 * structure.
 * For example:
 * @code{.c}
 * my_queue->push(my_queue, my_data);
 * @endcode
 */
struct list_desc;

/**
 * @struct list_iterator
 * @brief Structure used to iterate through the list using Iterator functions.
 */
struct iterator;

/**
 * @brief Prototype of the compare function.
 *
 * The function used to compare the elements of the liste when doing
 * operations on an ordered list.
 * @param data1 - First element to be compared
 * @param data2 - Second element to be compared
 * @return
 *  - -1 - If data1 < data2
 *  - 0 - If data1 == data2
 *  - 1 - If data1 > data2
 */
typedef int32_t (*f_cmp)(void *data1, void *data2);

/**
 * @name Generic functions
 * Each function interacting with the list have one of the following formats.\n
 * Aditionaly they may have one more parametere for specific functionalities.\n
 * In the Iterator functions, the list reference is replaced by the iterator's
 * one.
 * @{
 */

/**
 * @brief Add an element in the list.
 *
 * The element of the list is created and the data field is stored in it.
 * @param list_desc - Reference to the list. Created by \ref list_init
 * @param data - Data to store in a list element
 * @return
 *  - \ref SUCCESS : On success
 *  - \ref FAILURE : Otherwise
 */
typedef int32_t (*f_add)(struct list_desc *list_desc, void *data);

/**
 * @brief Edit an element in the list. The content is replaced by new_data.
 * @param list_desc - Reference to the list. Created by \ref list_init .
 * @param new_data - New data to replace the old one
 * @return
 *  - \ref SUCCESS : On success
 *  - \ref FAILURE : Otherwise
 */
typedef int32_t (*f_edit)(struct list_desc *list_desc, void *new_data);

/**
 * @brief Read an element from the list.
 * @param list_desc - Reference to the list. Created by \ref list_init
 * @param result - If not null, result is filled with:
 * @param data - Content of the list element, NULL if some error occur.
 * @return \n
 *  - \ref SUCCESS : On success
 *  - \ref FAILURE : Otherwise
 * @note If the content of an element can be 0 then the result must be checked
 * to see if the functions has succeded
 */
typedef int32_t (*f_read)(struct list_desc *list_desc, void **data);

/**
 * @brief Read and remove an element from the list.
 * @param list_desc - Reference to the list. Created by \ref list_init
 * @param result - If not null, result is filled with:
 * @param data - Content of the list element, NULL if some error occur.
 * @return
 *  - \ref SUCCESS : On success
 *  - \ref FAILURE : Otherwise
 */
typedef int32_t (*f_get)(struct list_desc *list_desc, void **data);

/** @} */

/**
 * @enum adapter_type
 * @brief Selects functionalities for functions in \ref list_desc
 */
enum adapter_type {
	/** Default type is LIST_STACK */
	LIST_DEFAULT,
	/**
	 * Functions for a LIFO list (Last-in first-out). Elements are inserted
	 * in one end and extracted from the other end.
	 *  - \e Push: Insert element
	 *  - \e Pop: Get next element (Read and remove)
	 *  - \e Top_next: Read next element
	 *  - \e Back: Read first element
	 *  - \e Swap: Edit the content of the next element
	 */
	LIST_QUEUE,
	/**
	 * Functions for a FIFO list (First-in first-out). Elements are inserted
	 * and extracted only from the same end.
	 *  - \e Push: Insert element
	 *  - \e Pop: Get top element (Read and remove)
	 *  - \e Top_next: Read top element
	 *  - \e Back: Read bottom element
	 *  - \e Swap: Edit the content of the top element
	 */
	LIST_STACK,
	/**
	 * Functions for ordered list. The order of element is determinated
	 * usinge the \ref f_cmp.
	 *  - \e Push: Insert element
	 *  - \e Pop: Get lowest element (Read and remove)
	 *  - \e Top_next: Read lowest element
	 *  - \e Back: Read the biggest element
	 *  - \e Swap: Edit the lowest element
	 */
	LIST_PRIORITY_LIST
};

struct list_desc {
	/** Refer to \ref adapter_type */
	f_add	push;
	/** Refer to \ref adapter_type */
	f_get	pop;
	/** Refer to \ref adapter_type */
	f_read	top_next;
	/** Refer to \ref adapter_type */
	f_read	back;
	/** Refer to \ref adapter_type */
	f_edit	swap;
	/** Structure storing the list internal parameters */
	void	*priv_desc;
};

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/

int32_t list_init(struct list_desc **list_desc, enum adapter_type type,
		  f_cmp comparator);
int32_t list_remove(struct list_desc *list_desc);
int32_t list_get_size(struct list_desc *list_desc, uint32_t *out_size);

/**
 * @name Iterator functions
 * An iterator is used to iterate through the list. For a list, any number of
 * iterators can be created. All must be removed before removing a list.
 * @{
 */
int32_t iterator_init(struct iterator **iter, struct list_desc *list_desc,
		      bool start);
int32_t iterator_remove(struct iterator *iter);
int32_t iterator_move(struct iterator *iter, int32_t idx);
int32_t iterator_find(struct iterator *iter, void *cmp_data);
int32_t iterator_insert(struct iterator *iter, void *data, bool after);
int32_t iterator_edit(struct iterator *iter, void *new_data);
int32_t iterator_read(struct iterator *iter, void **data);
int32_t iterator_get(struct iterator *iter, void **data);
/** @}*/

/**
 * @name Operations on the ends of the list
 * These functions will operate on the first or last element of the list
 * @{
 */
int32_t list_add_first(struct list_desc *list_desc, void *data);
int32_t list_edit_first(struct list_desc *list_desc, void *new_data);
int32_t list_read_first(struct list_desc *list_desc, void **data);
int32_t list_get_first(struct list_desc *list_desc, void **data);

int32_t list_add_last(struct list_desc *list_desc, void *data);
int32_t list_edit_last(struct list_desc *list_desc, void *new_data);
int32_t list_read_last(struct list_desc *list_desc, void **data);
int32_t list_get_last(struct list_desc *list_desc, void **data);
/** @}*/

/**
 * @name Operations by index
 * These functions use an index to identify the element in the list.
 * @{
 */
int32_t list_add_idx(struct list_desc *list_desc, void *data, uint32_t idx);
int32_t list_edit_idx(struct list_desc *list_desc, void *new_data,
		      uint32_t idx);
int32_t list_read_idx(struct list_desc *list_desc, void **data, uint32_t idx);
int32_t list_get_idx(struct list_desc *list_desc, void **data, uint32_t idx);
/** @}*/

/**
 * @name Operations by comparation
 * These functions use the specified \ref f_cmp at \ref list_init to identify
 * the element this will operate on.
 * @{
 */
int32_t list_add_find(struct list_desc *list_desc, void *data);
int32_t list_edit_find(struct list_desc *list_desc, void *new_data,
		       void *cmp_data);
int32_t list_read_find(struct list_desc *list_desc, void **data,
		       void *cmp_data);
int32_t list_get_find(struct list_desc *list_desc, void **data, void *cmp_data);
/** @}*/

#endif //LIST_H
