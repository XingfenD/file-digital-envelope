/**
 * @file ./utils/src/utils.c
 * @brief realize the functions stated in file:./utils/inc/utils.h
 * @author 202230218113
 * @version 1.0
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <debug.h>
#include <utils.h>
#include <string.h>

void utils_print()
{
    // TODO: this function should be removed in the final version of this project
    debug_print("this is function ./utils/src/utils.c:utils_print()\n");
}

/* start of pkcs7 function definations */

size_t pkcs7_padded_len(size_t in_len)
{
    /* find the smallest multiple of 16 greater than @in_len */
    if (in_len % 16 == 0)
    {
        return in_len + 16;
    }
    else
    {
        return ((in_len + 15) >> 4) << 4;
    }
}

size_t pkcs7_parsed_len(const uint8_t *bytes_to_parse, size_t in_len)
{
    /* read the last byte in the array */
    int padded_bytes_num = bytes_to_parse[(int)in_len - 1];
    for (int i = in_len - 1; i >= (int)(in_len - padded_bytes_num); i--)
    {
        if ((int)bytes_to_parse[i] != padded_bytes_num)
        {
            return in_len;
        }
    }

    return in_len - padded_bytes_num;
}

void pkcs7_padding(const uint8_t *input, const size_t in_len, uint8_t *output)
{
    memcpy(output, input, in_len);
    int padded_len = pkcs7_padded_len(in_len);
    uint8_t pad_byte = padded_len - in_len;
    for (int i = in_len; i < padded_len; i++)
    {
        output[i] = pad_byte;
    }
}

/* end of pkcs7 function definations */

/* start of advanced string function definations */

static char *str_malloc_cat(const char *dst, const char *src)
{
    int dst_len = strlen(dst);
    char *ret = malloc(sizeof(char) * (dst_len + strlen(src) + 1));

    strcpy(ret, dst);
    strcpy(ret + dst_len, src);

    return ret;
}

char *str_malloc_cpy(const char *src)
{
    int len_of_src = strlen(src);
    char *dst;

    dst = (char *)malloc(sizeof(char) * (len_of_src + 1));
    strcpy(dst, src);

    return dst;
}
/**
 * @brief
 * @param origin_file_name
 * @param new_ext_name
 * @return
 */
char *str_rep_ext(const char *origin_file_name, const char *new_ext_name)
{
    char *p_dot = NULL;
    char *ret = NULL;
    if ((p_dot = strchr(origin_file_name, '.')) == NULL)
    {
        ret = str_malloc_cat(origin_file_name, new_ext_name);
    }
    else
    {
        ret = calloc((int)(p_dot - origin_file_name) + 1 + strlen(new_ext_name) + 1, sizeof(char));
        memcpy(ret, origin_file_name, (p_dot - origin_file_name) + 1);
        strcat(ret, new_ext_name);
    }

    return ret;
}

/* end of advanced string function definations */

/* start of encrypt&decrypt mode function definations */

/**
 * @brief
 * @param
 * @param
 * @param iv
 * @param input
 * @param nblocks
 * @param output
 */
void cbc_encrypt_blocks(enum algomode mode, const *key, uint8_t *iv, const uint8_t *input, size_t nblocks, uint8_t *output)
{
    uint32_t block_size;
    uint8_t *pinput = input;
    uint8_t *temp = malloc(sizeof(uint8_t) * 16);
    switch (mode)
    {
    case AES:
        /* code */
        block_size = 16;
        while (nblocks--)
        {
            for (size_t i = 0; i < block_size; i++)
            {
                temp[i] = pinput[i] ^ iv[i];
            }
            sm4_encrypt_block(temp, key, output);
            iv = output;
            pinput += block_size;
            output += block_size;
        }
        break;
    case DES:
        /* code */
        block_size = 8;
        while (nblocks--)
        {
            for (size_t i = 0; i < block_size; i++)
            {
                temp[i] = pinput[i] ^ iv[i];
            }
            sm4_encrypt_block(temp, key, output);
            iv = output;
            pinput += block_size;
            output += block_size;
        }
        break;
    case SM4:
        block_size = 16;
        print_bytes(pinput, 16 * 2);
        while (nblocks--)
        {
            for (size_t i = 0; i < block_size; i++)
            {
                temp[i] = pinput[i] ^ iv[i];
            }
            sm4_encrypt_block(temp, key, output);
            iv = output;
            pinput += block_size;
            output += block_size;
        }
        break;
    default:
        break;
    }
}

/**
 * @brief
 * @param
 * @param
 * @param iv
 * @param input
 * @param nblocks
 * @param output
 */
void cbc_decrypt_blocks(enum algomode mode, const *key, uint8_t *iv, const uint8_t *input, size_t nblocks, uint8_t *output)
{
    uint32_t block_size;
    uint8_t *pinput = input;
    switch (mode)
    {
    case AES:
        /* code */
        block_size = 16;
        while (nblocks--)
        {
            sm4_encrypt_block(pinput, key, output);
            for (size_t i = 0; i < block_size; i++)
            {
                output[i] = output[i] ^ iv[i];
            }
            iv = pinput;
            pinput += block_size;
            output += block_size;
        }
        break;
    case DES:
        /* code */
        block_size = 8;
        while (nblocks--)
        {
            sm4_encrypt_block(pinput, key, output);
            for (size_t i = 0; i < block_size; i++)
            {
                output[i] = output[i] ^ iv[i];
            }
            iv = pinput;
            pinput += block_size;
            output += block_size;
        }
        break;
    case SM4:
        block_size = 16;
        while (nblocks--)
        {
            sm4_encrypt_block(pinput, key, output);
            for (size_t i = 0; i < block_size; i++)
            {
                output[i] = output[i] ^ iv[i];
            }
            iv = pinput;
            pinput += block_size;
            output += block_size;
        }
        break;
    default:
        break;
    }
}
