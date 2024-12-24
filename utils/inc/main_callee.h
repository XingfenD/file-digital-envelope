/**
 * @file ./utils/inc/main_callee.h
 * @author 2022302181113
 * @brief state the sub functions called by main
 * @version 1.0
 * @date 2024-12-12
 *
 * @copyright Copyright (c) 2024
 *
 * in order to avoid cluttered content within the file:./main.c
 * realize the main subfunctions in this module
 *
 * NOTE: this module shouldn't be included by other modules except for ./main.c
 */

#ifndef MAIN_CALLEE_H
#define MAIN_CALLEE_H

    /**
     * @brief a struct to store output of parse_fde_file
     * @param[out]  crypt_alg                               the field of crypt algorithm
     * @param[out]  crypted_key[length=@crypted_key_len]      the encrypted symmetric key
     * @param[out]  asy_info[length=@asy_info_len]          other infomation used in asymmetric encryption
     * @param[out]  sym_info[length=@sym_info_len]          other infomation used in symmetric encryption
     * @param[out]  crypted_text[length=@crypted_text_len]    the encrypted text
     *
     * @param[out]  crypted_key_len
     * @param[out]  asy_info_len
     * @param[out]  sym_info_len
     * @param[out]  crypted_text_len
     * @note:
     * NOTE: @crypted_key, @crypted_text, @asy_info, @sym_info is not expected to be inited
     * but they should be freed by hand
     */
    typedef struct parse_rst {
        uint8_t crypt_alg;
        uint8_t *crypted_key, *asy_info, *sym_info, *crypted_text;
        unsigned short crypted_key_len, asy_info_len, sym_info_len;
        size_t crypted_text_len;
    } ParseRst;

    /**
     * @brief parse the fde file head and output the @crypted_key and @crypted_text
     * @param[in]   fde_file_path           the path to the fde file
     * @param[out]  file_ext_name           the file extension encoded in fde file
     * @param[out]  parse_rst               more output stored in @ref ParseRst
     * @return int - the status code
     * 0 - returns normally
     *
     * @note
     * NOTE: the caller should malloc 17 bytes to $file_ext_name before calling
     *
     * NOTE: the caller should free the pointer in $parse_rst
     *
     * NOTE: the caller should pass address of two size_t $parse_rst->$crypted_key_len and $crypted_text_len
     *
     * we should malloc and asign an address to two (uint8_t*) $crypted_key and $crypted_text,
     * conseqently, we pass the address of (uint8_t*)
     */
    int parse_fde_file(const char *fde_file_path, char *file_ext_name, ParseRst *parse_rst);

    /**
     * @brief read the bin_file and write into a byte array
     * @param[in]   bin_file_path           path to the bin file
     * @param[out]  bin                     the pointer to a (uint8_t*)
     * @param[out]  bin_len                 the pointer to bin_len
     * @return int - the exit status code
     * @note
     * NOTE: the caller should free the memory of @bin
     */
    int read_bin_file(const char *bin_file_path, uint8_t **bin, size_t *bin_len);

    void fde_print_help();

#endif /* !MAIN_CALLEE_H */
