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
    char *infile_path = NULL, *outfile_path = NULL;

    // parse the command line input
    while ((opt = getopt(argc, argv, "f:o:k:m:h")) != -1) {
        switch (opt) {
            case 'f':
                if (infile_path == NULL) {
                    printf("Input file: %s\n", optarg);
                    infile_path = str_malloc_cpy(optarg);
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
                    outfile_path = str_malloc_cpy(optarg);
                } else {
                    printf("Multiple options: -%c\n", opt);
                    EXIT_MAIN(ERR_MULTPLE_OPT);
                }

                break;
            case 'k':
                printf("Key file: %s\n", optarg);
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

    if (optind < argc) {
        printf("Remaining arguments: ");
        for (; optind < argc; optind++) {
            printf("%s ", argv[optind]);
        }
        ASK_EXIT_MAIN
    }

exit:
    free(infile_path);
    free(outfile_path);
    return ret;
}
