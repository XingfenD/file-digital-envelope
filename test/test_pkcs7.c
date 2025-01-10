#include <stdio.h>
#include <utils.h>
#include <debug.h>

int main() {
    uint8_t blocks[] = {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B,
        0x03, 0x03, 0x03, 0x03
    };
    uint8_t pad[32];
    pkcs7_padding(blocks, 16, pad);
    dbpr_uint8_arr_hex(pad, pkcs7_padded_len(16));
    dbpr_int("parsed len", pkcs7_parsed_len(blocks, 32));
    return 0;
}
