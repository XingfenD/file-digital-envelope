/**
 * @file ./lib/debug.c
 * @brief realize the functions stated in file:./inc/debug.h
 * @author 2022302181113
 * @version 2.0
 * @date 2024-12-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <debug.h>
#include <stdarg.h>
#include <stdio.h>

    void debug_print(const char *format, ...) {
        #ifdef DEBUG_ON
            va_list args;

            va_start(args, format);
            vprintf(format, args);
            va_end(args);
        #endif /* DEBUG_ON */
    }

    void pr_subkey_word(const unsigned char * word_addr) {
        for (int i = 0; i < 4; i++) {
            debug_print("%02X ", word_addr[i]);
        }
        debug_print("\n");
    }

    void pr_subkey_round(const uint32_t subKeys[32]) {
        for (int i = 0; i < 32; i++) {
            debug_print("0x%I32X\n", subKeys[i]);
        }
    }

    void pr_uint32_hex(const uint32_t uint32) {
        debug_print("0x%I32X\n", uint32);
    }

    void pr_uint32_bin(const uint32_t uint32) {

    }