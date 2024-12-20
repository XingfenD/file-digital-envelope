/**
 * @file ./utils/src/debug.c
 * @brief realize the functions stated in file:./utils/inc/debug.h
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

int debug_global_tag = 1;

void debug_print(const char *format, ...) {
    #ifdef DEBUG_ON
        va_list args;

        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    #endif /* DEBUG_ON */
}

void dbpr_tag() {
    debug_print("tag%d\n", debug_global_tag++);
}

void dbpr_uint8_arr_hex(const uint8_t * uint8_arr, int len) {
    for (int i = 0; i < len; i++) {
        debug_print("0x%02X ", uint8_arr[i]);
    }
    debug_print("\n");
}

// void dbpr_subkey_round(const uint32_t subKeys[32]) {
//     for (int i = 0; i < 32; i++) {
//         debug_print("0x%I32X\n", subKeys[i]);
//     }
// }

void dbpr_uint32_hex(const uint32_t uint32) {
    #if defined(_WIN32) || defined(_WIN64)
        debug_print("0x%I32X\n", uint32);
    #elif defined(__linux__) /* _WIN32 || _WIN64 */
        debug_print("0x%08X\n", uint32);
    #endif /* __linux__ */
}

void dbpr_uint32_bin(uint32_t uint32) {
    for (int i = 0; i < 32; i++) {
        debug_print("%d", (uint32 & 0x80000000) >> 31);
        if ((i + 1) % 8 == 0) {
            debug_print(" ");
        }
        uint32 <<= 1;
    }
    debug_print("\n");
}

void dbpr_int(const char *var_name, int int2pr){
    debug_print("%s == %d\n", var_name, int2pr);
}

// void dbpr_fde_head(const FDE_HEAD fde_head) {
//     debug_print("string: %s\n", fde_head.file_type);
//     debug_print("crypt_alg: ");
//     dbpr_uint32_hex(fde_head.crypt_alg);
//     debug_print("sym_key_len: ");
//     dbpr_uint32_hex(fde_head.sym_key_len);
// }
