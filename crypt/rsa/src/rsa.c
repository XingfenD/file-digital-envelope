#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include "rsa.h"
#include "bignum.h"

 // 读取文件内容到缓冲区，并返回缓冲区指针
// static char* copyFile(const char* filename, long long* file_size) {
//     // 打开文件，"rb" 表示以二进制模式读取文件
//     FILE* file = fopen(filename, "rb");
//     if (file == NULL) {
//         printf("错误：无法打开文件 %s\n", filename);
//         return NULL;
//     }

//     // 移动文件指针到文件末尾，获取文件大小
//     fseek(file, 0, SEEK_END);
//     *file_size = ftell(file);  // 获取当前文件指针的位置，作为文件大小
//     fseek(file, 0, SEEK_SET);   // 将文件指针重新定位到文件开头

//     // 为文件内容分配内存
//     char* buffer = (char*)malloc(*file_size);
//     if (buffer == NULL) {
//         printf("错误：无法为文件分配内存\n");
//         fclose(file);
//         return NULL;
//     }

//     // 读取文件内容到缓冲区
//     size_t bytes_read = fread(buffer, 1, *file_size, file);
//     if (bytes_read != *file_size) {
//         printf("错误：无法完全读取文件内容\n");
//         free(buffer);  // 释放内存
//         fclose(file);
//         return NULL;
//     }

//     // 关闭文件
//     fclose(file);

//     // 返回缓冲区指针，包含了文件的所有内容
//     return buffer;
// }

// static int is_private_pem_exist(const char *key_path)//判断当前文件下有没有private.pem
// {
//     if(fopen(key_path, "r"))//改为你自己的路径
//     {
//         return 1;
//     }
//     else
//     {
//         printf("\n当前文件未找到private.bin这个文件，请用openssl工具生成\n\n");
//         return 0;
//     }
// }

//从文件中读取私钥信息
static void read_private_pem(char* file_buffer, rsa_pkey* privat_key, rsa_key* public_key, rsa_key* private_key)
{
    int i = 0;
    file_buffer += 38;
    if(*file_buffer == 0)
    {
        file_buffer += 1;
    }
    for(i = 0; i < 256; i++)
    {
        privat_key->modulus[i] = file_buffer[i];
    }
    file_buffer += 255;

    file_buffer += 3;
    if(*file_buffer == 0)
    {
        file_buffer += 1;
    }
    for(i = 0; i < 3; i++)
    {
        privat_key->publicExponent[i + 1] = file_buffer[i];
    }
    file_buffer += 2;
    file_buffer += 4;
    if(*file_buffer == 0)
    {
        file_buffer += 1;
    }
    for(i = 0; i < 256; i++)
    {
        privat_key->exponent[i] = file_buffer[i];
    }
    file_buffer += 255;

    public_key->bits = 2048;
    private_key->bits = 2048;

    memcpy(&public_key->exponent[RSA_MAX_MODULUS_LEN-sizeof(privat_key->publicExponent ) ],  privat_key->publicExponent,  sizeof(privat_key->publicExponent ));
    memcpy(&public_key->modulus [RSA_MAX_MODULUS_LEN-sizeof(privat_key->modulus ) ],  privat_key->modulus,  sizeof(privat_key->modulus ));
    memcpy(&private_key->exponent[RSA_MAX_MODULUS_LEN-sizeof(privat_key->exponent ) ],  privat_key->exponent,  sizeof(privat_key->exponent ));
    memcpy(&private_key->modulus [RSA_MAX_MODULUS_LEN-sizeof(privat_key->modulus ) ],  privat_key->modulus,  sizeof(privat_key->modulus ));
}

// //打印私钥信息
// static void prinf_privat_key(rsa_pkey* privat_key)
// {
//     int i = 0;
//     printf("\nmoduls:\n");
//     for(i = 1; i <= 256; i++)
//     {
//         printf("%02x ",privat_key->modulus[i - 1]);
//         if(i % 16 == 0)
//         {
//             printf("\n");
//         }
//     }

//     printf("\npubicExpinent:\n");
//     for(i = 0; i < 4; i++)
//     {
//         printf("%02x ",privat_key->publicExponent[i]);
//     }

//     printf("\nprivateExponent:\n");
//     for(i = 1; i <= 256; i++)
//     {
//         printf("%02x ",privat_key->exponent[i - 1]);
//         if(i % 16 == 0)
//         {
//             printf("\n");
//         }
//     }

