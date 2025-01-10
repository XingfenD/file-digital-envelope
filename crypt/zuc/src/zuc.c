#include <zuc.h>
#include <table_defs.h>
#include <malloc.h>
#include <utils.h>

#define ADD31(a, b) \
    a += (b);       \
    a = (a & 0x7fffffff) + (a >> 31)
#define ROT31(a, k) ((((a) << (k)) | ((a) >> (31 - (k)))) & 0x7FFFFFFF)
#define ROT32(a, k) (((a) << (k)) | ((a) >> (32 - (k))))

#define L1(X)         \
    ((X) ^            \
    ROT32((X), 2) ^   \
    ROT32((X), 10) ^  \
    ROT32((X), 18) ^  \
    ROT32((X), 24))

#define L2(X)         \
    ((X) ^            \
    ROT32((X), 8) ^   \
    ROT32((X), 14) ^  \
    ROT32((X), 22) ^  \
    ROT32((X), 30))

#define LFSRWithInitialisationMode(u) \
    v = LFSR[0];                      \
    ADD31(v, ROT31(LFSR[0], 8));      \
    ADD31(v, ROT31(LFSR[4], 20));     \
    ADD31(v, ROT31(LFSR[10], 21));    \
    ADD31(v, ROT31(LFSR[13], 17));    \
    ADD31(v, ROT31(LFSR[15], 15));    \
    ADD31(v, (u));                    \
    {                                 \
        int j;                        \
        for (j = 0; j < 15; j++)      \
            LFSR[j] = LFSR[j + 1];    \
    }                                 \
    LFSR[15] = v

#define LFSRWithWorkMode()                            \
    {                                                 \
        int j;                                        \
        uint64_t a = LFSR[0];                         \
        a += ((uint64_t)LFSR[0]) << 8;                \
        a += ((uint64_t)LFSR[4]) << 20;               \
        a += ((uint64_t)LFSR[10]) << 21;              \
        a += ((uint64_t)LFSR[13]) << 17;              \
        a += ((uint64_t)LFSR[15]) << 15;              \
        a = (a & 0x7fffffff) + (a >> 31);             \
        v = (uint32_t)((a & 0x7fffffff) + (a >> 31)); \
        for (j = 0; j < 15; j++)                      \
            LFSR[j] = LFSR[j + 1];                    \
        LFSR[15] = v;                                 \
    }

#define BitReconstruction(X0, X1, X2, X3)                          \
    {                                                              \
        X0 = ((LFSR[15] & 0x7fff8000) << 1) | (LFSR[14] & 0xffff); \
        X1 = (LFSR[11] << 16) | (LFSR[9] >> 15);                   \
        X2 = (LFSR[7] << 16) | (LFSR[5] >> 15);                    \
        X3 = (LFSR[2] << 16) | (LFSR[0] >> 15);                    \
    }

#define MAKEU31(k, d, iv)    \
    (((uint32_t)(k) << 23) | \
    ((uint32_t)(d) << 8) |   \
    (uint32_t)(iv))

#define MAKEU32(a, b, c, d)  \
    (((uint32_t)(a) << 24) | \
    ((uint32_t)(b) << 16) |  \
    ((uint32_t)(c) << 8) |   \
    ((uint32_t)(d)))

#define GETU32(p) (((uint32_t)(p)[0] << 24) ^ ((uint32_t)(p)[1] << 16) ^ ((uint32_t)(p)[2] << 8) ^ ((uint32_t)(p)[3]))
#define PUTU32(p, v)                   \
    {                                  \
        (p)[0] = (uint8_t)((v) >> 24); \
        (p)[1] = (uint8_t)((v) >> 16); \
        (p)[2] = (uint8_t)((v) >> 8);  \
        (p)[3] = (uint8_t)(v);         \
    }

#define F(X0, X1, X2)                                                                                   \
    (X0 ^ R1) + R2;                                                                                     \
    W1 = R1 + X1;                                                                                       \
    W2 = R2 ^ X2;                                                                                       \
    u = L1((W1 << 16) | (W2 >> 16));                                                                    \
    v = L2((W2 << 16) | (W1 >> 16));                                                                    \
    R1 = MAKEU32(ZUC_S0[u >> 24], ZUC_S1[(u >> 16) & 0xFF], ZUC_S0[(u >> 8) & 0xFF], ZUC_S1[u & 0xFF]); \
    R2 = MAKEU32(ZUC_S0[v >> 24], ZUC_S1[(v >> 16) & 0xFF], ZUC_S0[(v >> 8) & 0xFF], ZUC_S1[v & 0xFF]);

