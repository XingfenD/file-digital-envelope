/**
 * @file ./utils/inc/utils.h
 * @brief state some useful functions and macros
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef UTILS_H
#define UTILS_H

    #include <stdint.h>

    /**
     * @brief the function to show the module:utils is included and compiled correctly
     *
     * // TODO: this function should be removed in the final version of this project
     */
    void utils_print();

    /**
     * @brief the head section structure of fde file
     * @details
     * file_type    - 0x00 ~ 0x02: a string of 3 characters - must be "FDE"
     * crypt_alg    - 0x03 ~ 0x03: high four bits refers the asymmetric encryption, low four bits refers the symmetric encryption
     * sym_key_len  - 0x04 ~ 0x05: bytes-num of the encrypted symmetric key
     *
     * the structure of a fde file
     *
     * head section - this structure                                            - 6 bytes long
     * key section  - the symmetric key ( encrypted by asymmetric encryption )  - @sym_key_len bytes long
     * ciphertext   - the ciphertext encrypted by symmetric key                 - the rest bytes of the file
     */
    typedef struct _fde_head {
        uint8_t file_type[3];   /* a string of 3 characters - must be "FDE" */
        uint8_t crypt_alg;      /* high four bits refers the asymmetric encryption, low four bits refers the symmetric encryption */
        uint16_t sym_key_len;   /* bytes-num of the encrypted symmetric key */
    } FDE_HEAD;

    /**
     * @brief calculate the PKCS#7 padded length of @bytes_to_pad
     * @param[in]   bytes_to_pad            the input bytes to calculate padded length
     * @param[in]   in_len                  the length of @bytes_to_pad
     * @return size_t - the padded length
     */
    size_t pkcs7_padded_len(const uint8_t bytes_to_pad, size_t in_len);

    /**
     * @brief calculate the length of data before applying PKCS#7 padding
     * @param[in]   bytes_to_parse          the input bytes to calculate parsed length
     * @param[in]   in_len                  the length of @bytes_to_parse
     * @return size_t the length before padding
     */
    size_t pkcs7_parsed_len(const uint8_t bytes_to_parse, size_t in_len);

#endif /* !UTILS_H */
