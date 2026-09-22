---
topic: Endianness
date_written: <YYYY-MM-DD>
---

# Topic: Endianness

Maps to GFG Q58/Q102 (the guide lists this topic twice in the numbered
range — it's a near-guaranteed question). The coding-shaped version of
this topic is `problems/endian_swap` — do it as part of closing this
topic out.

## 1. Concept explanation
Q: Define big-endian and little-endian precisely in terms of byte addresses: for a multi-byte value stored starting at address `A`, which byte (most-significant or least-significant) ends up at the lowest address, in each scheme? Give a worked example: show how the 32-bit value `0x12345678` is laid out in memory (list each address `A`, `A+1`, `A+2`, `A+3` and the byte value stored there) under both big-endian and little-endian.
A:

## 2. Predict the output — detecting endianness
Q:
```c
#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint32_t x = 1;
    unsigned char *p = (unsigned char *)&x;
    printf("%d\n", p[0]);
    return 0;
}
```
A: Explain why this prints `1` on a little-endian machine and `0` on a big-endian one — walk through exactly which byte `p[0]` is looking at and why that byte differs between the two layouts for the value `1`.

## 3. Concept explanation — why it matters in embedded/networking
Q: Give at least three concrete situations where endianness mismatches cause real bugs: (1) parsing a binary wire protocol (e.g. a sensor's datasheet specifies a multi-byte field's byte order, and your MCU's native order might not match it), (2) sharing a struct between two different processors on the same board that don't share the same native endianness, (3) reading a binary file/firmware image format that defines its own byte order independent of the machine writing or reading it. For each, name what "network byte order" conventionally refers to and why picking ONE explicit wire byte order (regardless of what any given host's native order is) is the actual fix, rather than trying to detect and special-case every possible host.
A:

## 4. Debugging / undefined behavior
Q: A teammate "fixes" endianness by punning a struct directly onto a byte buffer instead of doing explicit shifts:
```c
#pragma pack(push, 1)
typedef struct
{
    uint16_t id;
    uint32_t length;
} PacketHeader;
#pragma pack(pop)

void parseHeader(const uint8_t *buf, PacketHeader *out)
{
    *out = *(const PacketHeader *)buf;
}
```
Explain two separate problems with this, independent of `#pragma pack` actually fixing the padding: (1) it does nothing about byte order at all — if the wire format is big-endian and the host is little-endian (or vice versa), the fields come out byte-swapped, silently, with no compiler warning; (2) `*(const PacketHeader *)buf` requires `buf` to be suficiently aligned for `PacketHeader`'s alignment requirement, which isn't guaranteed for an arbitrary `const uint8_t *` into a byte stream — describe what can go wrong on an architecture that faults on unaligned access (tie this back to `12b_pointers_advanced_embedded.md` question 5 in the LearnCpp track if you did that one). Then describe the correct approach: read each field with explicit byte-at-a-time reconstruction (shifts, no struct-casting the raw buffer).
A:

## 5. Implementation
Q: You already implemented `swap16`/`swap32`/`host_to_be32`/`be32_to_host` in `problems/endian_swap`. Without looking at that file, write from memory a function `uint32_t bytesToU32BigEndian(const uint8_t *bytes)` that reconstructs a `uint32_t` from 4 bytes known to be in big-endian (network) order on the wire, using shifts — this should work correctly regardless of the host's own native endianness (i.e. it should give the same answer whether compiled for a little-endian or big-endian target). Explain in one sentence why this function needs no `#ifdef`/runtime endianness check at all, unlike `host_to_be32`.
A:

---

## Corrections
Reopen the material. Fix anything wrong above (don't erase the original
wrong answer — strike it through or note what was wrong, so the mistake
stays visible on review).

-

## Missed on first pass?
- [ ] Yes — copy the missed question(s) into `review/missed_questions.md`
- [ ] No
