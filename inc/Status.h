/**
 * @file Status.h
 * @author Himel Patel
 * @date 23 Feb 2019
 * @brief return status codes
 *
 */

#ifndef _STATUS_H_
#define _STATUS_H_

#ifdef __cplusplus
  extern "C" {
#endif

/**
 * @brief An enum of status codes
 * 
 */
typedef enum {
  FAIL = -1,
  OK = 0,     
  EMPTY = 1,
  OVERWRITTEN = 2,
  NULL_ARGS = 3
} status_code_t;

#ifdef __cplusplus
}
#endif

#endif /*_STATUS_H_*/