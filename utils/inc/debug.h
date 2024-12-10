/**
 * @file ./inc/debug.h
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

    /**
     * the macro to open debug print,
     * you can compile and run the project normally with the marco off
     * and you need not comment the function calls, you will be faced at many warning messages
     *
     * this lib encapsulate it's own format print function debug_print()
     * consequently, call debug_print() function if you want a customized output result
     */
    #define DEBUG_ON

    /**
     * @brief format debug print function which has similar functions to printf()
     * @param[in]   format              the format string
     * @param[in]   ...                 the args
     */
    void debug_print(const char *format, ...);

    /**
     * @brief print all the subkeys directly with new line in the end of a single subkey
     * @param[in]   subKeys
     */
    void pr_subkey_round(const uint32_t subKeys[32]);

    /**
     * @brief print a uint32_t in hex format
     * @param[in]   uint32              the uint32_t to print
     */
    void pr_uint32_hex(const uint32_t uint32);

#endif /* !DEBUG_H */
