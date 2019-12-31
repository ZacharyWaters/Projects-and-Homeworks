#include "grader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

int timeout_time = 1;

int main() {
    printf("starting tests\n");

    for(int i = 0; i < NUM_TESTS; i++) {
        setup();

        pid_t intermediate_pid = fork();
        if (intermediate_pid == 0) {
            pid_t worker_pid = fork();
            if (worker_pid == 0) {
                int status = tests[i].tester(&t);
                if(status) {
                    _exit(EXIT_FAILURE);
                }
                _exit(EXIT_SUCCESS);
            }

            pid_t timeout_pid = fork();
            if (timeout_pid == 0) {
                sleep(timeout_time);
                _exit(EXIT_SUCCESS);
            }

            int workerOrTimerStatus;
            bool timedOut = false;
            pid_t exited_pid = wait(&workerOrTimerStatus);
            if (exited_pid == worker_pid) {
                kill(timeout_pid, SIGKILL);
            } else {
                kill(worker_pid, SIGKILL);
                timedOut = true;
            }

            wait(NULL); // Collect the other process

            if(timedOut) {
                printf("test timed out\n");
                _exit(EXIT_FAILURE);
            } else {
                _exit(workerOrTimerStatus);
            }
        }

        int status;
        waitpid(intermediate_pid, &status, 0);

        if(status == EXIT_SUCCESS) {
            printf("[%sPASS%s] %s\n", TERM_GREEN, TERM_RESET, tests[i].name);
            tests[i].passed = true;
        } else {
            printf("[%sFAIL%s] %s\n", TERM_RED, TERM_RESET, tests[i].name);
        }
    }

    printf("done with tests\n");
    FILE* f = fopen("gradelog.json", "w");
    fprintf(f, "[");
    for(int i = 0; i < NUM_TESTS; i++) {
        fprintf(f,"{\"test_name\":\"%s\", \"test_passed\":%s, \"test_total\":%d, \"test_score\":%d}%s\n",
                tests[i].name,
                tests[i].passed ? "true" : "false",
                tests[i].value,
                tests[i].passed ? tests[i].value : 0,
                i + 1 == NUM_TESTS ? "" : ",");
    }
    fprintf(f,"]");
    fclose(f);

}