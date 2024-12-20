#include <sm4.h>
#include <table_defs.h>
#include <stddef.h>
#include <malloc.h>
#include <utils.h>

#define ROTL(value, shift) ((uint32_t)value << shift) | ((uint32_t)value >> (32 - shift))

// uint32_t subkey[32];

static uint32_t T_key(uint32_t dwa)
{
    uint32_t dwt = 0;
    uint32_t dwb = 0;
    uint8_t a[4];
    uint8_t b[4];

    a[0] = (dwa >> 24) & 0xff;
    a[1] = (dwa >> 16) & 0xff;
    a[2] = (dwa >> 8) & 0xff;
    a[3] = dwa & 0xff;
    b[0] = S[a[0]];
    b[1] = S[a[1]];
    b[2] = S[a[2]];
    b[3] = S[a[3]];

    dwt = ((uint32_t)b[0] << 24) ^ ((uint32_t)b[1] << 16) ^ ((uint32_t)b[2] << 8) ^ ((uint32_t)b[3]);
    dwb = dwt ^ (ROTL(dwt, 13)) ^ (ROTL(dwt, 23));
    return dwb;
}

int sm4_make_enc_subkeys(const uint8_t key[SM4_KEY_SIZE], uint32_t encSubKeys[SM4_ROUNDS])
{
    uint32_t MK[4];
    uint32_t k[36];
    uint32_t i = 0;

    MK[0] = key[0] << 24 | key[1] << 16 | key[2] << 8 | key[3];
    MK[1] = key[4] << 24 | key[5] << 16 | key[6] << 8 | key[7];
    MK[2] = key[8] << 24 | key[9] << 16 | key[10] << 8 | key[11];
    MK[3] = key[12] << 24 | key[13] << 16 | key[14] << 8 | key[15];

    k[0] = MK[0] ^ SM4_FK[0];
    k[1] = MK[1] ^ SM4_FK[1];
    k[2] = MK[2] ^ SM4_FK[2];
    k[3] = MK[3] ^ SM4_FK[3];
    for (; i < 32; i++)
    {
        k[i + 4] = k[i] ^ (T_key(k[i + 1] ^ k[i + 2] ^ k[i + 3] ^ SM4_CK[i]));
        encSubKeys[i] = k[i + 4];
        // subkey[i] = encSubKeys[i];
    }
    return 0;
}

int sm4_make_dec_subkeys(const uint8_t key[SM4_KEY_SIZE], uint32_t decSubKeys[SM4_ROUNDS])
{
    uint32_t tmp_subKeys[32];
    sm4_make_enc_subkeys(key, tmp_subKeys);
    for (int i = 0; i < 32; i++)
    {
        decSubKeys[i] = tmp_subKeys[31 - i];
    }
    return 0;
}

void sm4_encrypt_block(const uint8_t *input, const uint32_t encSubKeys[SM4_ROUNDS], uint8_t *output)
{
    uint32_t buff[36];
    uint32_t tmp;
    for (int i = 0; i < 4; i++)
    {
        buff[i] = (input[4 * i + 0] << 24) | (input[4 * i + 1] << 16) | (input[4 * i + 2] << 8) | (input[4 * i + 3]);
    }
    for (int i = 0; i < 32; i++)
    {
        tmp = buff[i + 1] ^ buff[i + 2] ^ buff[i + 3] ^ encSubKeys[i];
        buff[i + 4] = buff[i] ^ Table0[(tmp >> 24) & 0xff] ^ Table1[(tmp >> 16) & 0xff] ^ Table2[(tmp >> 8) & 0xff] ^ Table3[(tmp >> 0) & 0xff];
    }
    for (int i = 0; i < 4; i++)
    {
        output[4 * i] = (buff[35 - i] >> 24) & 0xff;
        output[4 * i + 1] = (buff[35 - i] >> 16) & 0xff;
        output[4 * i + 2] = (buff[35 - i] >> 8) & 0xff;
        output[4 * i + 3] = (buff[35 - i] >> 0) & 0xff;
    }
}

void sm4_decrypt_block(const uint8_t *input, const uint32_t decSubKeys[SM4_ROUNDS], uint8_t *output)
{
    uint32_t buff[36];
    uint32_t tmp;
    for (int i = 0; i < 4; i++)
    {
        buff[i] = (input[4 * i + 0] << 24) | (input[4 * i + 1] << 16) | (input[4 * i + 2] << 8) | (input[4 * i + 3]);
    }
    for (int i = 0; i < 32; i++)
    {
        tmp = buff[i + 1] ^ buff[i + 2] ^ buff[i + 3] ^ decSubKeys[i];
        buff[i + 4] = buff[i] ^ Table0[(tmp >> 24) & 0xff] ^ Table1[(tmp >> 16) & 0xff] ^ Table2[(tmp >> 8) & 0xff] ^ Table3[(tmp >> 0) & 0xff];
    }
    for (int i = 0; i < 4; i++)
    {
        output[4 * i] = (buff[35 - i] >> 24) & 0xff;
        output[4 * i + 1] = (buff[35 - i] >> 16) & 0xff;
        output[4 * i + 2] = (buff[35 - i] >> 8) & 0xff;
        output[4 * i + 3] = (buff[35 - i] >> 0) & 0xff;
    }
}

