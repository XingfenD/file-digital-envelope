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

int main(int argc, char *argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                printf("Input file: %s\n", optarg);
                break;
            case 'o':
                printf("Output file: %s\n", optarg);
                break;
            case '?':
                printf("Invalid option: %c\n", optopt);
                break;
            default:
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

    return 0;
}
