#include "solution.h"

/* TODO: implement. Use rb->count to distinguish full from empty — with
 * only head == tail as your signal, those two states look identical. */

void ring_init(RingBuffer* rb)
{
    for(int i = 0; i < RING_CAPACITY; i++)
    {
        rb->data[i] = 0;
    }

    rb->count = 0;
    rb->tail = 0;
    rb->head = 0;
}

bool ring_is_empty(const RingBuffer* rb)
{
    return rb->count == 0 ? true : false;
}
bool ring_is_full(const RingBuffer* rb)
{
    return rb->count == RING_CAPACITY ? true : false;
}

bool ring_push(RingBuffer* rb, int value)
{
    if(ring_is_full(rb))
    {
        return false;
    }

    rb->count++;
    rb->data[rb->head] = value;
    rb->head = (rb->head + 1) % RING_CAPACITY;

    return true;
}

bool ring_pop(RingBuffer* rb, int* out)
{
    if(ring_is_empty(rb))
    {
        return false;
    }

    rb->count--;
    *out = rb->data[rb->tail];
    rb->tail = (rb->tail + 1) % RING_CAPACITY;

    return true;
}
