#include "solution.h"

/* TODO: implement all five declarations from solution.h.
 *
 * Hints (don't peek until you've tried):
 *   - host_is_little_endian: put a known multi-byte value in a variable,
 *     take its address, reinterpret as a pointer to the smallest byte
 *     type, and look at byte [0].
 *   - swap16/swap32: shifts and masks, no casts to a different-width
 *     type needed.
 *   - host_to_be32/be32_to_host: call host_is_little_endian() and
 *     swap32() only when it's true; otherwise return the value unchanged.
 */
