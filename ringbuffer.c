#include "ringbuffer.h"

void ringbuffer_init(ringbuffer_t *rb,uint8_t *buffer,size_t size){
    rb->bufferHead=buffer;
    rb->bufferTail=buffer+size;
    ringbuffer_reset(rb);
}

void ringbuffer_reset(ringbuffer_t *rb){
    rb->head=rb->bufferHead;
    rb->tail=rb->bufferHead;
}

bool  ringbuffer_enqueue(ringbuffer_t *rb,uint8_t c){
    uint8_t *nextHead=rb->head+1;
    if(nextHead==rb->bufferTail){
        nextHead=rb->bufferHead;
    }
    if(nextHead==rb->tail){
        return false;
    }
    *rb->head=c;
    rb->head=nextHead;
    return true;
}

bool  ringbuffer_dequeue(ringbuffer_t *rb,uint8_t *c){
    if(rb->tail != rb->head){
        *c=*rb->tail;
        uint8_t *nextTail=rb->tail+1;
        if(nextTail==rb->bufferTail){
            nextTail=rb->bufferHead;
        }
        rb->tail=nextTail;
        return true;
    }
    return false;
}


