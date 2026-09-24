#pragma once
#include <stdbool.h>
#include <stdint.h>

/* Bit-field extract/insert on a 32-bit word (register/packet field access).
 * Bits numbered from 0 = LSB; field occupies [start, start + count).
 */

/*
 * Extract bits [start, start+count) from value into *out, right-aligned
 * (i.e. *out's bit 0 is value's bit `start`). count == 0 writes 0.
 * Returns false (out untouched) if start + count > 32.
 */
bool extract_bits(uint32_t value, unsigned start, unsigned count, uint32_t* out);

/*
 * Write the low `count` bits of field into bits [start, start+count) of
 * value, leaving every other bit of value unchanged, result in *out.
 * Returns false (out untouched) if start + count > 32, or if field has
 * any bit set at position `count` or higher (needs more than count bits
 * to represent). count == 0 only succeeds when field == 0.
 */
bool insert_bits(uint32_t value, uint32_t field, unsigned start, unsigned count, uint32_t* out);
