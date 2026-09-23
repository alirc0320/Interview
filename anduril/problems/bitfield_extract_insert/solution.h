#pragma once
#include <stdbool.h>
#include <stdint.h>

/* Bit-field extract/insert on a 32-bit word (register/packet field access).
 * Bits numbered from 0 = LSB; field occupies [start, start + count).
 * Invalid range (start + count > 32, incl. wraparound) => return false,
 * *out left unchanged.
 */

/* Extract bits [start, start+count) from value into *out. count == 0 writes 0. */
bool extract_bits(uint32_t value, unsigned start, unsigned count, uint32_t* out);

/* Insert field into bits [start, start+count) of value, result in *out.
 * Rejects (false, *out unchanged) a field that doesn't fit in count bits;
 * count == 0 only accepts field == 0.
 */
bool insert_bits(uint32_t value, uint32_t field, unsigned start, unsigned count, uint32_t* out);
