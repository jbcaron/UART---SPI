
/*
 * queue.c
 *
 * Description: This file contains the implementation of a queue (FIFO) data
 * structure. It is implemented as a circular buffer with a fixed size.
 * 
 * Note :
 * - The type of data stored in the queue is uint8_t and can be changed by
 *  modifying the type of the data array in the Queue structure in queue.h
 * - The size of the queue is defined by MAX_QUEUE_SIZE in queue.h
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "queue.h"

// Initialize the queue
Queue queue_init(void) {
	return (Queue) { .front = 0, .len = 0 };
}

// Add an element to the queue
enum queueError queue_push(Queue *queue, uint8_t data) {
	if (queue->len < MAX_QUEUE_SIZE) {
		queue->data[(queue->front + queue->len) % MAX_QUEUE_SIZE] = data;
		queue->len++;
		return QUEUE_OK;
	}
	return QUEUE_FULL;
}

// Add many elements to the queue
enum queueError queue_push_many(Queue *queue, uint8_t *data, size_t len) {
	if (len > MAX_QUEUE_SIZE)
		return QUEUE_TOO_MANY;
	else if (queue->len + len > MAX_QUEUE_SIZE)
		return QUEUE_FULL;
	for (size_t i = 0; i < len; i++) {
		queue->data[(queue->front + queue->len + i) % MAX_QUEUE_SIZE] = data[i];
	}
	queue->len += len;
	return QUEUE_OK;
}

// Remove an element from the queue
enum queueError queue_pop(Queue *queue, uint8_t *data) {
	if (queue->len > 0) {
		if (data)
			*data = queue->data[queue->front];
		queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
		queue->len--;
		return QUEUE_OK;
	}
	return QUEUE_EMPTY;
}

// Remove all elements from the queue
void queue_pop_all(Queue *queue) {
	queue->front = 0;
	queue->len = 0;
}

// Get the number of elements in the queue
size_t queue_len(Queue *queue) {
	return queue->len;
}

// Get the element at the front of the queue
enum queueError queue_front(Queue *queue, uint8_t *data) {
	if (queue->len > 0) {
		*data = queue->data[queue->front];
		return QUEUE_OK;
	}
	return QUEUE_EMPTY;
}

// Get the element at the back of the queue
enum queueError queue_back(Queue *queue, uint8_t *data) {
	if (queue->len > 0) {
		*data = queue->data[(queue->front + queue->len - 1) % MAX_QUEUE_SIZE];
		return QUEUE_OK;
	}
	return QUEUE_EMPTY;
}

// Check if the queue is empty
bool queue_empty(Queue *queue) {
	return queue->len == 0;
}

// Check if the queue is full
bool queue_full(Queue *queue) {
	return queue->len == MAX_QUEUE_SIZE;
}