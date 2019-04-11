/*
 * circular_buffer.h
 *
 *  Created on: 26-Nov-2018
 *      Author: Sorabh
 */

#ifndef SOURCES_CIRCULAR_BUFFER_H_
#define SOURCES_CIRCULAR_BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct circular_buff
{
	uint8_t *buffer;
	uint8_t *head;
	uint8_t *tail;
	size_t length;
	int32_t count;
}c_buff;

typedef enum status
{
	CB_FULL = -5,
	CB_EMPTY,
	CB_INVALID_ADDR,
	CB_NOT_INITIALIZED,
	CB_NULL_PTR,
	CB_FAILURE,
	CB_SUCCESS
}CB_STATUS;


CB_STATUS cb_new(c_buff *cb, size_t length);

CB_STATUS cb_resize(c_buff *cb, size_t length);

int32_t cb_getSize(c_buff *cb);

CB_STATUS cb_add_data(c_buff *cb, uint8_t data);

CB_STATUS cb_remove_data(c_buff *cb, uint8_t *data);

bool cb_isFull(c_buff *cb);

bool cb_isEmpty(c_buff *cb);

CB_STATUS cb_delete(c_buff *cb);


#endif /* SOURCES_CIRCULAR_BUFFER_H_ */
