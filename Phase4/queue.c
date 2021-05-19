/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Spring 2021
 *
 * Queue Utilities
 */

#include "queue.h"
#include "spede.h"
#include "spede/stdio.h"

/**
 * Initializes an empty queue
 * Sets the empty queue items to -1
 *
 * @param  queue - pointer to the queue
 * @return -1 on error; 0 on success
 */
int queue_init(queue_t *queue) 
{
    // Populate all queue items with the QUEUE_EMPTY_VALUE
    int index = 0;    
    for (;index < QUEUE_SIZE; index++)
    {
        queue->items[index] = QUEUE_EMPTY_VALUE;
    }

    // Initialize all queue member values to default
    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;
  
    return 0;
}

/**
 * Adds an item in to the tail of a queue
 * @param  queue - pointer to the queue
 * @param  item  - the item to add
 * @return -1 on error; 0 on success
 */
int queue_in(queue_t *queue, int item) 
{
    // Return an error if the queue is full
    if (queue->size == QUEUE_SIZE) 
    {
        return -1;
    }

    // Add the item to the tail of the queue
    queue->items[queue->tail] = item;

    // Move the tail forward
    // If we are at the end of the array, move the tail to the beginning
    queue->tail = (queue->tail + 1) % QUEUE_SIZE;

    // Increment size (since we just added an item to the queue)
    queue->size++;
    return 0;
}

/**
 * Pulls an item from out from the head of the specified queue
 * @param  queue - pointer to the queue
 * @return -1 on error; 0 on success
 */
int queue_out(queue_t *queue, int *item) 
{
    // return -1 if queue is empty
    if (queue->size == 0)
    {
        return -1;
    }

    // Get the item from the head of the queue
    *item = queue->items[queue->head];    

    // Reset the empty item
    queue->items[queue->head] = QUEUE_EMPTY_VALUE;

    // Move the head forward
    // If we are at the end of the array, move the head to the beginning
    queue->head = (queue->head + 1) % QUEUE_SIZE;

    // Decrement size (since we just removed an item from the queue)
    queue->size--;
    return 0;
}