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

/* TODO: reconstruct the main function */
int main(int argc, char *argv[]) {
    int opt;
    int mode = UN_ASSIGNED;
    int ret = NORMAL_EXIT;
    uint8_t cipher_algo = 0x00;
    char *infile_path = NULL, *outfile_path = NULL, *keyfile_path = NULL;

    /* TODO: realize an advanced args parsing with dependency and exclusion */
    /* parse the command line input */
    while ((opt = getopt(argc, argv, "f:o:k:m:h:a:s")) != -1) {
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
                    if (strcmp(optarg, G_ASY_NAMES[i]) == 0) {
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
        size_t plain_text_len, cipher_text_len, pub_key_len;
        FILE *outfile = NULL;
        FDE_HEAD fde_head = {};

        /**
         * @brief assign basic value to fde_head
         * [*] file_type
         * [*] origin_ext
         * [*] cipher_algo
         * [ ] asy_info_len
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
            fde_head.sym_key_len = 16;
            fde_head.sym_info_len = 16;
            sym_info = malloc(fde_head.sym_info_len * sizeof(uint8_t));
            sym_key = malloc(fde_head.sym_key_len * sizeof(uint8_t));
            random_bytes(sym_info, fde_head.sym_info_len);
            random_bytes(sym_key, fde_head.sym_key_len);
            sm4_padding_encrypt(plain_text, &cipher_text, plain_text_len, &cipher_text_len, sym_info, sym_key);
            break;
        default:
            break;
        }

        switch (GET_ASY_BITS(cipher_algo))
        {
        case ASY_RSA:
            break;
        default:
            break;
        }

        free(sym_key); /* malloced in symmetric encryption */
        free(sym_info); /* malloced in symmetric encryption */
        free(asy_info); /* malloced in asymmetric encryption */
        free(plain_text); /* malloced in read_bin_file */
        free(pub_key);  /* malloced in read_bin_file */
    } else if (mode == DEC_MODE){ /* if (mode == ENC_MODE) */
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
            rsa_padding_decrypt(
                parse_rst.crypted_key, sym_key,
                parse_rst.crypted_key_len,
                pub_key, pub_key_len
            );
            sym_key_len = parse_rst.crypted_key_len;
            break;
        default:
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

        default:
            break;
        }

        /* write the decrypt result to output file */

        /* init the default outfile_path */

        if (outfile_path == NULL) {
            outfile_path = str_rep_ext(infile_path, ext_name);
        }

        FILE *outfile = fopen(outfile_path, "wb");
        fwrite(decrypt_text, sizeof(uint8_t), decrypt_text_len, outfile);
        free(outfile);

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
