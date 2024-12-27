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
    #include <stddef.h>
    #include <malloc.h>

    /**
     * @brief the function to show the module:utils is included and compiled correctly
     *
     * TODO: this function should be removed in the final version of this project
     *
     */
    void utils_print();

    /**
     * @brief the head section structure of fde file
     * @param file_type     0x00 ~ 0x02: a string of 3 characters - must be "FDE"
     * @param origin_ext    0x03 ~ 0x12: a string of the extension of the original filename
     * @param cipher_algo   0x13 ~ 0x13: high four bits refers the asymmetric encryption, low four bits refers the symmetric encryption
     * @param asy_info_len  0x14 ~ 0x15: bytes-num of other infomation used in asymmetric encryption
     * @param sym_info_len  0x16 ~ 0x17: bytes-num of other infomation used in symmetric encryption
     * @param sym_key_len   0x18 ~ 0x19: bytes-num of the encrypted symmetric key
     * @param reserved      0x1A ~ 0x1F: reserved bytes for extension
     * @details
     * the structure of a fde file
     *
     * head section - this structure                                            - 26 bytes long
     * key section  - the symmetric key ( encrypted by asymmetric encryption )  - @sym_key_len bytes long
     * info section - the other infomation used in encrypt algorithm            - @sym_info_len + @asy_info_len bytes long
     * ciphertext   - the ciphertext encrypted by symmetric key                 - the rest bytes of the file
     */
    typedef struct _fde_head {
        uint8_t file_type[3];   /* a string of 3 characters - must be "FDE" */
        uint8_t origin_ext[16]; /* the orgin file name' extension */
        uint8_t cipher_algo;      /* high four bits refers the asymmetric encryption, low four bits refers the symmetric encryption */
        uint16_t asy_info_len;  /* bytes-num of other infomation used in asymmetric encryption */
        uint16_t sym_info_len;  /* bytes-num of other infomation used in symmetric encryption */
        uint16_t sym_key_len;   /* bytes-num of the encrypted symmetric key */
        uint8_t reserved[6];    /* reserved for futural extension */
    } FDE_HEAD;


    /* start of pkcs7 function statements */
    /* function names in this module start with pkcs7_ */
    #ifdef UITILS_PKCS7_MOD
    #error "Macro UITILS_PKCS7_MOD already defined"
    #endif/* UITILS_PKCS7_MOD */
    #define UITILS_PKCS7_MOD
    #ifdef UITILS_PKCS7_MOD
        /* NOTE: the pkcs7 block size is 16 */

        /**
         * @brief calculate the PKCS#7 padded length of @bytes_to_pad
         * @param[in]   in_len                  the length of @bytes_to_pad
         * @return size_t - the padded length
         */
        size_t pkcs7_padded_len(size_t in_len);

        /**
         * @brief calculate the length of data before applying PKCS#7 padding
         * @param[in]   bytes_to_parse          the input bytes to calculate parsed length
         * @param[in]   in_len                  the length of @bytes_to_parse
         * @return size_t the length before padding
         */
        size_t pkcs7_parsed_len(const uint8_t *bytes_to_parse, size_t in_len);

        /**
         * @brief padding a bytes array using PKCS#7 (block_size = 16)
         * @param[in]   input[length=@in_len]   the bytes to be padded
         * @param[in]   in_len                  the length of input
         * @param[out]  output                  the result of padding
         * @note
         * NOTE: the caller should malloc @out_len bytes to the @output
         *
         * use function pkcs7_padded_len to calculate @out_len mentioned above
         */
        void pkcs7_padding(const uint8_t *input, const size_t in_len, uint8_t *output);

    #endif /* UITILS_PKCS7_MOD */
    /* end of pkcs7 function statements */


    /* start of advanced string function statements */
    /* function names in this module start with str_ */
    #ifdef UITILS_STR_MOD
    #error "Macro UITILS_STR_MOD already defined"
    #endif/* UITILS_STR_MOD */
    #define UITILS_STR_MOD
    #ifdef UITILS_STR_MOD

        /**
         * @brief add @src to the end of @dst and store the result to a new string
         * @param[in]   dst                     the start part of result
         * @param[in]   src                     the end part of result
         * @return char* - the strcat result
         * @note
         * the copyed string ends with '\0'
         *
         * NOTE: the caller should free the memory
         */
        // char* str_malloc_cat(const char *dst, const char *src);

        /**
         * @brief malloc the same memory size and copy the content from @src to dst(return)
         * @param[in]   src                     the src string to be copyed
         * @return char* - the returned address of the copy of @src
         * @note
         * NOTE: the caller should free the memory
         *
         * the copyed string ends with '\0'
         */
        char *str_malloc_cpy(const char *src);

        /**
         * @brief replace the extension of @origin_file_name with @new_ext_name
         * @param[in]   origin_file_name        the origin file name to replace
         * @param[in]   new_ext_name            the new extension
         * @return char* - the replaced file name
         * @note
         * NOTE: the caller should free the memory
         *
         * the returned string ends with '\0'
         *
         * if the input @origin_file_name does not have an extension,
         * this function will add an extension to it
         */
        char *str_rep_ext(const char *origin_file_name, const char *new_ext_name);

        /**
         * @brief find a string in strArr
         * @param[in]   str2find        the string to find
         * @param[in]   strArr          array of strings
         * @param[in]   size            size of the $strArr
         * @return int - the index of $str2find in $strArr, -1 refers not found
         */
        // int str_find(const char str2find[], const char *strArr[], const int size);

    #endif /* UITILS_STR_MOD */
    /* end of advanced string function statements */

    /**
     * @brief random and malloc a bytes array
     * @param[out]  byte2random     the output of random bytes
     * @param[in]   bytes_num       number of bytes to generate randomly
     * @note
     * NOTE: the caller should malloc $bytes_num bytes memory to $bytes2random
     */
    void random_bytes(uint8_t *bytes2random, int bytes_num);

#endif /* !UTILS_H */