void zuc_init(const uint8_t *key, const uint8_t *iv)
{
    uint32_t W, W1, W2, u, v, i;
    uint32_t X0, X1, X2, X3;
    for (i = 0; i < 16; i++)
    {
        LFSR[i] = MAKEU31(key[i], KD[i], iv[i]);
    }
    /* R1 R2的初始化 */
    R1 = 0;
    R2 = 0;
    for (i = 0; i < 32; i++)
    {
        BitReconstruction(X0, X1, X2, X3);
        W = F(X0, X1, X2);
        LFSRWithInitialisationMode(W >> 1);
    }
}

uint32_t zuc_generate_keyword()
{
    uint32_t X0, X1, X2, X3;
    uint32_t W1, W2, u, v;
    uint32_t Z;

    BitReconstruction(X0, X1, X2, X3);
    Z = X3 ^ F(X0, X1, X2);
    LFSRWithWorkMode();

    return Z;
}

void zuc_generate_keystream(size_t nwords, uint32_t *keystream)
{
    uint32_t X0, X1, X2, X3;
    uint32_t W, W1, W2, u, v;
    size_t i;

    BitReconstruction(X0, X1, X2, X3);
    F(X0, X1, X2);
    LFSRWithWorkMode();
    for (i = 0; i < nwords; i++)
    {
        keystream[i] = zuc_generate_keyword();
    }
}

/**
 * @brief zuc_encrypt
 * @param input input data
 * @param inlen input data length
 * @param key  key for encryption
 * @param iv   initialization vector
 * @param output  output data
 */
void zuc_encrypt(const uint8_t *input, size_t inlen, const uint8_t *key, const uint8_t *iv, uint8_t *output)
{
    zuc_init(key, iv);
    size_t nwords;
    uint32_t t, n = 0;
    uint8_t *pinput = input;
    if (inlen % 4 == 0)
    {
        nwords = inlen / 4;
    }
    else
    {
        nwords = inlen / 4 + 1;
    }
    uint32_t *keystream = malloc(sizeof(uint32_t) * nwords);
    zuc_generate_keystream(nwords, keystream);
    while (inlen)
    {
        if (inlen >= 4)
        {
            t = GETU32(pinput)^keystream[n];
            PUTU32(output, t);
            inlen -= 4;
            pinput += 4;
            output += 4;
        }
        else
        {
            uint8_t word[4];
            size_t i;
            PUTU32(word, keystream[n]);
            for (i = 0; i < inlen; i++)
            {
                output[i] = pinput[i] ^ word[i];
            }
            break;
        }
        n++;
    }
    free(keystream);
}

/**
 * @brief zuc_decrypt
 * @param input input data
 * @param inlen input data length
 * @param key key for decryption
 * @param iv initialization vector
 * @param output output data
 */
void zuc_decrypt(const uint8_t *input, size_t inlen, const uint8_t *key, const uint8_t *iv, uint8_t *output)
{
    zuc_init(key, iv);
    size_t nwords;
    uint32_t t, n = 0;
    uint8_t *pinput = input;
    if (inlen % 4 == 0)
    {
        nwords = inlen / 4;
    }
    else
    {
        nwords = inlen / 4 + 1;
    }
    uint32_t *keystream = malloc(sizeof(uint32_t) * nwords);
    zuc_generate_keystream(nwords, keystream);
    while (inlen)
    {
        if (inlen >= 4)
        {
            t = GETU32(pinput)^keystream[n];
            PUTU32(output, t);
            inlen -= 4;
            pinput += 4;
            output += 4;
        }
        else
        {
            uint8_t word[4];
            size_t i;
            PUTU32(word, keystream[n]);
            for (i = 0; i < inlen; i++)
            {
                output[i] = pinput[i] ^ word[i];
            }
            break;
        }
        n++;
    }
    free(keystream);
}

