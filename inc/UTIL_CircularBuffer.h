/**
 * @file UTIL_CircularBuffer.h
 * @author Himel Patel
 * @date 23 Feb 2019
 * @brief Generic Implementation of a Circular Buffer
 *
 */

#ifndef _UTIL_CircularBuffer_H_
#define _UTIL_CircularBuffer_H_

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "Status.h"

/**
 * @brief This structure holds the circular buffers internal state information
 * 
 * A client must allocate this structure and initliase it using `UTIL_CircularBuffer`
 * When using the API functions, a reference to this structure must also be passed in
 * 
 * @note members of this structure should not be accessed directly by the client
 */
typedef struct
{
  uint8_t* array;
  uint32_t front;
  uint32_t rear;
  size_t array_size;
  size_t item_size;
  size_t item_count;
} circular_buffer_t;

/**
 * @brief Initialisation function for a CircularBuffer
 * 
 * @param circular_buffer     allocated circular buffer structure
 * @param allocated_mem       allocated memory used for storing the items
 * @param allocated_mem_size  size of the allocated memory in bytes
 * @param item_size           size in bytes of the item being stored 
 * @return status_code_t      NULL_ARGS if a null parameter is passed in, FAIL if allocated_mem_size
 *                            is not divisible by the item size, otherwise OK
 * 
 * @note the allocated_mem_size must be divisble by the item size
 */
status_code_t UTIL_CircularBuffer(circular_buffer_t* circular_buffer, uint8_t* allocated_mem, size_t allocated_mem_size, size_t item_size);

/**
 * @brief Push an item to the back of the circular buffer
 * 
 * @param circular_buffer allocated circular buffer structure 
 * @param value           pointer to the item being pushed
 * @return status_code_t  NULL_ARGS if a null parameter is passed in, OVERWRITTEN if the circular buffer
 *                        is already full and the oldest item is being replace, otherwise OK
 */
status_code_t UTIL_CircularBuffer_Push(circular_buffer_t* circular_buffer, const void* value);

/**
 * @brief Pop off the first item in the circular buffer 
 * 
 * @param circular_buffer allocated circular buffer structure
 * @param value           pointer which will be assigned the item being popped
 * @return status_code_t  NULL_ARGS if a null parameter is passed in, EMPTY if the circular buffer
 *                        has no items to be popped, otherwise OK
 */
status_code_t UTIL_CircularBuffer_Pop(circular_buffer_t* circular_buffer, void* value);

/**
 * @brief Returns the number of items in the circular buffer
 * 
 * @param circular_buffer allocated circular buffer structure
 * @param size            pointer which will be assigned with the size
 * @return status_code_t  NULL_ARGS if a null parameter is passed in, EMPTY if the circular buffer
 *                        has no items, otherwise OK
 */
status_code_t UTIL_CircularBuffer_Size(circular_buffer_t* circular_buffer, size_t* size);

/**
 * @brief Peek the front of the circular buffer (item will not be removed)
 * 
 * @param circular_buffer allocated circular buffer structure
 * @param value           pointer which will be assigned the front item
 * @return status_code_t  NULL_ARGS if a null parameter is passed in, EMPTY if the circular buffer
 *                        has no items, otherwise OK
 */
status_code_t UTIL_CircularBuffer_Front(circular_buffer_t* circular_buffer, void* value);

/**
 * @brief Peek the back of the circular buffer (item will not be removed) 
 * 
 * @param circular_buffer allocated circular buffer structure 
 * @param value           pointer which will be assigned the rear item
 * @return status_code_t  NULL_ARGS if a null parameter is passed in, EMPTY if the circular buffer
 *                        has no items, otherwise OK
 */
status_code_t UTIL_CircularBuffer_Rear(circular_buffer_t* circular_buffer, void* value);


#ifdef __cplusplus
}
#endif

#endif /*_UTIL_CircularBuffer_H_*/
