#include <criterion/criterion.h>
#include "queue.h"

Test(queue, test_queue_init) {
    Queue queue = queue_init();
    cr_assert_eq(queue_empty(&queue), true);
    cr_assert_eq(queue_full(&queue), false);
    cr_assert_eq(queue_len(&queue), 0);
}

Test(queue, test_queue_push_pop) {
    Queue queue = queue_init();

    cr_assert_eq(queue_push(&queue, 42), QUEUE_OK);
    cr_assert_eq(queue_len(&queue), 1);

    uint8_t value;
    cr_assert_eq(queue_pop(&queue, &value), QUEUE_OK);
    cr_assert_eq(value, 42);
    cr_assert_eq(queue_len(&queue), 0);
}

Test(queue, test_queue_push_many_success) {
    Queue queue = queue_init();

    uint8_t data[3] = {42, 53, 64};

    cr_assert_eq(queue_push_many(&queue, data, 3), QUEUE_OK);
    cr_assert_eq(queue_len(&queue), 3);
    
    uint8_t result[3];
    cr_assert_eq(queue_pop(&queue, &result[0]), QUEUE_OK);
    cr_assert_eq(result[0], 42);
    cr_assert_eq(queue_pop(&queue, &result[1]), QUEUE_OK);
    cr_assert_eq(result[1], 53);
    cr_assert_eq(queue_pop(&queue, &result[2]), QUEUE_OK);
    cr_assert_eq(result[2], 64);
    cr_assert_eq(queue_len(&queue), 0);
}

Test(queue, test_queue_push_many_fill_and_pop) {
    Queue queue = queue_init();

    uint8_t data[MAX_QUEUE_SIZE];
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        data[i] = i;
    }

    cr_assert_eq(queue_push_many(&queue, data, MAX_QUEUE_SIZE), QUEUE_OK);
    cr_assert_eq(queue_len(&queue), MAX_QUEUE_SIZE);
	cr_assert_eq(queue_full(&queue), true);

    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        uint8_t value;
        cr_assert_eq(queue_pop(&queue, &value), QUEUE_OK);
        cr_assert_eq(value, i);
    }
    cr_assert_eq(queue_empty(&queue), true);
}

Test(queue, test_queue_push_many_full) {
    Queue queue = queue_init();

    uint8_t data[MAX_QUEUE_SIZE];
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        data[i] = i;
    }
    cr_assert_eq(queue_push_many(&queue, data, MAX_QUEUE_SIZE), QUEUE_OK);
    cr_assert_eq(queue_push_many(&queue, data, 1), QUEUE_FULL);
}

Test(queue, test_queue_push_many_too_many) {
    Queue queue = queue_init();

    uint8_t data[MAX_QUEUE_SIZE + 1];
    cr_assert_eq(queue_push_many(&queue, data, MAX_QUEUE_SIZE + 1), QUEUE_TOO_MANY);
}

Test(queue, test_queue_full) {
    Queue queue = queue_init();

    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        cr_assert_eq(queue_push(&queue, i), QUEUE_OK);
    }
    cr_assert_eq(queue_full(&queue), true);

    cr_assert_eq(queue_push(&queue, 99), QUEUE_FULL);
}

Test(queue, test_queue_empty) {
    Queue queue = queue_init();

    cr_assert_eq(queue_empty(&queue), true);
    cr_assert_eq(queue_push(&queue, 42), QUEUE_OK);
    cr_assert_eq(queue_empty(&queue), false);

    uint8_t value;
    cr_assert_eq(queue_pop(&queue, &value), QUEUE_OK);
    cr_assert_eq(queue_empty(&queue), true);
}

Test(queue, test_queue_front_back) {
    Queue queue = queue_init();

    cr_assert_eq(queue_push(&queue, 42), QUEUE_OK);

    uint8_t front_value, back_value;
    cr_assert_eq(queue_front(&queue, &front_value), QUEUE_OK);
    cr_assert_eq(front_value, 42);

    cr_assert_eq(queue_back(&queue, &back_value), QUEUE_OK);
    cr_assert_eq(back_value, 42);
}

Test(queue, test_queue_error_handling) {
    Queue queue = queue_init();

    uint8_t value;

    // Test popping from an empty queue
    cr_assert_eq(queue_pop(&queue, &value), QUEUE_EMPTY);

    // Fill the queue to capacity
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        cr_assert_eq(queue_push(&queue, i), QUEUE_OK);
    }

    // Test pushing to a full queue
    cr_assert_eq(queue_push(&queue, 99), QUEUE_FULL);
}

Test(queue, test_queue_multiple_push_pop) {
    Queue queue = queue_init();

    for (int i = 1; i <= MAX_QUEUE_SIZE; i++) {
        cr_assert_eq(queue_push(&queue, i), QUEUE_OK);
        cr_assert_eq(queue_len(&queue), i);
    }

    for (int i = 1; i <= MAX_QUEUE_SIZE; i++) {
        uint8_t value;
        cr_assert_eq(queue_pop(&queue, &value), QUEUE_OK);
        cr_assert_eq(value, i);
        cr_assert_eq(queue_len(&queue), MAX_QUEUE_SIZE - i);
    }
    cr_assert_eq(queue_empty(&queue), true);
}

Test(queue, test_queue_front_back_after_push_pop) {
    Queue queue = queue_init();

    for (int i = 1; i <= MAX_QUEUE_SIZE; i++) {
        cr_assert_eq(queue_push(&queue, i), QUEUE_OK);
    }

    for (int i = 1; i <= MAX_QUEUE_SIZE; i++) {
        uint8_t front_value, back_value;
        cr_assert_eq(queue_front(&queue, &front_value), QUEUE_OK);
        cr_assert_eq(front_value, i);
        cr_assert_eq(queue_back(&queue, &back_value), QUEUE_OK);
        cr_assert_eq(back_value, MAX_QUEUE_SIZE);
        cr_assert_eq(queue_pop(&queue, NULL), QUEUE_OK);
    }
    cr_assert_eq(queue_empty(&queue), true);
}

Test(queue, test_queue_push_full_and_pop_all) {
    Queue queue = queue_init();

    for (int i = 1; i <= MAX_QUEUE_SIZE; i++) {
        cr_assert_eq(queue_push(&queue, i), QUEUE_OK);
    }
    cr_assert_eq(queue_full(&queue), true);

    uint8_t value;
    for (int i = 1; i <= MAX_QUEUE_SIZE; i++) {
        cr_assert_eq(queue_pop(&queue, &value), QUEUE_OK);
        cr_assert_eq(value, i);
    }
    cr_assert_eq(queue_empty(&queue), true);
}