//     printf("\nprime1:\n");
//     for(i = 1; i <= 128; i++)
//     {
//         printf("%02x ",privat_key->prime[0][i - 1]);
//         if(i % 16 == 0)
//         {
//             printf("\n");
//         }
//     }

//     printf("\nprime2:\n");
//     for(i = 1; i <= 128; i++)
//     {
//         printf("%02x ",privat_key->prime[1][i - 1]);
//         if(i % 16 == 0)
//         {
//             printf("\n");
//         }
//     }

//     printf("\nexponent1:\n");
//     for(i = 1; i <= 128; i++)
//     {
//         printf("%02x ",privat_key->primeExponent[0][i - 1]);
//         if(i % 16 == 0)
//         {
//             printf("\n");
//         }
//     }

//     printf("\nexponent2:\n");
//     for(i = 1; i <= 128; i++)
//     {
//         printf("%02x ",privat_key->primeExponent[1][i - 1]);
//         if(i % 16 == 0)
//         {
//             printf("\n");
//         }
//     }

//     printf("\ncoefficient:\n");
//     for(i = 1; i <= 128; i++)
//     {
//         printf("%02x ",privat_key->coefficient[i - 1]);
//         if(i % 16 == 0)
//         {
//             printf("\n");
//         }
//     }

// }


// void read_key_file(const char *key_path, uint8_t **key, size_t *key_len) {
//     int i = 0;
//     if(is_private_pem_exist(key_path) == 1)
//     {
//         *key = copyFile(key_path, key_len);
//     }
//     else
//     {
//         system("pause");
//         exit(0);
//     }

// }

static void generate_rand(uint8_t *block, uint32_t block_len) {
    uint32_t i;
    for(i=0; i<block_len; i++) {
        srand ((unsigned)time(NULL));
        block[i] = rand();
    }
}

static int block_operation(uint8_t *out, uint32_t *out_len, uint8_t *in, uint32_t in_len, rsa_key *key);


static int rsa_public_encrypt(uint8_t *out, uint32_t *out_len, uint8_t *in, uint32_t in_len, rsa_key *pk) {
    int status;
    uint8_t byte, pkcs_block[RSA_MAX_MODULUS_LEN];
    uint32_t i, modulus_len;

    modulus_len = (pk->bits + 7) / 8;
    if(in_len + 11 > modulus_len) {
        return ERR_WRONG_LEN;
    }

    pkcs_block[0] = 0;
    pkcs_block[1] = 2;
    for(i=2; i<modulus_len-in_len-1; i++) {
        do {
            generate_rand(&byte, 1);
        } while(byte == 0);
        pkcs_block[i] = byte;
    }

    pkcs_block[i++] = 0;

    memcpy((uint8_t *)&pkcs_block[i], (uint8_t *)in, in_len);
    status = block_operation(out, out_len, pkcs_block, modulus_len, pk);

    // Clear potentially sensitive information
    byte = 0;
    memset((uint8_t *)pkcs_block, 0, sizeof(pkcs_block));

    return status;
}

static int rsa_public_decrypt(uint8_t *out, uint32_t *out_len, uint8_t *in, uint32_t in_len, rsa_key *pk) {
    int status;
    uint8_t pkcs_block[RSA_MAX_MODULUS_LEN];
    uint32_t i, modulus_len, pkcs_block_len;

    modulus_len = (pk->bits + 7) / 8;
    if(in_len > modulus_len)
        return ERR_WRONG_LEN;

    status = block_operation(pkcs_block, &pkcs_block_len, in, in_len, pk);
    if(status != 0)
        return status;

    if(pkcs_block_len != modulus_len)
        return ERR_WRONG_LEN;

    if((pkcs_block[0] != 0) || (pkcs_block[1] != 1))
        return ERR_WRONG_DATA;

    for(i=2; i<modulus_len-1; i++) {
        if(pkcs_block[i] != 0xFF)   break;
    }

    if(pkcs_block[i++] != 0)
        return ERR_WRONG_DATA;

    *out_len = modulus_len - i;
    if(*out_len + 11 > modulus_len)
        return ERR_WRONG_DATA;

    memcpy((uint8_t *)out, (uint8_t *)&pkcs_block[i], *out_len);

    // Clear potentially sensitive information
    memset((uint8_t *)pkcs_block, 0, sizeof(pkcs_block));

    return status;
}

