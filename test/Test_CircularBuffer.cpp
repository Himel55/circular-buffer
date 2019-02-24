#include "catch.hpp"
#include "UTIL_CircularBuffer.h"

#define BUFFER_SIZE_BYTES               (20)
#define BUFFER_SIZE_ITEMS               (BUFFER_SIZE_BYTES / sizeof(uint32_t))
#define NONDIVISIBLE_BUFFER_SIZE_BYTES  (11)
#define RANDOM_VALUE                    (108)
#define DIFFERENT_RANDOM_VALUE          (12345)

class Background {
  public:
    circular_buffer_t circular_buffer;
    Background() {
      (void) UTIL_CircularBuffer(&circular_buffer, buffer, BUFFER_SIZE_BYTES, sizeof(uint32_t));      
    }
    void Push(uint32_t value){
      (void) UTIL_CircularBuffer_Push(&circular_buffer, (void*) &value);
    }
    void Fill_with(uint32_t value){
      for(uint8_t i = 0; i < BUFFER_SIZE_ITEMS; i++) {
        Push(value);
      }
    }
    size_t Get_size() {
      size_t size;
      (void) UTIL_CircularBuffer_Size(&circular_buffer, &size);
      return size;
    }
  private:
      uint8_t buffer[BUFFER_SIZE_BYTES];
};

SCENARIO("Initialise") {

  GIVEN("buffer size is NOT divisible by item size") {
    uint8_t buffer[NONDIVISIBLE_BUFFER_SIZE_BYTES];
    circular_buffer_t circular_buffer;
    status_code_t status =  UTIL_CircularBuffer(&circular_buffer, buffer, NONDIVISIBLE_BUFFER_SIZE_BYTES, sizeof(uint32_t));

    THEN("status shall return fail") {
      CHECK(status == FAIL);
    }
  }

  GIVEN("circular buffer is initialised correctly") {
    uint8_t buffer[BUFFER_SIZE_BYTES];
    circular_buffer_t circular_buffer;
    status_code_t status =  UTIL_CircularBuffer(&circular_buffer, buffer, BUFFER_SIZE_BYTES, sizeof(uint32_t));

    THEN("status shall return ok") {
      CHECK(status == OK);
    }
  }
}

SCENARIO("Push/Pop/Size") {
  Background bg;

  GIVEN("a single item is pushed") {
    uint32_t pushed_item = 20;
    status_code_t status =  UTIL_CircularBuffer_Push(&bg.circular_buffer, (const void*) &pushed_item);

    THEN("status shall return ok") {
      CHECK(status == OK);
    }

    THEN("size shall be 1") {
      size_t size = 0;
      UTIL_CircularBuffer_Size(&bg.circular_buffer, &size);
      CHECK(size == 1);
    }

    WHEN("the item is popped") {
      uint32_t popped_item = 0;
      status_code_t status =  UTIL_CircularBuffer_Pop(&bg.circular_buffer, (void*) &popped_item);

      THEN("status shall return ok") {
        CHECK(status == OK);
      }

      THEN("size shall be 0") {
        size_t size = 1;
        UTIL_CircularBuffer_Size(&bg.circular_buffer, &size);
        CHECK(size == 0);
      }

      THEN("the popped item shall equal the pushed item") {
        CHECK(popped_item == pushed_item);
      }
    }
  }
}

