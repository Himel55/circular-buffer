#include "UTIL_CircularBuffer.h"
#include "Status.h"
#include <stdio.h>

typedef struct
{
  uint32_t i;
  uint32_t j;
  uint32_t k;
} vector_t;

uint8_t buffer [9 * sizeof(vector_t)];
circular_buffer_t circular_buffer;

static void Fill_vector(vector_t* vector, uint32_t i, uint32_t j, uint32_t k)
{
  vector->i = i;
  vector->j = j;
  vector->k = k;
}

int main (void)
{
  (void) UTIL_CircularBuffer(&circular_buffer,(uint8_t*) &buffer, sizeof(buffer), sizeof(vector_t));
  vector_t vector;

  for(uint8_t i = 0; i <10; i++)
  {
    Fill_vector(&vector, i, i+1, i+2);
    if(UTIL_CircularBuffer_Push(&circular_buffer, &vector) == OVERWRITTEN)
    {
      puts("Overwritten!");
    }
  }

  for(uint8_t i = 0; i <10; i++)
  {
    if (UTIL_CircularBuffer_Pop(&circular_buffer, &vector) == EMPTY)
    {
      puts("Empty!");
    }
    else
    {
      printf("i: %d j: %d k: %d\n", vector.i, vector.j, vector.k);
    }
  }

  return 0;
}