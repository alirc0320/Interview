#pragma once
#include <stdbool.h>
#include <stdint.h>

/* Bit-field extract/insert on a 32-bit word — classic embedded C question
 * (register field access, packet field access, etc).
 *
 * Contract:
 *   - Bits are numbered from 0 = least significant. `start` is the lowest
 *     bit of the field; the field occupies bits [start, start + count).
 *   - If start + count > 32 (including unsigned wraparound), return false
 *     and leave *out unchanged.
 *   - count == 0 is valid: extract_bits writes 0 to *out; insert_bits
 *     writes `value` unchanged to *out, UNLESS `field` != 0, in which case
 *     it must fail (a nonzero field cannot fit in zero bits).
 *   - insert_bits must reject (return false) a `field` that does not fit
 *     in `count` bits, rather than silently truncating it or corrupting
 *     neighbouring bits.
 *   - On any rejection, *out must be left unchanged (do not write garbage
 *     to an out-param on failure — a common embedded API footgun).
 */
bool extract_bits(uint32_t value, unsigned start, unsigned count, uint32_t* out);
bool insert_bits(uint32_t value, uint32_t field, unsigned start, unsigned count, uint32_t* out);
