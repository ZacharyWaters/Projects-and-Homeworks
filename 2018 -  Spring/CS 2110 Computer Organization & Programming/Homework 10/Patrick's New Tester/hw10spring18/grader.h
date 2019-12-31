//
// Created by pjztam on 4/4/18.
//

#ifndef HW10_GRADER_H
#define HW10_GRADER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "wrap.h"
#include "array_list.h"

extern alloc_tester t;

typedef struct _student_test {
    int (*tester)(alloc_tester* t1);
    char* name;
    int value;
    bool passed;
} student_test;


typedef struct _fake_free_func_t {
    int size;
    void* arr[10];
} fake_free_func_t;

fake_free_func_t fake_free;



// function prototypes
int setup();
int teardown();
void fake_free_func(void* data);

#define NUM_TESTS 32

// create
int test_create_first_malloc_fails(alloc_tester* t1);
int test_create_second_malloc_fails(alloc_tester* t1);
int test_create_success(alloc_tester* t1);

// shallow copy
int test_shallow_copy_first_malloc_fails(alloc_tester* t1);
int test_shallow_copy_second_malloc_fails(alloc_tester* t1);
int test_shallow_copy_fail_params(alloc_tester* t1);
int test_shallow_copy_success(alloc_tester* t1);

// add tests
int test_add_neg_empty_1(alloc_tester* t1);
int test_add_neg_empty_2(alloc_tester* t1);
int test_add_neg_easy_1(alloc_tester* t1);
int test_add_neg_easy_2(alloc_tester* t1);
int test_add_neg_easy_3(alloc_tester* t1);
int test_add_neg_hard_1(alloc_tester* t1);
int test_add_neg_hard_2(alloc_tester* t1);
int test_add_neg_hard_3(alloc_tester* t1);
int test_add_pos_empty_1(alloc_tester* t1);
int test_add_pos_empty_2(alloc_tester* t1);
int test_add_pos_easy_1(alloc_tester* t1);
int test_add_pos_easy_2(alloc_tester* t1);
int test_add_pos_easy_3(alloc_tester* t1);
int test_add_pos_easy_4(alloc_tester* t1);
int test_add_pos_hard_1(alloc_tester* t1);
int test_add_pos_hard_2(alloc_tester* t1);
int test_add_pos_hard_3(alloc_tester* t1);
int test_add_pos_hard_4(alloc_tester* t1);
int test_add_pos_hard_5(alloc_tester* t1);
int test_add_null_params(alloc_tester* t1);
int test_add_resize_fail(alloc_tester* t1);

// remove tests
int test_rem_pos_empty_1(alloc_tester* t1);
int test_rem_pos_easy_1(alloc_tester* t1);
int test_rem_pos_easy_2(alloc_tester* t1);
int test_rem_pos_easy_3(alloc_tester* t1);
int test_rem_pos_easy_4(alloc_tester* t1);
int test_rem_pos_hard_1(alloc_tester* t1);
int test_rem_pos_hard_2(alloc_tester* t1);
int test_rem_pos_hard_3(alloc_tester* t1);

// destroy
int test_destroy_null(alloc_tester* t1);
int test_destroy(alloc_tester* t1);

// trim to size
int test_trim_null(alloc_tester* t1);
int test_trim(alloc_tester* t1);

// ta functions
array_list_t* ta_create_array_list();
array_list_t* ta_create_array_list_2();

#endif //HW10_GRADER_H


