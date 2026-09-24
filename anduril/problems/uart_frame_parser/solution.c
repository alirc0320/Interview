#include "solution.h"
#include <stdbool.h>

/* TODO: implement uart_parse_frame. Walk the buffer with a pointer (or
 * index — your call), reconstruct the big-endian length field by hand
 * (don't reinterpret_cast/memcpy two bytes into a uint16_t — do it with
 * shifts, the way you would if the two bytes weren't adjacent in a
 * struct), then validate length and checksum before touching *frame. */



/* buf/buf_len may contain more than one frame's worth of bytes (only the
 * first frame is parsed) or fewer bytes than a full frame needs.
 *
 * On success: fills *frame and returns the number of bytes consumed
 * (4 + length). On any failure, returns 0 and leaves *frame unspecified:
 *   - buf_len < 1, or buf[0] != UART_START_BYTE
 *   - buf_len < 3 (can't even read the length field)
 *   - declared length > UART_MAX_PAYLOAD
 *   - buf_len < 4 + length (buffer doesn't contain the full frame)
 *   - checksum byte doesn't match the XOR of the payload bytes
 */

//   * Wire format (all on the wire, no struct-casting the buffer):
//  *   byte 0        : start byte, must equal UART_START_BYTE
//  *   byte 1-2      : payload length, big-endian uint16_t (byte 1 = high)
//  *   byte 3-3+len  : payload bytes
//  *   byte 3+len    : checksum = XOR of all payload bytes (0 if len == 0)

size_t uart_parse_frame(const uint8_t* buf, size_t buf_len, UartFrame* frame)
{
    if(buf_len == 0 || buf_len <= 2)
    {
        return 0;
    }

    if(buf[0] != UART_START_BYTE)
    {
        return 0;
    }

    uint16_t payload_length = 0;
    payload_length = (uint16_t)(buf[1]);
    payload_length = (uint16_t)((uint32_t)payload_length << 8u);
    payload_length |= (uint16_t)(buf[2]);

    if(payload_length > UART_MAX_PAYLOAD || buf_len <= payload_length + 3)
    {
        return 0;
    }

    frame->length = payload_length;

    size_t parity_bit = 0;

    for(size_t i = 0; i < payload_length; i++)
    {
        frame->payload[i] = buf[3 + i];
        parity_bit ^= buf[3 + i];
    }

    size_t checksum = buf[3 + payload_length];

    if(parity_bit != checksum && payload_length != 0)
    {
        return 0;
    }

    if(payload_length == 0 && checksum == 0 && parity_bit == 0)
    {
        return 4 + payload_length;
    }

    return 4 + payload_length;
}
