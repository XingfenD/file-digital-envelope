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
#include <rsa.h>
#include <utils.h>
#include <defs.h>
#include <malloc.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int opt;
    int mode = UN_ASSIGNED;
    char *infile_path = NULL, *outfile_path = NULL;

    while ((opt = getopt(argc, argv, "f:o:k:m:h")) != -1) {
        switch (opt) {
            case 'f':
                printf("Input file: %s\n", optarg);
                infile_path = str_malloc_cpy(optarg);
                break;
            case 'o':
                printf("Output file: %s\n", optarg);
                outfile_path = str_malloc_cpy(optarg);
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
                        printf("Conflict options: -D -E\n");
                        return ERR_CONFLICT_OPT;
                    }
                } else if (strcmp(optarg, "dec") == 0) {
                    if (mode == UN_ASSIGNED) {
                        mode = DEC_MODE;
                        printf("Decrypt mode\n");
                    } else {
                        printf("Conflict options: -D -E\n");
                        return ERR_CONFLICT_OPT;
                    }
                }
                break;
            case '?':
                printf("Invalid option: %c\n", optopt);
                break;
                printf("Unknown option: %c\n", opt);
                break;
        }
    }

    if (optind < argc) {
        printf("Remaining arguments: ");
        for (; optind < argc; optind++) {
            printf("%s ", argv[optind]);
        }
        printf("\n");
    }
    free(infile_path);
    free(outfile_path);

    return 0;
}
