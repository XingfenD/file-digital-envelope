/**
 * @file ./main.c
 * @brief the main function of FDE tool
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <getopt.h>
#include <utils.h>
#include <defs.h>
#include <string.h>
#include <main_callee.h>
#include <crypt.h>
#include <time.h>
#include <stdlib.h>
#include <debug.h>

const char *G_SYM_NAMES[] = {
    "sm4", "aes", "rc4", "zuc"
};

const char *G_ASY_NAMES[] = {
    "rsa"
};

const int G_SYM_NAMES_LEN = sizeof(G_SYM_NAMES) / sizeof(char*);
const int G_ASY_NAMES_LEN = sizeof(G_ASY_NAMES) / sizeof(char*);

/* TODO: reconstruct the main function */
int main(int argc, char *argv[]) {
    int opt;
    int mode = UN_ASSIGNED;
    int ret = NORMAL_EXIT;
    uint8_t cipher_algo = 0x00;
    char *infile_path = NULL, *outfile_path = NULL, *keyfile_path = NULL;

    /* TODO: realize an advanced args parsing with dependency and exclusion */
    /* parse the command line input */
    while ((opt = getopt(argc, argv, "f:o:k:m:a:s:h")) != -1) {
        switch (opt) {
            case 'f':
                if (infile_path == NULL) {
                    printf("Input file: %s\n", optarg);
                    infile_path = optarg;
                } else {
                    printf("Multiple options: -%c\n", opt);
                    // ret = ERR_MULTPLE_OPT;
                    // goto exit;
                    EXIT_MAIN(ERR_MULTPLE_OPT);
                }
                break;
            case 'o':
                if (outfile_path == NULL) {
                    printf("Output file: %s\n", optarg);
                    outfile_path = optarg;
                } else {
                    printf("Multiple options: -%c\n", opt);
                    EXIT_MAIN(ERR_MULTPLE_OPT);
                }
                break;
            case 'k':
                if (keyfile_path == NULL) {
                    printf("Key file: %s\n", optarg);
                    keyfile_path = optarg;
                } else {
                    printf("Multiple options: -%c\n", opt);
                    EXIT_MAIN(ERR_MULTPLE_OPT);
                }
                break;
            case 'm':
                if (strcmp(optarg, "enc") == 0) {
                    if (mode == UN_ASSIGNED) {
                        mode = ENC_MODE;
                        printf("Encrypt mode\n");
                    } else {
                        printf("Conflict options: -m dec and -m enc\n");
                        EXIT_MAIN(ERR_CONFLICT_OPT);
                    }
                } else if (strcmp(optarg, "dec") == 0) {
                    if (mode == UN_ASSIGNED) {
                        mode = DEC_MODE;
                        printf("Decrypt mode\n");
                    } else {
                        printf("Conflict options: -m enc and -m dec\n");
                        EXIT_MAIN(ERR_CONFLICT_OPT);
                    }
                } else {
                    printf("Unknown option argument: %s\n", optarg);
                    EXIT_MAIN(ERR_UNKNOWN_ARG)
                }
                break;
            case 'h':
                fde_print_help();
                EXIT_MAIN(NORMAL_EXIT)
                break;
            case 'a':
                for (uint8_t i = 0; i < G_ASY_NAMES_LEN; i++) {
                    if (strcmp(optarg, G_ASY_NAMES[i]) == 0) {
                        cipher_algo |= GET_ASY_BITS((i + 1) << 4);
                    }
                }
                break;
            case 's':
                for (uint8_t i = 0; i < G_SYM_NAMES_LEN; i++) {
                    if (strcmp(optarg, G_SYM_NAMES[i]) == 0) {
                        cipher_algo |= GET_SYM_BITS(i + 1);
                    }
                }
                break;
            case '?':
                printf("Invalid option: %c\n", optopt);
                printf("Unknown option: %c\n", opt);
                EXIT_MAIN(ERR_UNKNOWN_OPT)
                break;
        }
    }

    /* start of handling parameter relationships */

    /* missing inputfile handling */
    if (infile_path == NULL) {
        printf("Required parameters are not specified: -f");
        EXIT_MAIN(ERR_MISS_PARAM)
    }

    /* missing keyfile handling */
    if (keyfile_path == NULL) {
        printf("Required parameters are not specified: -k");
        EXIT_MAIN(ERR_MISS_PARAM)
    }

    /* missing running mode handling */
    if (mode == UN_ASSIGNED) {
        printf("Required parameters are not specified: -m");
        EXIT_MAIN(ERR_MISS_PARAM)
    }

    /* encryption mode requires specifying a password algorithm */
    if (mode == ENC_MODE) {
        if (!GET_ASY_BITS(cipher_algo)) {
            printf("Required parameters are not specified: -a");
            EXIT_MAIN(ERR_MISS_PARAM)
        }
        if (!GET_SYM_BITS(cipher_algo)) {
            printf("Required parameters are not specified: -s");
            EXIT_MAIN(ERR_MISS_PARAM)
        }
    }

    /* extra arguments handling */
    if (optind < argc) {
        printf("Remaining arguments: ");
        for (; optind < argc; optind++) {
            printf("%s ", argv[optind]);
        }
        ASK_EXIT_MAIN
    } /* optind < argc */

    /* end of handling parameter relationships */

    /* execute main function according to the args input */
    if (mode == ENC_MODE) {
        uint8_t *plain_text = NULL, *cipher_text = NULL;
        uint8_t *asy_info = NULL, *sym_info = NULL;
        uint8_t *sym_key = NULL, *pub_key = NULL;
        uint8_t *cipher_sym_key = NULL;
        size_t plain_text_len, cipher_text_len, pub_key_len;
        size_t cipher_sym_key_len;
        FILE *outfile = NULL;
        FDE_HEAD fde_head = {};

        /**
         * @brief assign basic value to fde_head
         * [*] file_type
         * [*] origin_ext
         * [*] cipher_algo
         * [*] asy_info_len
         * [ ] sym_info_len
         * [ ] sym_key_len - will be assigned in symmtric switch block
         */
        memcpy(fde_head.file_type, "FDE", 3);
        fde_head.cipher_algo = cipher_algo;
        char *pdot = strrchr(infile_path, '.');
        if (NULL == pdot) { /* if the file does not have extension */
            for (int i = 0; i < 16; i++) {
                fde_head.origin_ext[i] = '\0';
            }
        } else { /* if the file does have extension */
            /* NOTE: may have bug */
            memcpy(fde_head.origin_ext, pdot + 1, strlen(pdot) + 1);
            // strcpy((char *) fde_head.origin_ext, pdot + 1);
        }
        fde_head.asy_info_len = 0;

        /* init default arg */
        if (outfile_path == NULL) {
            /* set the output file name if the path are not specified */
            outfile_path = str_rep_ext(infile_path, "fde");
        }

        /* TODO: check the return code of the functions below */

        // outfile = fopen(outfile_path, "wb");
        read_bin_file(infile_path, &plain_text, &plain_text_len);
        read_bin_file(keyfile_path, &pub_key, &pub_key_len);

        srand((unsigned)time(NULL));

        switch (GET_SYM_BITS(cipher_algo))
        {
        case SYM_SM4:
            // fde_head.sym_key_len = 16;
            fde_head.sym_info_len = 16;
            sym_info = malloc(fde_head.sym_info_len * sizeof(uint8_t));
            sym_key = malloc(16 * sizeof(uint8_t));
            random_bytes(sym_info, fde_head.sym_info_len);
            random_bytes(sym_key, 16);

            dbpr_uint8_arr_hex(sym_key, 16);
            dbpr_uint8_arr_hex(sym_info, 16);

            sm4_padding_encrypt(
                plain_text, &cipher_text,
                plain_text_len, &cipher_text_len,
                sym_info, sym_key
            );
            break;
        case SYM_AES:
            fde_head.sym_info_len = 16;
            sym_info = malloc(fde_head.sym_info_len * sizeof(uint8_t));
            sym_key = malloc(16 * sizeof(uint8_t));
            random_bytes(sym_info, fde_head.sym_info_len);
            random_bytes(sym_key, 16);
            aes_padding_encrypt(
                plain_text, &cipher_text,
                plain_text_len, &cipher_text_len,
                sym_info, sym_key
            );
            break;
        case SYM_RC4:
            fde_head.sym_info_len = 0;
            // sym_info = malloc(fde_head.sym_info_len * sizeof(uint8_t));
            cipher_text = malloc(plain_text_len * sizeof(uint8_t));
            sym_key = malloc(16 * sizeof(uint8_t));
            // random_bytes(sym_info, fde_head.sym_info_len);
            random_bytes(sym_key, 16);
            rc4_encrypt(
                plain_text, plain_text_len,
                sym_key, cipher_text
            );
            break;
        case SYM_ZUC:
            fde_head.sym_info_len = 16;
            sym_info = malloc(fde_head.sym_info_len * sizeof(uint8_t));
            sym_key = malloc(16 * sizeof(uint8_t));
            cipher_text = malloc(plain_text_len * sizeof(uint8_t));
            random_bytes(sym_info, fde_head.sym_info_len);
            random_bytes(sym_key, 16);
            zuc_encrypt(
                plain_text, plain_text_len,
                sym_key, sym_info, cipher_text
            );
            break;
        default:
            break;
        }

        switch (GET_ASY_BITS(cipher_algo))
        {
        case ASY_RSA:
            fde_head.sym_key_len = 256;
            cipher_sym_key = malloc(fde_head.sym_key_len * sizeof(uint8_t));
            rsa_encrypt(sym_key, cipher_sym_key, 16, &cipher_sym_key_len, pub_key, pub_key_len);
            break;
        }


        outfile = fopen(outfile_path, "wb");
        fwrite(&fde_head, 1, sizeof(fde_head), outfile);
        fwrite(cipher_sym_key, 1, 256, outfile);
        fwrite(asy_info, 1, fde_head.asy_info_len, outfile);
        fwrite(sym_info, 1, fde_head.sym_info_len, outfile);
        fwrite(cipher_text, 1, cipher_text_len, outfile);

        fclose(outfile);

        free(cipher_sym_key);
        free(sym_key); /* malloced in symmetric encryption */
        free(sym_info); /* malloced in symmetric encryption */
        free(asy_info); /* malloced in asymmetric encryption */
        free(cipher_text); /* malloced in padding encrypt */
        free(plain_text); /* malloced in read_bin_file */
        free(pub_key);  /* malloced in read_bin_file */
    } else if (mode == DEC_MODE) { /* if (mode == ENC_MODE) */
        char ext_name[17];

        ParseRst parse_rst = {}; /* the return value of function:./utils/main_callee::parse_fde_file() */

        uint8_t *pub_key = NULL, *sym_key = NULL, *decrypt_text = NULL;
        size_t pub_key_len, sym_key_len, decrypt_text_len;

        /* parse the fde head and read body */
        parse_fde_file(infile_path, ext_name, &parse_rst);

        /* malloc the memory to decrypt_text and sym_key */
        decrypt_text = malloc(sizeof(uint8_t) * parse_rst.crypted_text_len);
        sym_key = malloc(sizeof(uint8_t) * parse_rst.crypted_key_len);

        /* decrypt the key using asymmetric encrypt algorithm */
        switch (GET_ASY_BITS(parse_rst.cipher_algo))
        {
        case ASY_RSA:
            /* TODO: check the return code of the functions below */
            /* decrypt the sym-key */
            read_bin_file(keyfile_path, &pub_key, &pub_key_len);
            dbpr_uint8_arr_hex(sym_key, 16);
            rsa_decrypt(
                parse_rst.crypted_key, sym_key,
                parse_rst.crypted_key_len, &sym_key_len,
                pub_key, pub_key_len
            );
            dbpr_uint8_arr_hex(sym_key, 16);
            // sym_key_len = parse_rst.crypted_key_len;
            break;

        }

        /* decrypt the key using symmetric encrypt algorithm */
        switch (GET_SYM_BITS(parse_rst.cipher_algo))
        {
        case SYM_SM4:
            sm4_padding_decrypt(
                parse_rst.crypted_text, decrypt_text,
                parse_rst.crypted_text_len, &decrypt_text_len,
                parse_rst.sym_info, sym_key
            );
            break;
        case SYM_AES:
            aes_padding_decrypt(
                parse_rst.crypted_text, decrypt_text,
                parse_rst.crypted_text_len, &decrypt_text_len,
                parse_rst.sym_info, sym_key
            );
            break;
        case SYM_RC4:
            rc4_decrypt(
                parse_rst.crypted_text, parse_rst.crypted_text_len,
                sym_key, decrypt_text
            );
            break;
        case SYM_ZUC:
            zuc_decrypt(parse_rst.crypted_text, parse_rst.crypted_text_len, sym_key, parse_rst.sym_info, decrypt_text);
        }

        /* write the decrypt result to output file */

        /* init the default outfile_path */

        if (outfile_path == NULL) {
            outfile_path = str_rep_ext(infile_path, ext_name);
        }

        FILE *outfile = fopen(outfile_path, "wb");
        fwrite(decrypt_text, sizeof(uint8_t), decrypt_text_len, outfile);
        fclose(outfile);

        free(decrypt_text); /* malloced in main after parse_fde_file() */
        free(sym_key); /* malloced in after parse_fde_file() */
        free(pub_key); /* malloced in read_bin_file() */
        free(parse_rst.crypted_key); /* malloced and inited in parse_fde_file() */
        free(parse_rst.crypted_text); /* malloced and inited in parse_fde_file() */
        free(parse_rst.asy_info); /* malloced and inited in parse_fde_file() */
        free(parse_rst.sym_info); /* malloced and inited in parse_fde_file() */
    } /* else if (mode == DEC_MODE) */


exit:
    // free(infile_path);
    free(outfile_path); /* may be malloced in str_rep_ext */
    return ret;
}
