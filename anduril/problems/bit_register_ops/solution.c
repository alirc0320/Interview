#include "solution.h"

/* TODO: implement all five declarations from solution.h using shifts,
 * masks, and the usual |=, &=, ^= idioms — no loops needed for any of
 * these except (optionally) none at all. */


 void reg_set_bit(uint32_t* reg, unsigned bit)
 {
    *reg |= 1u << bit;
 }

 void reg_clear_bit(uint32_t* reg, unsigned bit)
 {
    *reg &= ~(1u << bit);
 }

  void reg_toggle_bit(uint32_t* reg, unsigned bit)
 {
    *reg ^= (1u << bit);
 }

int reg_test_bit(const uint32_t* reg, unsigned bit)
{
    return (*reg >> bit) & 1;
}

/* Set masked bits of *reg to value's; leave the rest untouched. */
void reg_write_masked(uint32_t* reg, uint32_t mask, uint32_t value)
{
    *reg = (*reg & ~mask) | (value & mask);
}
