/**
 * @file ./test/test_demo.c
 * @brief the demo test file
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */

/* you can include the module in directory:./utils and cryptographic algorithm */
#include <utils.h>
#include <sm4.h>

int main() {
    utils_print();
    sm4_print();

    return 0;
}