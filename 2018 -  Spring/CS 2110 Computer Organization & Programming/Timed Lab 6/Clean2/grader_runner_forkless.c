#include "grader.h"

int main() {

    printf("running tests\n");
    int total = 0;
    for(int i = 0; i < NUM_TESTS; i++) {
        setup();
        int ret = tests[i].tester(&t);
        if(!ret) {
            tests[i].passed = true;
            total += tests[i].value;
            printf("[%sPASS%s] %s\n", TERM_GREEN, TERM_RESET, tests[i].name);
        } else {
            printf("[%sFAIL%s] %s\n", TERM_RED, TERM_RESET, tests[i].name);
            tests[i].passed = false;
        }
        teardown();
    }
    printf("total: %d/100\n", total);
    printf("done with tests\n");

}