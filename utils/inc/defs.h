/**
 * @file ./utils/inc/defs.h
 * @author 2022302181113
 * @brief define some macros and macro functions
 * @version 1.0
 * @date 2024-12-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef DEFS_H
#define DEFS_H

    #define ENC_MODE 0
    #define DEC_MODE 1
    #define UN_ASSIGNED -1

    /* start of error code definations */

    // related macro functions
    #define ASK_EXIT_MAIN \
        printf("\nLooks like there's a problem with your input. Do you still want to continue? y or [n]\n"); \
        switch (getchar()) { \
            default: \
                ret = USER_EXIT; \
                goto exit; \
                break; \
            case 'y': \
                break; \
        }

    #define EXIT_MAIN(EXIT_CODE) \
        ret = EXIT_CODE; \
        goto exit;

    // error code marcos
    #define NORMAL_EXIT         0x00000000
    #define USER_EXIT           0x00000001
    #define ERR_CONFLICT_OPT    0x00000002
    #define ERR_MULTPLE_OPT     0x00000003
    #define ERR_UNKNOWN_OPT     0x00000004
    #define ERR_UNKNOWN_ARG     0x00000005
    #define ERR_PARSE_FAIL      0x00000006
    #define ERR_MISS_PARAM      0x00000007
    #define ERR_MISSING_FILE    0x00000008

    /* end of error code definations */

    /* start of cipher_algo definations */

    #define NON_ALGO            0x00    /* 00000000b */

    // asymmetric encryption    - high four bits
    extern const char *G_ASY_NAMES[];   /* defined in ./utils/src/main_callee.c */
    extern const int G_ASY_NAMES_LEN;   /* defined in ./utils/src/main_callee.c */
    #define GET_ASY_BITS(cipher_algo) ((cipher_algo) & 0xF0)
    #define ASY_RSA             0x10    /* 00010000b */

    // symmetric encryption     - low four bits
    extern const char *G_SYM_NAMES[];   /* defined in ./utils/src/main_callee.c */
    extern const int G_SYM_NAMES_LEN;   /* defined in ./utils/src/main_callee.c */
    #define GET_SYM_BITS(cipher_algo) ((cipher_algo) & 0x0F)
    #define SYM_SM4             0x01    /* 00000001b */
    #define SYM_AES             0x02    /* 00000010b */
    #define SYM_RC4             0x03    /* 00000011b */
    #define SYM_ZUC             0x04    /* 00000100b */

    typedef void (*CBC_ENC)(const uint8_t *input, void* subKeys, uint8_t *output);
    typedef void (*CBC_DEC)(const uint8_t *input, void* subKeys, uint8_t *output);

    /* end of cipher_algo definations */

#endif /* !DEFS_H */
