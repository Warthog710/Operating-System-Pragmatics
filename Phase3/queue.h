/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Queue Utilities
 */
#ifndef QUEUE_H
#define QUEUE_H

#include "global.h"

// Queue size
#define QUEUE_SIZE PROC_MAX

// Value for empty queue items
#define QUEUE_EMPTY_VALUE -1

// Queue data structure
typedef struct {
    int items[QUEUE_SIZE];  // queue items
    int head;               // queue head
    int tail;               // queue tail
    int size;               // queue size
} queue_t;

/**
 * Function declarations
 */

/**
 * Initializes an empty queue
 * Sets the empty queue items to -1
 *
 * @param  queue - pointer to the queue
 * @return -1 on error; 0 on success
 */
int queue_init(queue_t *queue);

/**
 * Adds an item in to the tail of a queue
 * @param  queue - pointer to the queue
 * @param  item  - the item to add
 * @return -1 on error; 0 on success
 */
int queue_in(queue_t *queue, int item);

/**
 * Pulls an item out from the head of the specified queue
 * @param  queue - pointer to the queue
 * @return -1 on error; 0 on success
 */
int queue_out(queue_t *queue, int *item);

#endif