SCENARIO("CircularBuffer Empty/Full") {
  Background bg;

  GIVEN("an empty circular buffer is popped") {
    uint32_t popped_item = 0;
    status_code_t status =  UTIL_CircularBuffer_Pop(&bg.circular_buffer, (void*) &popped_item);

    THEN("status shall return empty") {
      CHECK(status == EMPTY);
    }
  }

  GIVEN("size is called on an empty circular buffer") {
    size_t size = 1;
    status_code_t status =  UTIL_CircularBuffer_Size(&bg.circular_buffer, &size);

    THEN("status shall return empty") {
      CHECK(status == EMPTY);
    }

    THEN("size shall be 0") {
      CHECK(size == 0);
    }  
  }

  GIVEN("a circular buffer is filled with the same RANDOM_VALUE") {
    bg.Fill_with(RANDOM_VALUE);
    CHECK(bg.Get_size() == BUFFER_SIZE_ITEMS);

    WHEN("a DIFFERENT_RANDOM_VALUE is pushed on") {
      uint32_t pushed_item = DIFFERENT_RANDOM_VALUE;
      status_code_t status =  UTIL_CircularBuffer_Push(&bg.circular_buffer, (const void*) &pushed_item);

      THEN("status shall return overwritten") {
        CHECK(status == OVERWRITTEN);
      }

      THEN("size shall remain the same") {
        CHECK(bg.Get_size() == BUFFER_SIZE_ITEMS);
      }

      WHEN("the circular buffer is emptied apart from the last item") {
        uint32_t popped_items[BUFFER_SIZE_ITEMS - 1] = {0};
        for(uint8_t i = 0; i < BUFFER_SIZE_ITEMS - 1; i++) {
          (void) UTIL_CircularBuffer_Pop(&bg.circular_buffer, (void*) &popped_items[i]);
        }

        THEN("all items shall equal RANDOM_VALUE") {
          for(uint8_t i = 0; i < BUFFER_SIZE_ITEMS - 1; i++) {
            CHECK(popped_items[i] == RANDOM_VALUE);
          }
        }

        WHEN("the last item is popped")
        {
          uint32_t popped_item = 0;
          (void) UTIL_CircularBuffer_Pop(&bg.circular_buffer, (void*) &popped_item);

          THEN("the item shall equal DIFFERENT_RANDOM_VALUE"){
            CHECK(popped_item == DIFFERENT_RANDOM_VALUE);
          }

          THEN("size shall be 0"){
            CHECK(bg.Get_size() == 0);      
          }        
        }
      }
    }
  }
}

SCENARIO("Front/Back") {
  Background bg;

  GIVEN("the front item is viewed on an empty circular buffer"){
    uint32_t front_item = 0;
    status_code_t status =  UTIL_CircularBuffer_Front(&bg.circular_buffer, (void*) &front_item);
    
    THEN("status shall return empty") {
      CHECK(status == EMPTY);
    }    
  }

  GIVEN("the rear item is viewed on an empty circular buffer"){
    uint32_t rear_item = 0;
    status_code_t status =  UTIL_CircularBuffer_Rear(&bg.circular_buffer, (void*) &rear_item);
    
    THEN("status shall return empty") {
      CHECK(status == EMPTY);
    }    
  }

  GIVEN("RANDOM_VALUE and DIFFERENT_RANDOM_VALUE are pushed on") {
    bg.Push(RANDOM_VALUE);
    bg.Push(DIFFERENT_RANDOM_VALUE);

    WHEN("the front item is viewed") {
      uint32_t front_item = 0;
      status_code_t status =  UTIL_CircularBuffer_Front(&bg.circular_buffer, (void*) &front_item);

      THEN("it shall return RANDOM_VALUE") {
        CHECK(front_item == RANDOM_VALUE);
      }

      THEN("status shall return ok") {
        CHECK(status == OK);
      }
    }

    WHEN("the rear item is viewed") {
      uint32_t rear_item = 0;
      status_code_t status =  UTIL_CircularBuffer_Rear(&bg.circular_buffer, (void*) &rear_item);

      THEN("it shall return DIFFERENT_RANDOM_VALUE") {
        CHECK(rear_item == DIFFERENT_RANDOM_VALUE);
      }

      THEN("status shall return ok") {
        CHECK(status == OK);
      }
    }
  }
}

SCENARIO("Null Checks") {
  GIVEN("NULL arguments are passed into initialise") {
    status_code_t status =  UTIL_CircularBuffer(NULL, NULL, 0, 0);

    THEN("status shall return fail") {
      CHECK(status == NULL_ARGS);
    }
  }

  GIVEN("NULL arguments are passed into push") {
    status_code_t status =  UTIL_CircularBuffer_Push(NULL, NULL);

    THEN("status shall return fail") {
      CHECK(status == NULL_ARGS);
    }
  }

  GIVEN("NULL arguments are passed into pop") {
    status_code_t status =  UTIL_CircularBuffer_Pop(NULL, NULL);

    THEN("status shall return fail") {
      CHECK(status == NULL_ARGS);
    }
  }

  GIVEN("NULL arguments are passed into size") {
    status_code_t status =  UTIL_CircularBuffer_Size(NULL, NULL);

    THEN("status shall return fail") {
      CHECK(status == NULL_ARGS);
    }
  }

  GIVEN("NULL arguments are passed into front") {
    status_code_t status =  UTIL_CircularBuffer_Front(NULL, NULL);

    THEN("status shall return fail") {
      CHECK(status == NULL_ARGS);
    }
  }

  GIVEN("NULL arguments are passed into rear") {
    status_code_t status =  UTIL_CircularBuffer_Rear(NULL, NULL);

    THEN("status shall return fail") {
      CHECK(status == NULL_ARGS);
    }
  }
}

