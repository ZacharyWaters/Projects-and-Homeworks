//
// Created by pjztam on 4/17/18.
//

#ifndef TL06_SPRING18_GRADER_H
#define TL06_SPRING18_GRADER_H

#include "wrap.h"
#include "list_array.h"

// typedefs
typedef struct _student_test {
    int (*tester)(alloc_tester* t1);
    char* name;
    int value;
    bool passed;
} student_test;

typedef struct _fake_free_func_t {
    int size;
    void* arr[30];
} fake_free_func_t;

typedef struct _fake_copy_func_t {
    int size;
    void* arr[30];
} fake_copy_func_t;

// defines
#define NUM_TESTS 15

#define TERM_RED     "\x1b[31m"
#define TERM_GREEN   "\x1b[32m"
#define TERM_RESET   "\x1b[0m"

// externs
extern alloc_tester t;
extern student_test tests[];

// function prototypes
int setup();
int teardown();
void fake_free_func(void* data);
int fake_copy_func(const void* data, void** data_out);
int fake_eq_func(const void* a, const void* b);


// create node
int test_create_node_second_alloc_null(alloc_tester* t1);
int test_create_node(alloc_tester* t1);

// create list
int test_create_list_third_alloc_null(alloc_tester* t1);
int test_create_list(alloc_tester* t1);

// set
int test_set_null_params(alloc_tester* t1);
int test_set_easy_1(alloc_tester* t1);
int test_set_easy_2(alloc_tester* t1);
int test_set_easy_3(alloc_tester* t1);
int test_set_easy_4(alloc_tester* t1);
int test_set_hard_1(alloc_tester* t1);
int test_set_hard_2(alloc_tester* t1);
int test_set_hard_3(alloc_tester* t1);
int test_set_hard_4(alloc_tester* t1);
int test_set_hard_4_fail(alloc_tester* t1);

// get
int test_get_1(alloc_tester* t1);
int test_get_2(alloc_tester* t1);
int test_get_3(alloc_tester* t1);

// destroy
int test_destroy(alloc_tester* t1);

//
//// ta functions
list_array* ta_create_list_array_empty();
list_array* ta_create_list_array_nodes_1();
list_array* ta_create_list_array_nodes_3();


#endif //TL06_SPRING18_GRADER_H