// void sm4_encrypt_cbc(uint8_t *input, size_t length, uint8_t iv[16], const uint32_t encSubkey[SM4_ROUNDS], uint8_t *output)
// {
//     int i;
//     uint8_t temp[16];
//     while (length > 0)
//     {
//         for (i = 0; i < 16; i++)
//             temp[i] = input[i] ^ iv[i];
//         sm4_encrypt_block(temp, encSubkey, output);
//         memcpy(iv, output, 16);
//         input += 16;  // input+=16;
//         output += 16; // output+=16;
//         length -= 16;
//         ;
//     }
// }

// void sm4_decrypt_cbc(uint8_t *input, size_t length, uint8_t iv[16], const uint32_t decSubkey[SM4_ROUNDS], uint8_t *output)
// {
//     int i;
//     uint8_t temp[16];
//     while (length > 0)
//     {
//         memcpy(temp, input, 16);
//         sm4_decrypt_block(input, decSubkey, output);
//         for (i = 0; i < 16; i++)
//             output[i] = output[i] ^ iv[i];
//         memcpy(output, iv, 16);
//         input += 16;  // input+=16;
//         output += 16; // output+=16;
//         length -= 16;
//     }
// }

// int sm4_padding_encrypt_cbc(uint8_t *input, size_t inlen, uint8_t iv[16], const uint32_t encSubkey[SM4_ROUNDS], uint8_t *output, size_t *outlen)
// {
//     uint8_t block[16];
//     size_t rem = inlen % 16;
//     int padding = 16 - inlen % 16;

//     memcpy(block, input + inlen - rem, rem);
//     memset(block + rem, padding, padding);
//     if (inlen / 16)
//     {
//         sm4_encrypt_cbc(input, inlen, iv, encSubkey, output);
//         output += inlen - rem;
//         iv = output - 16;
//     }
//     sm4_encrypt_cbc(block, 16, iv, encSubkey, output);
//     *outlen = inlen - rem + 16;
//     return 1;
// }
// int sm4_padding_decrypt_cbc(uint8_t *input, size_t inlen, uint8_t iv[16], const uint32_t decSubkey[SM4_ROUNDS], uint8_t *output, size_t *outlen)
// {
//     uint8_t block[16];
//     size_t len = sizeof(block);
//     int padding;

//     if (inlen == 0)
//     {
//         error_puts("warning: input lenght = 0");
//         return 0;
//     }
//     if (inlen % 16 != 0 || inlen < 16)
//     {
//         error_puts("invalid cbc ciphertext length");
//         return -1;
//     }
//     if (inlen > 16)
//     {
//         sm4_decrypt_cbc(input, iv, inlen - 16, decSubkey, output);
//         iv = input + inlen - 32;
//     }
//     sm4_decrypt_cbc(input + inlen - 16, iv, 16, decSubkey, block);
//     padding = block[15];
//     if (padding < 1 || padding > 16)
//     {
//         error_print();
//         return -1;
//     }
//     len -= padding;
//     memcpy(output + inlen - 16, block, len);
//     *outlen = inlen - padding;
//     return 1;
// }

void sm4_padding_encrypt(
    const uint8_t *input, uint8_t **output,
    size_t in_len, size_t *out_len,
    const uint8_t vector[16], const uint8_t key[16]
) {
    /* init the variables and malloc memory */
    *out_len = pkcs7_padded_len(in_len);
    uint8_t *padded_input = malloc(sizeof(uint8_t) * *out_len);
    *output = malloc(sizeof(uint8_t) * *out_len);

    /* pad the input bytes */
    pkcs7_padding(input, in_len, padded_input);

    /* TODO: execute sm4_cbc_encrypt process below */
    /**
     * related varibles
     * @param[in]   padded_input        the bytes to encrypt in cbc mode
     * @param[out]  *output             the encrypt result of cbc
     * @param[in]   *out_len            both the length of @padded_input and @*output
     * @param[in]   vector              the origin xor arg vector (iv)
     */

    free(padded_input);
}

void sm4_padding_decrypt(
    const uint8_t *input, uint8_t *output,
    size_t in_len, size_t *out_len,
    const uint8_t vector[16], const uint8_t key[16]
) {
    /* TODO: execute sm4_cbc_decrypt process below */
    /**
     * related varibles
     * @param[in]   input               the bytes to decrypt in cbc mode
     * @param[out]  output              the decrypt result of cbc
     * @param[in]   in_len              both the length of @input and @output
     * @param[in]   vector              the origin xor arg vector (iv)
     */

    *out_len = pkcs7_parsed_len(output, in_len);
}
