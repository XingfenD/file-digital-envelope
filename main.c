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
#include <malloc.h>
#include <string.h>
#include <main_callee.h>

int main(int argc, char *argv[]) {
    int opt;
    int mode = UN_ASSIGNED;
    int ret = 0;
    char *infile_path = NULL, *outfile_path = NULL, *keyfile_path;

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
            outfile_path = str_rep_ext(infile_path, "fde");
        }


    } else if (mode == DEC_MODE){
        char ext_name[17];
        uint8_t *ciper_key = NULL, *ciper_text = NULL;
        uint8_t crypt_alg;
        size_t ciper_text_len, ciper_key_len;

        /* init default arg */
        parse_fde_file(infile_path, ext_name, &crypt_alg, ciper_key, ciper_text, &ciper_key_len, &ciper_text_len);

        free(ciper_key);
        free(ciper_text);
    }


exit:
    // free(infile_path);
    free(outfile_path);
    return ret;
}
