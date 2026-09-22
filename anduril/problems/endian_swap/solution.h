#pragma once
#include <stdbool.h>
#include <stdint.h>

/* Endianness — GFG Q58/Q102, and a near-certain live-coding topic given the
 * team works with hardware registers and wire protocols.
 *
 * Implement all of these WITHOUT arpa/inet.h's htons/htonl (that's the
 * point of the exercise) — pure bit manipulation only. */

/* True if this host stores the least-significant byte of a multi-byte
 * integer at the lowest address (x86, most ARM in default mode). Must be
 * determined at runtime by inspecting memory, not from a compile-time
 * macro like __BYTE_ORDER__ (the point is to show you understand *why*
 * it's true, via a union or pointer cast onto a known value). */
bool host_is_little_endian(void);

/* Reverse the byte order of a 16/32-bit value. Must be an involution:
 * swap16(swap16(x)) == x for all x. */
uint16_t swap16(uint16_t v);
uint32_t swap32(uint32_t v);

/* Convert between host byte order and big-endian ("network") byte order.
 * On a little-endian host these swap; on a big-endian host they are the
 * identity. Both directions use the same function (big-endian <-> host is
 * its own inverse). */
uint32_t host_to_be32(uint32_t host_value);
uint32_t be32_to_host(uint32_t be_value);
