//
// Created by pjztam on 4/15/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "grader.h"

int main(int argc, char* argv[]) {
  printf("writing test list to file\n");
  FILE* f = fopen("tests.json", "w");
  fprintf(f, "[");
  for(int i = 0; i < NUM_TESTS; i++) {
    fprintf(f,"{\"test_name\":\"%s\", \"test_score\":%d}%s\n",
            tests[i].name,
            tests[i].value,
            i + 1 == NUM_TESTS ? "" : ",");
  }
  fprintf(f,"]");
  fclose(f);
}

