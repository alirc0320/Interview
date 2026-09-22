#include "solution.h"

/* TODO: implement uart_parse_frame. Walk the buffer with a pointer (or
 * index — your call), reconstruct the big-endian length field by hand
 * (don't reinterpret_cast/memcpy two bytes into a uint16_t — do it with
 * shifts, the way you would if the two bytes weren't adjacent in a
 * struct), then validate length and checksum before touching *frame. */
