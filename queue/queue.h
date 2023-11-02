#ifndef	__QUEUE_H__
#define	__QUEUE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* macros =================================================================== */

// define the maximum queue size
#ifndef MAX_QUEUE_SIZE
	#define MAX_QUEUE_SIZE 64
#endif // MAX_QUEUE_SIZE

/* enumerations ============================================================= */

enum queueError {
	QUEUE_OK = 0,
	QUEUE_EMPTY,
	QUEUE_FULL,
	QUEUE_TOO_MANY
};

/* structures =============================================================== */

typedef struct {
    uint8_t data[MAX_QUEUE_SIZE]; // Table of data
    size_t front; // Index of the first element
	size_t len; // Number of elements in the queue
} Queue;

/* public function prototypes ================================================ */

// Initialize the queue
Queue queue_init(void);

// Add an element to the queue
enum queueError queue_push(Queue *queue, uint8_t data);

// Add many elements to the queue
enum queueError queue_push_many(Queue *queue, uint8_t *data, size_t len);

// Remove an element from the queue
enum queueError queue_pop(Queue *queue, uint8_t *data);

// Remove all elements from the queue
void queue_pop_all(Queue *queue);

// Get the number of elements in the queue
size_t queue_len(Queue *queue);

// Get the element at the front of the queue
enum queueError queue_front(Queue *queue, uint8_t *data);

// Get the element at the back of the queue
enum queueError queue_back(Queue *queue, uint8_t *data);

// Check if the queue is empty
bool queue_empty(Queue *queue);

// Check if the queue is full
bool queue_full(Queue *queue);



#endif // __QUEUE_H__