#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* Parse a simple length-prefixed UART-style frame out of a raw byte
 * buffer using pointer arithmetic — this is the shape of GFG Q31-35
 * (serial comms) turned into an actual coding problem, and it also
 * exercises pointers (Q3) and endianness (Q58/Q102) together, which is
 * a realistic combination for a live-coding round.
 *
 * Wire format (all on the wire, no struct-casting the buffer):
 *   byte 0        : start byte, must equal UART_START_BYTE
 *   byte 1..2     : payload length, big-endian uint16_t (byte 1 = high)
 *   byte 3..3+len : payload bytes
 *   byte 3+len    : checksum = XOR of all payload bytes (0 if len == 0)
 *
 * Total frame size is 4 + length bytes.
 */

#define UART_START_BYTE 0x7Eu
#define UART_MAX_PAYLOAD 64u

typedef struct
{
    uint16_t length;
    uint8_t payload[UART_MAX_PAYLOAD];
} UartFrame;

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
size_t uart_parse_frame(const uint8_t* buf, size_t buf_len, UartFrame* frame);
