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
            printf("[PASS] %s\n", tests[i].name);
        } else {
            tests[i].passed = false;
        }
        teardown();
    }
    printf("total: %d/100\n", total);
    printf("done with tests\n");

}