static int rsa_private_encrypt(uint8_t *out, uint32_t *out_len, uint8_t *in, uint32_t in_len, rsa_key *sk) {
    int status;
    uint8_t pkcs_block[RSA_MAX_MODULUS_LEN];
    uint32_t i, modulus_len;

    modulus_len = (sk->bits + 7) / 8;
    if(in_len + 11 > modulus_len)
        return ERR_WRONG_LEN;

    pkcs_block[0] = 0;
    pkcs_block[1] = 1;
    for(i=2; i<modulus_len-in_len-1; i++) {
        pkcs_block[i] = 0xFF;
    }

    pkcs_block[i++] = 0;

    memcpy((uint8_t *)&pkcs_block[i], (uint8_t *)in, in_len);

    status = block_operation(out, out_len, pkcs_block, modulus_len, sk);

    /* Clear potentially sensitive information */
    memset((uint8_t *)pkcs_block, 0, sizeof(pkcs_block));

    return status;
}

static int rsa_private_decrypt(uint8_t *out, uint32_t *out_len, uint8_t *in, uint32_t in_len, rsa_key *sk) {
    int status;
    uint8_t pkcs_block[RSA_MAX_MODULUS_LEN];
    uint32_t i, modulus_len, pkcs_block_len;

    modulus_len = (sk->bits + 7) / 8;
    if(in_len > modulus_len)
        return ERR_WRONG_LEN;

    status = block_operation(pkcs_block, &pkcs_block_len, in, in_len, sk);
    if(status != 0)
        return status;

    if(pkcs_block_len != modulus_len)
        return ERR_WRONG_LEN;

    if((pkcs_block[0] != 0) || (pkcs_block[1] != 2))
        return ERR_WRONG_DATA;

    for(i=2; i<modulus_len-1; i++) {
        if(pkcs_block[i] == 0)  break;
    }

    i++;
    if(i >= modulus_len)
        return ERR_WRONG_DATA;
    *out_len = modulus_len - i;
    if(*out_len + 11 > modulus_len)
        return ERR_WRONG_DATA;
    memcpy((uint8_t *)out, (uint8_t *)&pkcs_block[i], *out_len);
    // Clear potentially sensitive information
    memset((uint8_t *)pkcs_block, 0, sizeof(pkcs_block));

    return status;
}

static int block_operation(uint8_t *out, uint32_t *out_len, uint8_t *in, uint32_t in_len, rsa_key *key) {
    uint32_t edigits, ndigits;
    bn_t c[BN_MAX_DIGITS], e[BN_MAX_DIGITS], m[BN_MAX_DIGITS], n[BN_MAX_DIGITS];

    bn_decode(m, BN_MAX_DIGITS, in, in_len);
    bn_decode(n, BN_MAX_DIGITS, key->modulus, RSA_MAX_MODULUS_LEN);
    bn_decode(e, BN_MAX_DIGITS, key->exponent, RSA_MAX_MODULUS_LEN);

    ndigits = bn_digits(n, BN_MAX_DIGITS);
    edigits = bn_digits(e, BN_MAX_DIGITS);

    if(bn_cmp(m, n, ndigits) >= 0) {
        return ERR_WRONG_DATA;
    }

    bn_mod_exp(c, m, e, edigits, n, ndigits);

    *out_len = (key->bits + 7) / 8;
    bn_encode(out, *out_len, c, ndigits);

    // Clear potentially sensitive information
    memset((uint8_t *)c, 0, sizeof(c));
    memset((uint8_t *)m, 0, sizeof(m));

    return 0;
}

void rsa_encrypt(uint8_t *input, uint8_t *output, size_t in_len, size_t *out_len, uint8_t *key, size_t key_len) {
    rsa_key pk = {0};
    rsa_key sk = {0};
    rsa_pkey pkey = {0};
    char *key_bin = (char*)key;
    char a = *key;
    read_private_pem(key_bin, &pkey, &pk, &sk);

    rsa_public_encrypt(output, out_len, input, in_len, &pk);
}

void rsa_decrypt(uint8_t *input, uint8_t *output, size_t in_len, size_t *out_len, uint8_t *key, size_t key_len) {
    rsa_key pk = {0};
    rsa_key sk = {0};
    rsa_pkey pkey = {0};
    char *key_bin = (char*)key;
    read_private_pem(key_bin, &pkey, &pk, &sk);

    rsa_private_decrypt(output, out_len, input, in_len, &sk);
}