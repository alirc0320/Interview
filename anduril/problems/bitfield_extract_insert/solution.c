#include "solution.h"

/* TODO: implement extract_bits and insert_bits. Watch for:
 *   - `1u << 32` being undefined behavior (count == 32 is a valid input).
 *   - unsigned wraparound when checking start + count > 32.
 *   - not disturbing bits outside the field on insert.
 */

/* Extract bits [start, start+count) from value into *out, right-aligned so
 * *out's bit 0 is value's bit `start`. count == 0 writes 0. */
bool extract_bits(uint32_t value, unsigned start, unsigned count, uint32_t* out)
{
    if (count > 32 || start > 32 - count)
    {
        return false;
    }
    else
    {
        *out = 0;
        uint32_t end_pos = start + count;

        /* Build *out one bit at a time: pull bit i out of value, then OR
         * it into the same position i in *out. */
        for (int i = start; i < end_pos; i++)
        {
            uint32_t temp_val = (value >> i) & 1;
            *out |= temp_val << i - start;
        }
    }

    return true;
}

/*
 * Insert field into bits [start, start+count) of value, result in *out.
 * value is a 32-bit slot with a hole cut out at [start, start+count);
 * field fills that hole, everything else in value stays the same.
 * e.g. value = 0x12345678, insert field = 0xAB at start=8, count=8
 * (the hole holding 0x56) -> out = 0x1234AB78.
 * If field has any bit set at position count or higher (needs more than
 * count bits to represent), returns false and leaves *out unchanged.
 * When count == 0, the only valid field is 0.
 */
bool insert_bits(uint32_t value, uint32_t field, unsigned start, unsigned count, uint32_t* out)
{
    if (count > 32 || start > 32 - count)
    {
        return false;
    }

    uint32_t end_pos = start + count;
    uint32_t mask = 0;

    for (int i = start; i < end_pos; i++)
    {
        mask |= 1u << i;
    }

    /* field must fit entirely within its count low bits, or it doesn't
     * fit in the slot we're inserting it into. */
    if (count < 32 && (field >> count) != 0)
    {
        return false;
    }

    /* Clear the slot in value, then drop field's bits into it, shifted
     * up so field's bit 0 lands at value's bit `start`. */
    *out = (value & ~mask) | (field << start);
    return true;
}
