/**
 * @file ./test/test_debug.c
 * @brief test the function in module:./utils/debug
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <debug.h>
// #include <stdint.h>

int main() {
    uint32_t demo_word = 0x01234567;

    debug_print("the result of pr_uint32_hex: ");
    pr_uint32_hex(demo_word);
    debug_print("the result of pr_uint32_bin: ");
    pr_uint32_bin(demo_word);

    return 0;
}