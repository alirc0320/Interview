#pragma once
#include <stdbool.h>
#include <stddef.h>

/* Fixed-capacity circular buffer over a statically-sized array — no
 * malloc/free anywhere. This is the shape embedded code actually uses
 * (GFG Q10 "circular buffer", Q16 "static memory allocation"): capacity
 * is a compile-time constant, storage lives inline in the struct so it
 * can be a global/static or stack variable with a known footprint.
 *
 * FIFO semantics: ring_push appends, ring_pop removes the oldest item. */

#define RING_CAPACITY 8

typedef struct
{
    int data[RING_CAPACITY];
    size_t head;  /* index the next push writes to */
    size_t tail;  /* index the next pop reads from */
    size_t count; /* number of valid elements currently stored */
} RingBuffer;

void ring_init(RingBuffer *rb);
bool ring_is_empty(const RingBuffer *rb);
bool ring_is_full(const RingBuffer *rb);

/* Returns false (and leaves rb unchanged) if the buffer is full. */
bool ring_push(RingBuffer *rb, int value);

/* Returns false (and leaves *out unchanged) if the buffer is empty. */
bool ring_pop(RingBuffer *rb, int *out);
