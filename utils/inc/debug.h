/**
 * @file ./utils/inc/debug.h
 * @brief state some debug print functions and related macros
 * @author 2022302181113
 * @version 2.0
 * @date 2024-12-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef DEBUG_H
#define DEBUG_H

    #include <stdint.h>
    #include <utils.h>

    /**
     * the macro to open debug print,
     * you can compile and run the project normally with the marco off
     * without commenting the function calls, but you will be faced at many warning messages
     *
     * this lib encapsulate it's own format print function debug_print()
     * consequently, call debug_print() function if you want a customized output result
     */
    #define DEBUG_ON

    // extern int debug_global_tag;

    /**
     * @brief format debug print function which has similar functions to printf()
     * @param[in]   format              the format string
     * @param[in]   ...                 the args
     */
    void debug_print(const char *format, ...);

    // void advance_debug_print(const char *format, ...);

    /**
     * @brief debug print a global variable tag to display where is the program stuck
     */
    void dbpr_tag();

    /**
     * @brief print a uint8 array in hex from left to right
     * @param[in]   uint8_arr           the array of uint8
     * @param[in]   len                 the length of uint8
     */
    void dbpr_uint8_arr_hex(const uint8_t * uint8_arr, int len);

    /**
     * @brief print all the subkeys directly with new line in the end of a single subkey
     * @param[in]   subKeys
     */
    // void dbpr_subkey_round(const uint32_t subKeys[32]);

    /**
     * @brief print a uint32_t in hex format
     * @param[in]   uint32              the uint32_t to print
     */
    void dbpr_uint32_hex(const uint32_t uint32);

    /**
     * @brief print a uint32_t in bin format with space as seperator every 8 bits
     * @param[in]   uint32              the uint32_t to print
     */
    void dbpr_uint32_bin(uint32_t uint32);

    /**
     * @brief print a integer with prompt message of var_name
     * @param[in]   var_name            the varible name of int2pr
     * @param[in]   int2pr              the integer to print
     */
    void dbpr_int(const char *var_name, int int2pr);

#endif /* !DEBUG_H */
