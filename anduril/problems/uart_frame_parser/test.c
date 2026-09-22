#include "minitest.h"
#include "solution.h"

#include <string.h>

/* payload {0xAA, 0x55, 0x0F}, checksum = 0xAA ^ 0x55 ^ 0x0F = 0xF0 */
static const uint8_t kValidFrame[] = {0x7E, 0x00, 0x03, 0xAA, 0x55, 0x0F, 0xF0};

TEST(parses_a_valid_frame)
{
    UartFrame frame;
    size_t consumed = uart_parse_frame(kValidFrame, sizeof(kValidFrame), &frame);
    CHECK_EQ(consumed, sizeof(kValidFrame));
    CHECK_EQ(frame.length, 3);
    CHECK_EQ(frame.payload[0], 0xAA);
    CHECK_EQ(frame.payload[1], 0x55);
    CHECK_EQ(frame.payload[2], 0x0F);
}

TEST(zero_length_payload_has_zero_checksum)
{
    uint8_t buf[] = {0x7E, 0x00, 0x00, 0x00};
    UartFrame frame;
    size_t consumed = uart_parse_frame(buf, sizeof(buf), &frame);
    CHECK_EQ(consumed, 4);
    CHECK_EQ(frame.length, 0);
}

TEST(rejects_wrong_start_byte)
{
    uint8_t buf[] = {0x00, 0x00, 0x03, 0xAA, 0x55, 0x0F, 0xF0};
    UartFrame frame;
    CHECK_EQ(uart_parse_frame(buf, sizeof(buf), &frame), 0u);
}

TEST(rejects_buffer_too_short_for_length_field)
{
    uint8_t buf[] = {0x7E, 0x00};
    UartFrame frame;
    CHECK_EQ(uart_parse_frame(buf, sizeof(buf), &frame), 0u);
    CHECK_EQ(uart_parse_frame(buf, 0, &frame), 0u);
}

TEST(rejects_length_exceeding_max_payload)
{
    uint8_t buf[] = {0x7E, 0x00, 0x64}; /* declares length 100 > 64 */
    UartFrame frame;
    CHECK_EQ(uart_parse_frame(buf, sizeof(buf), &frame), 0u);
}

TEST(rejects_truncated_frame)
{
    /* Declares length 3 but only supplies 2 payload bytes and no checksum. */
    uint8_t buf[] = {0x7E, 0x00, 0x03, 0xAA, 0x55};
    UartFrame frame;
    CHECK_EQ(uart_parse_frame(buf, sizeof(buf), &frame), 0u);
}

TEST(rejects_bad_checksum)
{
    uint8_t buf[sizeof(kValidFrame)];
    memcpy(buf, kValidFrame, sizeof(kValidFrame));
    buf[sizeof(buf) - 1] ^= 0x01; /* corrupt the checksum byte */
    UartFrame frame;
    CHECK_EQ(uart_parse_frame(buf, sizeof(buf), &frame), 0u);
}

TEST(parses_only_the_first_frame_when_buffer_holds_two)
{
    uint8_t buf[2 * sizeof(kValidFrame)];
    memcpy(buf, kValidFrame, sizeof(kValidFrame));
    memcpy(buf + sizeof(kValidFrame), kValidFrame, sizeof(kValidFrame));

    UartFrame frame;
    size_t consumed = uart_parse_frame(buf, sizeof(buf), &frame);
    CHECK_EQ(consumed, sizeof(kValidFrame));

    /* Caller advances by `consumed` to parse the next frame. */
    size_t consumed2 = uart_parse_frame(buf + consumed, sizeof(buf) - consumed, &frame);
    CHECK_EQ(consumed2, sizeof(kValidFrame));
}

int main(void)
{
    RUN_TEST(parses_a_valid_frame);
    RUN_TEST(zero_length_payload_has_zero_checksum);
    RUN_TEST(rejects_wrong_start_byte);
    RUN_TEST(rejects_buffer_too_short_for_length_field);
    RUN_TEST(rejects_length_exceeding_max_payload);
    RUN_TEST(rejects_truncated_frame);
    RUN_TEST(rejects_bad_checksum);
    RUN_TEST(parses_only_the_first_frame_when_buffer_holds_two);
    TEST_SUMMARY();
    return TEST_EXIT_CODE();
}
