#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grader.h"

int main(int argc, char* argv[]) {

    if(argc != 2) {
        printf("proper usage\n./tl06tests_single <test_name>\n");
        exit(1);
    }

    char* test_name = argv[1];

    printf("running test \"%s\"\n", test_name);

    // find specified test
    int test_idx = -1;
    for(int i = 0; i < NUM_TESTS; i++) {
        if(!strcmp(test_name, tests[i].name)) {
            test_idx = i;
            break;
        }
    }
    if(test_idx < 0) {
        fprintf(stderr, "invalid test name\n");
        exit(1);
    }

    // run the actual test
    setup();
    int ret = tests[test_idx].tester(&t);
    teardown();

    if(ret) {
        fprintf(stderr, "test returned non-zero status code\n");
        exit(5);
    }
    printf("test completed successfully\n");
    exit(0);
}