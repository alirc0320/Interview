#pragma once
#include <stdint.h>

/* Set/clear/toggle/test-bit and a masked read-modify-write, the exact
 * idioms used to poke a hardware register (GPIO, peripheral control
 * register, MMIO status word). The parameter is a plain `uint32_t *`
 * here so it's testable with an ordinary local variable; in real
 * firmware the same logic operates on a `volatile uint32_t *` pointing
 * at a fixed memory-mapped address — say in a comment on reg_test_bit
 * why that pointer would need to be `volatile` there but doesn't change
 * anything about the bit arithmetic itself. */

void reg_set_bit(uint32_t *reg, unsigned bit);
void reg_clear_bit(uint32_t *reg, unsigned bit);
void reg_toggle_bit(uint32_t *reg, unsigned bit);

/* Returns 0 or 1. */
int reg_test_bit(const uint32_t *reg, unsigned bit);

/* Read-modify-write: within `mask`, replace the current bits with the
 * corresponding bits of `value`; bits outside `mask` are left untouched.
 * (i.e. *reg = (*reg & ~mask) | (value & mask);) */
void reg_write_masked(uint32_t *reg, uint32_t mask, uint32_t value);
