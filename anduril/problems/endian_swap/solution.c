#include "solution.h"

/* 
 * TODO: implement all five declarations from solution.h.
 */

 /* True if this host stores the least-significant byte of a multi-byte
 * integer at the lowest address (x86, most ARM in default mode). Must be
 * determined at runtime by inspecting memory, not from a compile-time
 * macro like __BYTE_ORDER__ (the point is to show you understand *why*
 * it's true, via a union or pointer cast onto a known value). */
bool host_is_little_endian(void)
{
    uint32_t x = 0xFFFFFFFC;
    unsigned char *p = (unsigned char *)&x;
    return p[0] == 0xFC;
}

uint16_t swap16(uint16_t v)
{
    uint16_t left_v = (uint16_t)(v << 8u);
    uint16_t right_v = (uint16_t)(v >> 8u);
    v = left_v | right_v;
    return v;
}

uint32_t swap32(uint32_t v)
{ 
    uint32_t byte_one = (v >> 24) & 0x000000FF;
    uint32_t byte_two = (v >> 8) & 0x0000FF00;
    uint32_t byte_three = v << 8  & 0x00FF0000;
    uint32_t byte_four = v << 24  & 0xFF000000;
    v = byte_one | byte_two | byte_three | byte_four;
    return v;
}

uint32_t host_to_be32(uint32_t host_value)
{
    if(!host_is_little_endian())
    {
        return host_value;
    } else 
    {
        return swap32(host_value);
    }

}

uint32_t be32_to_host(uint32_t be_value)
{
    if(host_is_little_endian())
    {
        return swap32(be_value);
    } else 
    {
        return be_value;
    }  
}
