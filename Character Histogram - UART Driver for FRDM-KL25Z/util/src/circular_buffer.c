/*
 * circular_buffer.c
 *
 *  Created on: 26-Nov-2018
 *      Author: Sorabh
 */

#include "circular_buffer.h"
#include <stdarg.h>

/**
--------------------------------------------------------------------------------------------
print_msg
--------------------------------------------------------------------------------------------
*   This function prints the user message to console
*
*   @\param msg     User message to print
*
*   @\return        None
*
*/
void print_msg(const char *msg, ...)
{
#ifndef UNIT_TEST
	va_list va_args;

	va_start(va_args, msg);
	vprintf(msg, va_args);
	va_end(va_args);
	fflush(stdout);
#endif
}


CB_STATUS cb_new(c_buff *cb, size_t length)
{
	if (cb == NULL)
		return CB_NULL_PTR;

	cb->buffer = (uint8_t *) malloc(length * sizeof(uint8_t));

	if ((cb->buffer) == NULL)
		return CB_FAILURE;

	cb->head = cb->buffer;
	cb->tail = cb->buffer;
	cb->length = length;
	cb->count = 0;

	return CB_SUCCESS;
}

CB_STATUS cb_resize(c_buff *cb, size_t length)
{
    if (cb == NULL)
        return CB_NULL_PTR;

    int c = cb->count;

    cb->buffer = (uint8_t *) realloc(cb->buffer, (2 * length));

    if ((cb->buffer) == NULL)
        return CB_FAILURE;

    cb->head = (cb->buffer + length);
    cb->length = (length * 2);
    cb->count = c;

    return CB_SUCCESS;
}

int32_t cb_getSize(c_buff *cb)
{
    int32_t count = cb->count;
    return count;
}

CB_STATUS cb_add_data(c_buff *cb, uint8_t data)
{
	if (cb == NULL)
		return CB_NULL_PTR;

	if ((cb->buffer) == NULL)
		return CB_NOT_INITIALIZED;

	if (cb_isFull(cb) == true)
		return CB_FULL;

	if ((cb->head) == ((cb->buffer) + ((cb->length) - 1)))
	{
		*(cb->head) = data;
		print_msg("Insert Data at %d is %d\n", cb->count, *(cb->head));
		cb->head = cb->buffer;
		(cb->count)++;

		return CB_SUCCESS;
	}
	else if ((cb->head) < ((cb->buffer) + ((cb->length) - 1)))
	{
		*(cb->head) = data;
		print_msg("Insert Data at %d is %d\n", cb->count, *(cb->head));
		(cb->head)++;
		(cb->count)++;

        return CB_SUCCESS;
	}
	else {
		return CB_INVALID_ADDR;
	}
}

CB_STATUS cb_remove_data(c_buff *cb, uint8_t *data)
{
	if (cb == NULL)
		return CB_NULL_PTR;

	if ((cb->buffer) == NULL)
		return CB_NOT_INITIALIZED;

	if (cb_isEmpty(cb))
		return CB_EMPTY;

	if ((cb->tail) == ((cb->buffer) + ((cb->length) - 1)))
	{
		*(data) = *(cb->tail);
		print_msg("Remove Data from %d is %d	%d	%d\n", cb->count, *(data), *(cb->tail), (cb->count));
		cb->tail = cb->buffer;
		(cb->count)--;

		return CB_SUCCESS;
	}
	else if ((cb->tail) < ((cb->buffer) + ((cb->length) - 1)))
	{
		*(data) = *(cb->tail);
		print_msg("Remove Data from %d is %d	%d	%d\n", cb->count, *(data), *(cb->tail), (cb->count));
		(cb->tail)++;
		(cb->count)--;

        return CB_SUCCESS;
	}
	else {
		return CB_INVALID_ADDR;
	}

}

bool cb_isFull(c_buff *cb)
{
	if ((cb != NULL) || ((cb->buffer) != NULL))
	{
		if ((cb->count) == (cb->length))
			return true;
	}

	return false;
}

bool cb_isEmpty(c_buff *cb)
{
	if ((cb != NULL) || ((cb->buffer) != NULL))
    {
        if ((cb->count) == 0)
            return true;
    }

    return false;
}

CB_STATUS cb_delete(c_buff *cb)
{
	if ((cb != NULL) || ((cb->buffer) != NULL))
	{
		free(cb->buffer);
		cb->buffer = NULL;
		return CB_SUCCESS;
	}
	else {
		return CB_NOT_INITIALIZED;
	}
}

