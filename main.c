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

int main(int argc, char *argv[]) {
    int opt;
    int mode = UN_ASSIGNED;
    int ret = 0;
    char *infile_path = NULL, *outfile_path = NULL, *keyfile_path = NULL;

    // parse the command line input
    while ((opt = getopt(argc, argv, "f:o:k:m:h")) != -1) {
        switch (opt) {
            case 'f':
                if (infile_path == NULL) {
                    printf("Input file: %s\n", optarg);
                    infile_path = optarg;
                    // infile_path = str_malloc_cpy(optarg);
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
                    // outfile_path = str_malloc_cpy(optarg);
                } else {
                    printf("Multiple options: -%c\n", opt);
                    EXIT_MAIN(ERR_MULTPLE_OPT);
                }
                break;
            case 'k':
                if (keyfile_path == NULL) {
                    printf("Key file: %s\n", optarg);
                    keyfile_path = optarg;
                    // outfile_path = str_malloc_cpy(optarg);
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

                break;
            case '?':
                printf("Invalid option: %c\n", optopt);
                printf("Unknown option: %c\n", opt);
                EXIT_MAIN(ERR_UNKNOWN_OPT)
        }
    }

    // missing keyfile handling
    if (keyfile_path == NULL) {
        printf("Required parameters are not specified: -k");
        EXIT_MAIN(ERR_MISS_PARAM)
    }

    // extra arguments handling
    if (optind < argc) {
        printf("Remaining arguments: ");
        for (; optind < argc; optind++) {
            printf("%s ", argv[optind]);
        }
        ASK_EXIT_MAIN
    }

    /* execute main function according to the args input */
    if (mode == ENC_MODE) {

        /* init default arg */
        if (outfile_path == NULL) {
            /* set the output file name if the path are not specified */
            outfile_path = str_rep_ext(infile_path, "fde");
        }


    } else if (mode == DEC_MODE){
        char ext_name[17];
        uint8_t *ciper_key = NULL, *ciper_text = NULL, *pub_key = NULL, *sym_key = NULL, *decrypt_text = NULL;
        size_t ciper_key_len, ciper_text_len, pub_key_len, sym_key_len, decrypt_text_len;
        uint8_t crypt_alg;

        /* parse the fde head and read body */
        parse_fde_file(
            infile_path, ext_name,
            &crypt_alg,
            &ciper_key, &ciper_text,
            &ciper_key_len, &ciper_text_len
        );

        /* malloc the memory to decrypt_text and sym_key */
        decrypt_text = malloc(sizeof(uint8_t) * ciper_text_len);
        sym_key = malloc(sizeof(uint8_t) * ciper_key_len);

        /* decrypt the key using asymmetric encrypt algorithm */
        switch (crypt_alg & 0xF0)
        {
        case ASY_RSA:
            uint8_t vector[16];

            /* TODO: check the return code of the functions below */
            /* decrypt the sym-key */
            read_key_file(keyfile_path, &pub_key, &pub_key_len);
            rsa_padding_decrypt(ciper_key, sym_key, ciper_key_len, pub_key, pub_key_len);
            sym_key_len = ciper_key_len;
            /* decrypt the text */

            sm4_padding_decrypt(ciper_text, decrypt_text, ciper_text_len, &decrypt_text_len, vector, sym_key);

            break;
        default:
            break;
        }

        free(decrypt_text); /* malloced in main after parse_fde_file() */
        free(sym_key); /* malloced in after parse_fde_file() */
        free(pub_key); /* malloced in read_key_file() */
        free(ciper_key); /* malloced and inited in parse_fde_file() */
        free(ciper_text); /* malloced and inited in parse_fde_file() */
    }


exit:
    // free(infile_path);
    free(outfile_path); /* malloced and inited in parsing cmd line args */
    return ret;
}
