#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

/*
  single producer single consumer lockfree ringbuffer<uint8>
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    uint8_t *bufferHead;
    uint8_t *bufferTail;
    uint8_t *head;
    uint8_t *tail;
}ringbuffer_t;

/*
  initialize ringbuffer_t with uint8_t buffer[size]
*/
void     ringbuffer_init(ringbuffer_t *rb,uint8_t *buffer,size_t size);

/*
  clear ringbuffer
  non thread-safe
*/
void     ringbuffer_reset(ringbuffer_t *rb);

/*
  enqueue a uint8_t data
  must called by single thread
  thread-safe
  return true as successful
  return false if ringbuffer is full
*/
bool     ringbuffer_enqueue(ringbuffer_t *rb,uint8_t c);

/*
  dequeue a uint8_t data
  must called by single thread
  thread-safe
  return true as successful
  return false if ringbuffer is empty
*/
bool     ringbuffer_dequeue(ringbuffer_t *rb,uint8_t *c);

#ifdef __cplusplus
}
#endif

#endif





