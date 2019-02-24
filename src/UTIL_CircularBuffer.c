#include "UTIL_CircularBuffer.h"
#include <string.h>

status_code_t UTIL_CircularBuffer(circular_buffer_t* circular_buffer, uint8_t* allocated_mem, size_t allocated_mem_size, size_t item_size) {
  if(circular_buffer == NULL || allocated_mem == NULL) {
    return NULL_ARGS;
  }

  if(allocated_mem_size % item_size != 0) {
    return FAIL;
  }

  circular_buffer->array = allocated_mem;
  circular_buffer->front = 0;
  //Push function increments then assigns, so set this up just before overflow 
  circular_buffer->rear = circular_buffer->array_size - item_size;
  circular_buffer->array_size = allocated_mem_size;
  circular_buffer->item_size = item_size;
  circular_buffer->item_count = 0;

  return OK;
}

status_code_t UTIL_CircularBuffer_Push(circular_buffer_t* circular_buffer, const void* value)
{
  if(circular_buffer == NULL || value == NULL) {
    return NULL_ARGS;
  }

  circular_buffer->rear = (circular_buffer->rear + circular_buffer->item_size)%circular_buffer->array_size;
  memcpy((void*) &circular_buffer->array[circular_buffer->rear], value, circular_buffer->item_size);
  
  if(circular_buffer->array_size / circular_buffer->item_size == circular_buffer->item_count) {
    circular_buffer->front = (circular_buffer->front + circular_buffer->item_size)%circular_buffer->array_size;   
    return OVERWRITTEN;
  }

  circular_buffer->item_count++;
  return OK;
}

status_code_t UTIL_CircularBuffer_Pop(circular_buffer_t* circular_buffer, void* value)
{
  if(circular_buffer == NULL || value == NULL) {
    return NULL_ARGS;
  }

  if(circular_buffer->item_count == 0) {
    return EMPTY;
  }

  memcpy(value, (void*) &circular_buffer->array[circular_buffer->front], circular_buffer->item_size);
  circular_buffer->front = (circular_buffer->front + circular_buffer->item_size)%circular_buffer->array_size;
  circular_buffer->item_count--;

  return OK;
}

status_code_t UTIL_CircularBuffer_Size(circular_buffer_t* circular_buffer, size_t* size)
{
  if(circular_buffer == NULL || size == NULL) {
    return NULL_ARGS;
  }

  *size = circular_buffer->item_count;
  return (*size == 0) ? EMPTY : OK;
}

status_code_t UTIL_CircularBuffer_Front(circular_buffer_t* circular_buffer, void* value)
{
  if(circular_buffer == NULL || value == NULL) {
    return NULL_ARGS;
  }

  if(circular_buffer->item_count == 0) {
    return EMPTY;
  }  

  memcpy(value, (void*) &circular_buffer->array[circular_buffer->front], circular_buffer->item_size);
  return OK;
}

status_code_t UTIL_CircularBuffer_Rear(circular_buffer_t* circular_buffer, void* value)
{
  if(circular_buffer == NULL || value == NULL) {
    return NULL_ARGS;
  }

  if(circular_buffer->item_count == 0) {
    return EMPTY;
  }

  memcpy(value, (void*) &circular_buffer->array[circular_buffer->rear], circular_buffer->item_size);
  return OK;
}

