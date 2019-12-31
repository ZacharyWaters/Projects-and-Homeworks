#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "grader.h"
#include "array_list.h"
#include "circ_list.h"

// globals
fake_free_func_t fake_free;
fake_copy_func_t fake_copy;

// tests
student_test tests[NUM_TESTS] = {

        // create - 7 points
        {test_create_first_malloc_fails,        "test_create_first_malloc_fails",           2, false},
        {test_create_second_malloc_fails,       "test_create_second_malloc_fails",          2, false},
        {test_create_success,                   "test_create_success",                      3, false},


        // shallow - 8 points
        {test_shallow_copy_first_malloc_fails,  "test_shallow_copy_first_malloc_fails",     2, false},
        {test_shallow_copy_second_malloc_fails, "test_shallow_copy_second_malloc_fails",    2, false},
        {test_shallow_copy_fail_params,         "test_shallow_copy_fail_params",            2, false},
        {test_shallow_copy_success,             "test_shallow_copy_success",                2, false},

        // add - 42 points
        {test_add_pos_empty_1,                  "test_add_pos_empty_1",                     2, false},
        {test_add_pos_empty_2,                  "test_add_pos_empty_2",                     2, false},
        {test_add_pos_easy_1,                   "test_add_pos_easy_1",                      3, false},
        {test_add_pos_easy_2,                   "test_add_pos_easy_2",                      3, false},
        {test_add_pos_easy_3,                   "test_add_pos_easy_3",                      3, false},
        {test_add_pos_easy_4,                   "test_add_pos_easy_4",                      3, false},
        {test_add_pos_hard_1,                   "test_add_pos_hard_1",                      4, false},
        {test_add_pos_hard_2,                   "test_add_pos_hard_2",                      4, false},
        {test_add_pos_hard_3,                   "test_add_pos_hard_3",                      4, false},
        {test_add_pos_hard_4,                   "test_add_pos_hard_4",                      4, false},
        {test_add_pos_hard_5,                   "test_add_pos_hard_5",                      4, false},
        {test_add_null_params,                  "test_add_null_params",                     1, false},
        {test_add_resize_fail,                  "test_add_resize_fail",                     5, false},

        // rem - 35 points
        {test_rem_pos_empty_1,                  "test_rem_pos_empty_1",                     3, false},
        {test_rem_pos_easy_1,                   "test_rem_pos_easy_1",                      3, false},
        {test_rem_pos_easy_2,                   "test_rem_pos_easy_2",                      3, false},
        {test_rem_pos_easy_3,                   "test_rem_pos_easy_3",                      4, false},
        {test_rem_pos_easy_4,                   "test_rem_pos_easy_4",                      4, false},
        {test_rem_pos_hard_1,                   "test_rem_pos_hard_1",                      6, false},
        {test_rem_pos_hard_2,                   "test_rem_pos_hard_2",                      6, false},
        {test_rem_pos_hard_3,                   "test_rem_pos_hard_3",                      6, false},

        // destroy - 5 points
        {test_destroy_null,                     "test_destroy_null",                        1, false},
        {test_destroy,                          "test_destroy",                             4, false},

        // trim - 3 points
        {test_trim_null,                        "test_trim_null",                           1, false},
        {test_trim,                             "test_trim",                                2, false},

        // linked list ec - 25 points
        {test_ec_create,                        "test_ec_create",                           1, false},
        {test_ec_destroy,                       "test_ec_destroy",                          1, false},
        {test_ec_shallow,                       "test_ec_shallow",                          2, false},
        {test_ec_shallow_fail,                  "test_ec_shallow_fail",                     4, false},
        {test_ec_deep,                          "test_ec_deep",                             2, false},
        {test_ec_deep_fail,                     "test_ec_deep_fail",                        4, false},
        {test_ec_add,                           "test_ec_add",                              2, false},
        {test_ec_add_fail,                      "test_ec_add_fail",                         2, false},
        {test_ec_get,                           "test_ec_get",                              1, false},
        {test_ec_rem,                           "test_ec_rem",                              3, false},
        {test_ec_contains,                      "test_ec_contains",                         1, false},
        {test_ec_zip,                           "test_ec_zip",                              2, false},

};

int setup() {
    memset(&t, 0, sizeof(alloc_tester));
    memset(&fake_free, 0, sizeof(fake_free_func_t));
    return 0;
}

int teardown() {
    return 0;
}

array_list_t* ta_create_array_list() {
    array_list_t* arr = malloc(sizeof(array_list_t));
    arr->entries = calloc(10, sizeof(void*));
    arr->size = 0;
    arr->capacity = 10;
    return arr;
}

array_list_t* ta_create_array_list_2() {
    array_list_t* arr = malloc(sizeof(array_list_t));
    arr->entries = calloc(20, sizeof(void*));
    arr->size = 0;
    arr->capacity = 20;
    return arr;
}

// 4 calls
circ_list* ta_create_ll_2() {
    circ_list* ll = calloc(1, sizeof(circ_list));
    l_node* node0 = calloc(1, sizeof(l_node));
    l_node* node1 = calloc(1, sizeof(l_node));
    l_node* node2 = calloc(1, sizeof(l_node));

    ll->size = 3;
    ll->head = node0;
    node0->next = node1;
    node0->prev = node2;
    node0->data = (void*)1;

    node1->next = node2;
    node1->prev = node0;
    node1->data = (void*)2;

    node2->next = node0;
    node2->prev = node1;
    node2->data = (void*)3;

    return ll;
}

void fake_free_func(void* data) {
    fake_free.arr[fake_free.size++] = data;
}

int fake_copy_func(const void* data, void** data_out) {
    fake_copy.arr[fake_copy.size++] = (void*)data;
    *data_out = (void*)data;
    return 0;
}

int fake_eq_func(const void* a, const void* b) {
    char str[32];
    char* endstr;
    memset(str, 0, sizeof(str));
    sprintf(str, "%p", a);
    long a_dec = strtol(str, &endstr, 16);
    memset(str, 0, sizeof(str));
    sprintf(str, "%p", b);
    long b_dec = strtol(str, &endstr, 16);
    return a_dec + 1 == b_dec || a_dec == b_dec + 1;
}

int test_create_first_malloc_fails(alloc_tester* t1) {

    // 0 malloc calls succeed
    t1->malloc_calloc_budget = 0;


    array_list_t* arr = create_array_list();

    // should've return NULL because it failed
    assert(arr == NULL);

    // malloc/calloc called 1 time (because it failed on first call)
    assert(t1->malloc_calls + t1->calloc_calls == 1);

    // free has not been called
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // all memory should be freed
    assert(all_freed(&t));

    // right malloc call made
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 1, 0);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 1, 0);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 1, 0);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 1, 0);
    assert(malloc_array || calloc_array || malloc_back || calloc_back);

    // free everything
    cleanup(&t);

    return 0;
}

int test_create_second_malloc_fails(alloc_tester* t1) {

    // 1 malloc calls succeed
    t1->malloc_calloc_budget = 1;


    array_list_t* arr = create_array_list();

    // should've return NULL because it failed
    assert(arr == NULL);

    // malloc/calloc called 2 times (because it failed on second call)
    assert(t1->malloc_calls + t1->calloc_calls == 2);

    // free has been called once
    assert(t1->free_calls == 1);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // all memory should be freed
    assert(all_freed(&t));

    // malloc was called for both backing array and the struct
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 3, 0);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 3, 0);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 3, 0);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 3, 0);

    assert(malloc_array || calloc_array);
    assert(malloc_back || calloc_back);

    cleanup(&t);

    return 0;
}

int test_create_success(alloc_tester* t1) {

    // 2 malloc calls succeed
    t1->malloc_calloc_budget = 2;


    array_list_t* arr = create_array_list();

    // should be a good array list with no entries
    assert(arr);
    assert(arr->size == 0);
    assert(arr->entries);
    assert(arr->capacity == INITIAL_CAPACITY);

    // every entry should be NULL
    // assume that INITIAL_CAPACITY is 10
    void* expected[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    assert(memcmp(arr->entries, expected, sizeof(void*)*10) == 0);

    // malloc/calloc called 2 times (for backing array and struct)
    assert(t1->malloc_calls + t1->calloc_calls == 2);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for both backing array and the struct
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 2, 0);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 2, 0);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 2, 0);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 2, 0);

    assert(malloc_array || calloc_array);
    assert(malloc_back || calloc_back);

    cleanup(&t);

    return 0;
}

int test_shallow_copy_first_malloc_fails(alloc_tester* t1) {

    // 2 malloc calls succeed
    t1->malloc_calloc_budget = 2;

    // original array list
    array_list_t* arr = ta_create_array_list();
    void** entries_old = arr->entries;

    // put some test data in it
    void* expected[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                           (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    arr->size = 10;
    memcpy(arr->entries, expected, sizeof(void*)*10);

    // should be a bad array list

    array_list_t* arr2 = shallow_copy_array_list(arr);

    // original array list unchanged
    assert(arr->size == 10);
    assert(arr->capacity == INITIAL_CAPACITY);
    assert(arr->entries == entries_old);
    assert(memcmp(arr->entries, expected, sizeof(void*)*10) == 0);;

    // new array list is dead
    assert(arr2 == NULL);

    // malloc/calloc called 3 times (for original's backing array and struct) + 1
    assert(t1->malloc_calls + t1->calloc_calls == 3);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 1, 0);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 1, 0);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 1, 0);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 1, 0);

    assert(malloc_array || calloc_array || malloc_back || calloc_back);

    cleanup(&t);

    return 0;
}

int test_shallow_copy_second_malloc_fails(alloc_tester* t1) {

    // 3 malloc calls succeed (old array + half of new array)
    t1->malloc_calloc_budget = 3;

    // original array list
    array_list_t* arr = ta_create_array_list();
    void** entries_old = arr->entries;

    // put some test data in it
    void* expected[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                           (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    arr->size = 10;
    memcpy(arr->entries, expected, sizeof(void*)*10);

    // should be a bad array list
    array_list_t* arr2 = shallow_copy_array_list(arr);

    // original array list unchanged
    assert(arr->size == 10);
    assert(arr->capacity == INITIAL_CAPACITY);
    assert(arr->entries == entries_old);
    assert(memcmp(arr->entries, expected, sizeof(void*)*10) == 0);

    // new array list is dead
    assert(arr2 == NULL);

    // malloc/calloc called 4 times (for original's backing array and struct) + 2
    assert(t1->malloc_calls + t1->calloc_calls == 4);

    // free have been called once
    assert(t1->free_calls == 1);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 3, 0);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 3, 0);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 3, 0);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 3, 0);

    assert(malloc_array || calloc_array);
    assert(malloc_back || calloc_back);

    cleanup(&t);

    return 0;
}

int test_shallow_copy_fail_params(alloc_tester* t1) {

    // should fail immediately
    array_list_t* arr = shallow_copy_array_list(NULL);
    assert(arr == NULL);

    // no calls to alloc or free
    assert(t1->malloc_calls + t1->calloc_calls == 0);
    assert(t1->free_calls == 0);

    cleanup(&t);

    return 0;
}

int test_shallow_copy_success(alloc_tester* t1) {

    // 4 malloc calls succeed effectively creating 2 array lists
    t1->malloc_calloc_budget = 4;

    // original array list
    array_list_t* arr = ta_create_array_list();
    void** entries_old = arr->entries;

    // put some test data in it
    void* expected[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                           (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    arr->size = 10;
    memcpy(arr->entries, expected, sizeof(void*)*10);

    // should be a good array list
    array_list_t* arr2 = shallow_copy_array_list(arr);

    // original array list unchanged
    assert(arr->size == 10);
    assert(arr->capacity == INITIAL_CAPACITY);
    assert(arr->entries == entries_old);
    assert(memcmp(arr->entries, expected, sizeof(void*)*10) == 0);

    // new array list identical to old array list
    assert(arr2->size == 10);
    assert(arr2->capacity == INITIAL_CAPACITY);
    assert(arr2->entries != arr->entries);
    assert(memcmp(arr2->entries, expected, sizeof(void*)*10) == 0);

    // malloc/calloc called 4 times (for original's backing array and struct) + 2
    assert(t1->malloc_calls + t1->calloc_calls == 4);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 2, 0);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 2, 0);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 2, 0);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 2, 0);

    assert(malloc_array || calloc_array);
    assert(malloc_back || calloc_back);

    cleanup(&t);

    return 0;
}

/**
 * negative index on empty list
 * [ ] size:0, cap:10, idx:-1, dat:7
 * [7] size:1, cap:10
 */
int test_add_neg_empty_1(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)7,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 0;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, -1, (void*)7));

    // test the array list
    assert(arr->size == 1);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * super negative index on empty list
 * [ ] size:0, cap:10, idx:-72, dat:7
 * [7] size:1, cap:10
 */
int test_add_neg_empty_2(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)7,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 0;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, -72, (void*)7));

    // test the array list
    assert(arr->size == 1);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * negative index beginning of array no resize
 * [3,1,4,1,5] size:5, cap:10, idx:-6, dat:7
 * [7,3,1,4,1,5] size:6, cap:10
 */
int test_add_neg_easy_1(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)7, (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, -6, (void*)7));

    // test the array list
    assert(arr->size == 6);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * negative index middle of array no resize
 * [3,1,4,1,5] size:5, cap:10, idx:-5, dat:7
 * [3,7,1,4,1,5] size:6, cap:10
 */
int test_add_neg_easy_2(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)3, (void*)7, (void*)1, (void*)4, (void*)1, (void*)5,
                       NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, -5, (void*)7));

    // test the array list
    assert(arr->size == 6);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * negative index end of array no resize
 * [3,1,4,1,5] size:5, cap:10, idx:-1, dat:7
 * [3,1,4,1,5,7] size:6, cap:10
 */
int test_add_neg_easy_3(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, (void*)7,
                       NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, -1, (void*)7));

    // test the array list
    assert(arr->size == 6);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * super negative index beginning of array resize
 * [3,1,4,1,5,9,2,6,5,3] size:10, cap:10, idx:-44, dat:7
 * [7,3,1,4,1,5,9,2,6,5,3] size:11, cap:20
 */
int test_add_neg_hard_1(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    void* after[20] = {(void*)7,
                       (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                       (void*)9, (void*)2, (void*)6, (void*)5, (void*)3,
                       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, -44, (void*)7));

    // test the array list
    assert(arr->size == 11);
    assert(arr->capacity == 20);
    assert(memcmp(arr->entries, after, 20*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}

/**
 * super negative index middle of array resize
 * [3,1,4,1,5,9,2,6,5,3] size:10, cap:10, idx:-43, dat:7
 * [3,7,1,4,1,5,9,2,6,5,3] size:11, cap:20
 */
int test_add_neg_hard_2(alloc_tester* t1) {
    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    void* after[20] = {(void*)3, (void*)7, (void*)1, (void*)4, (void*)1, (void*)5,
                       (void*)9, (void*)2, (void*)6, (void*)5, (void*)3,
                       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    add_to_array_list(arr, -43, (void*)7);

    // test the array list
    assert(arr->size == 11);
    assert(arr->capacity == 20);
    assert(memcmp(arr->entries, after, 20*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}

/**
 * super negative index end of array resize
 * [3,1,4,1,5,9,2,6,5,3] size:10, cap:10, idx:-34, dat:7
 * [3,1,4,1,5,9,2,6,5,3,7] size:11, cap:20
 */
int test_add_neg_hard_3(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    void* after[20] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                       (void*)9, (void*)2, (void*)6, (void*)5, (void*)3, (void*)7,
                       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, -34, (void*)7));

    // test the array list
    assert(arr->size == 11);
    assert(arr->capacity == 20);
    assert(memcmp(arr->entries, after, 20*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}

/**
 * positive index beginning of empty array
 * [] size:0, cap:10, idx:0, dat:7
 * [7] size:1, cap:10
 */
int test_add_pos_empty_1(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)7,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 0;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 0, (void*)7));

    // test the array list
    assert(arr->size == 1);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * positive index middle of empty array
 * [] size:0, cap:10, idx:3, dat:7
 * [NULL, NULL, NULL, 7] size:4, cap:10
 */
int test_add_pos_empty_2(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    void* after[10] = {NULL,NULL,NULL,(void*)7,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 0;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 3, (void*)7));

    // test the array list
    assert(arr->size == 4);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}


/**
 * positive index beginning of array
 * [3,1,4,1,5] size:5, cap:10, idx:0, dat:7
 * [7,3,1,4,1,5] size:6, cap:10
 */
int test_add_pos_easy_1(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)7, (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                       NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 0, (void*)7));

    // test the array list
    assert(arr->size == 6);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}


/**
 * positive index middle of array
 * [3,1,4,1,5] size:5, cap:10, idx:1, dat:7
 * [3,7,1,4,1,5] size:6, cap:10
 */
int test_add_pos_easy_2(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)3, (void*)7, (void*)1, (void*)4, (void*)1, (void*)5,
                       NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 1, (void*)7));

    // test the array list
    assert(arr->size == 6);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}


/**
 * positive index end of array
 * [3,1,4,1,5] size:5, cap:10, idx:5, dat:7
 * [3,1,4,1,5,7] size:6, cap:10
 */
int test_add_pos_easy_3(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, (void*)7,
                       NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 5, (void*)7));

    // test the array list
    assert(arr->size == 6);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}


/**
 * positive index past end of array
 * [3,1,4,1,5] size:5, cap:10, idx:7, dat:7
 * [3,1,4,1,5,NULL,NULL,7] size:8, cap:10
 */
int test_add_pos_easy_4(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                       NULL,(void*)7,NULL,NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 7, (void*)7));

    // test the array list
    assert(arr->size == 8);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * positive index beginning of array resize
 * [3,1,4,1,5,9,2,6,5,3] size:10, cap:10, idx:0, dat:7
 * [7,3,1,4,1,5,9,2,6,5,3] size:11, cap:20
 */
int test_add_pos_hard_1(alloc_tester* t1) {
    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    void* after[20] = {(void*)7,
                       (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                       (void*)9, (void*)2, (void*)6, (void*)5, (void*)3,
                       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 0, (void*)7));

    // test the array list
    assert(arr->size == 11);
    assert(arr->capacity == 20);
    assert(memcmp(arr->entries, after, 20*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}


/**
 * positive index middle of array resize
 * [3,1,4,1,5,9,2,6,5,3] size:10, cap:10, idx:1, dat:7
 * [3,7,1,4,1,5,9,2,6,5,3] size:11, cap:20
 */
int test_add_pos_hard_2(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    void* after[20] = {(void*)3, (void*)7, (void*)1, (void*)4, (void*)1, (void*)5,
                       (void*)9, (void*)2, (void*)6, (void*)5, (void*)3,
                       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 1, (void*)7));

    // test the array list
    assert(arr->size == 11);
    assert(arr->capacity == 20);
    assert(memcmp(arr->entries, after, 20*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}


/**
 * positive index end of array resize
 * [3,1,4,1,5,9,2,6,5,3] size:10, cap:10, idx:10, dat:7
 * [3,1,4,1,5,9,2,6,5,3,7] size:11, cap:20
 */
int test_add_pos_hard_3(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    void* after[20] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                       (void*)9, (void*)2, (void*)6, (void*)5, (void*)3, (void*)7,
                       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 10, (void*)7));

    // test the array list
    assert(arr->size == 11);
    assert(arr->capacity == 20);
    assert(memcmp(arr->entries, after, 20*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}


/**
 * positive index past end of array resize
 * [3,1,4,1,5,9,2,6,5,3] size:10, cap:10, idx:12, dat:7
 * [3,1,4,1,5,9,2,6,5,3,NULL,NULL,7] size:13, cap:20
 */
int test_add_pos_hard_4(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    void* after[20] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                       (void*)9, (void*)2, (void*)6, (void*)5, (void*)3,
                       NULL,NULL,(void*)7,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 12, (void*)7));

    // test the array list
    assert(arr->size == 13);
    assert(arr->capacity == 20);
    assert(memcmp(arr->entries, after, 20*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}


/**
 * positive index past end of array super resize
 * [3,1,4,1,5,9,2,6,5,3] size:10, cap:10, idx:20, dat:7
 * [3,1,4,1,5,9,2,6,5,3
 * ,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,7] size:21, cap:40
 */
int test_add_pos_hard_5(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    void* after[40] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                       (void*)9, (void*)2, (void*)6, (void*)5, (void*)3,
                       NULL,NULL,NULL,NULL,NULL,
                       NULL,NULL,NULL,NULL,NULL,
                       (void*)7, NULL,NULL,NULL,NULL,
                       NULL,NULL,NULL,NULL,NULL,
                       NULL,NULL,NULL,NULL,NULL,
                       NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add
    assert(!add_to_array_list(arr, 20, (void*)7));

    // test the array list
    assert(arr->size == 21);
    assert(arr->capacity == 40);
    assert(memcmp(arr->entries, after, 40*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*40, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}

int test_add_null_params(alloc_tester* t1) {

    // fail on adding to NULL array
    assert(add_to_array_list(NULL, 1, (void*)1));

    return 0;
}

int test_add_resize_fail(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 0;

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the add - should return non-zero
    assert(add_to_array_list(arr, 20, (void*)7));

    // test the array list
    assert(arr->size == 10);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, before, 10*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*40, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}


/**
 * 0 index on empty list
 * [] size:0, cap:10, idx:0
 * [] size:0, cap:10
 * data_out: NULL
 * return fail
 */
int test_rem_pos_empty_1(alloc_tester* t1) {
    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    void* after[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 0;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the rem
    void* data;
    assert(remove_from_array_list(arr, 0, &data));

    // check data
    assert(data == NULL);

    // test the array list
    assert(arr->size == 0);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * positive index beginning of array
 * [3,1,4,1,5] size:5, cap:10, idx:0
 * [1,4,1,5] size:4, cap:10
 * data_out: 3
 * return success
 */
int test_rem_pos_easy_1(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)1, (void*)4, (void*)1, (void*)5,
                       NULL,NULL,NULL,NULL, NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the rem
    void* data;
    assert(!remove_from_array_list(arr, 0, &data));

    // check data
    assert(data == (void*)3);

    // test the array list
    assert(arr->size == 4);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * positive index middle of array
 * [3,1,4,1,5] size:5, cap:10, idx:1
 * [3,4,1,5] size:4, cap:10
 * data_out: 1
 * return success
 */
int test_rem_pos_easy_2(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)3, (void*)4, (void*)1, (void*)5,
                       NULL,NULL,NULL,NULL, NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the rem
    void* data;
    assert(!remove_from_array_list(arr, 1, &data));

    // check data
    assert(data == (void*)1);

    // test the array list
    assert(arr->size == 4);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * positive index end of array
 * [3,1,4,1,5] size:5, cap:10, idx:4
 * [3,1,4,1] size:4, cap:10
 * data_out: 5
 * return success
 */
int test_rem_pos_easy_3(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)3, (void*)1, (void*)4, (void*)1,
                       NULL,NULL,NULL,NULL, NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the rem
    void* data;
    assert(!remove_from_array_list(arr, 4, &data));

    // check data
    assert(data == (void*)5);

    // test the array list
    assert(arr->size == 4);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * positive index past end of array
 * [3,1,4,1,5] size:5, cap:10, idx:5
 * [3,1,4,1,5] size:5, cap:10
 * data_out: NULL
 * return fail
 */
int test_rem_pos_easy_4(alloc_tester* t1) {
    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                        NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5, NULL,
                       NULL,NULL,NULL,NULL};

    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the rem
    void* data;
    assert(remove_from_array_list(arr, 5, &data));

    // check data
    assert(data == NULL);

    // test the array list
    assert(arr->size == 5);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // no new allocs or frees
    assert(t1->alloc_size == 2);

    cleanup(&t);

    return 0;
}

/**
 * positive index beginning of array resize
 * [3,1,4,1,5] size:5, cap:20, idx:0
 * [1,4,1,5] size:4, cap:10
 * data_out: 3
 * return success
 */
int test_rem_pos_hard_1(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list_2();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[20] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        NULL,NULL,NULL,NULL,NULL,
                        NULL,NULL,NULL,NULL,NULL,
                        NULL,NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)1, (void*)4, (void*)1, (void*)5, NULL,
                       NULL,NULL,NULL,NULL,NULL};



    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*20);

    // do the rem
    void* data;
    assert(!remove_from_array_list(arr, 0, &data));

    // test the array list
    assert(arr->size == 4);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*10, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}

/**
 * positive index middle of array resize
 * [3,1,4,1,5] size:5, cap:20, idx:1
 * [3,4,1,5] size:4, cap:10
 * data_out: 1
 * return success
 */
int test_rem_pos_hard_2(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list_2();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[20] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        NULL,NULL,NULL,NULL,NULL,
                        NULL,NULL,NULL,NULL,NULL,
                        NULL,NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)3, (void*)4, (void*)1, (void*)5, NULL,
                       NULL,NULL,NULL,NULL,NULL};



    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*20);

    // do the rem
    void* data;
    assert(!remove_from_array_list(arr, 1, &data));

    // test the array list
    assert(arr->size == 4);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*10, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}

/**
 * positive index end of array resize
 * [3,1,4,1,5] size:5, cap:20, idx:4
 * [3,1,4,1] size:4, cap:10
 * data_out: 5
 * return success
 */
int test_rem_pos_hard_3(alloc_tester* t1) {

    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list_2();
    assert(t1->alloc_size == 2);

    t1->realloc_budget = 1;

    void* before[20] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        NULL,NULL,NULL,NULL,NULL,
                        NULL,NULL,NULL,NULL,NULL,
                        NULL,NULL,NULL,NULL,NULL};

    void* after[10] = {(void*)3, (void*)1, (void*)4, (void*)1, NULL,
                       NULL,NULL,NULL,NULL,NULL};



    // setup before list
    arr->size = 5;
    memcpy(arr->entries, before, sizeof(void*)*20);

    // do the rem
    void* data;
    assert(!remove_from_array_list(arr, 4, &data));

    // test the array list
    assert(arr->size == 4);
    assert(arr->capacity == 10);
    assert(memcmp(arr->entries, after, 10*sizeof(void*)) == 0);


    // 1 realloc was performed
    assert(t1->realloc_calls == 1);

    // free has not been called once
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // malloc was called for either backing array or the struct
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*10, 1, 0);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}

int test_destroy_null(alloc_tester* t1) {

    assert(destroy_array_list(NULL, NULL));
    assert(destroy_array_list((void*)0xffff, NULL));
    assert(destroy_array_list(NULL, (list_op)0xffff));

    return 0;
}


int test_destroy(alloc_tester* t1) {
    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    bool after_freed[10] = { true, true, true, true, true,
                             true, true, true, true, true };


    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // do the destroy
    assert(!destroy_array_list(arr, fake_free_func));

    // all data pointers freed
    bool freed[10];
    memset(freed, 0, sizeof(bool)*10);
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            if(fake_free.arr[j] == before[i]) {
                freed[i] = true;
            }
        }
    }
    assert(memcmp(after_freed, freed, sizeof(bool)*10) == 0);


    // all memory freed
    assert(all_freed(t1));

    cleanup(&t);

    return 0;
}

int test_trim_null(alloc_tester* t1) {

    assert(trim_to_size(NULL));

    return 0;
}

int test_trim(alloc_tester* t1) {
    t1->malloc_calloc_budget = 2;
    array_list_t* arr = ta_create_array_list_2();
    assert(t1->alloc_size == 2);

    void* before[10] = {(void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                        (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    // setup before list
    arr->size = 10;
    memcpy(arr->entries, before, sizeof(void*)*10);

    // trim can do one realloc
    t1->realloc_budget = 1;

    // do the trim
    assert(!trim_to_size(arr));

    assert(arr->capacity == 10);
    assert(find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*10, 1, 0));

    cleanup(&t);

    return 0;
}


int test_ec_create(alloc_tester* t1) {
    // 0 malloc calls succeed
    t1->malloc_calloc_budget = 1;


    circ_list* ll = create_linked_list();

    // should've return valid value
    assert(ll);

    // well formed ll
    assert(ll->size == 0);
    assert(ll->head == NULL);

    // malloc/calloc called 1 time
    assert(t1->malloc_calls + t1->calloc_calls == 1);

    // free has not been called
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));



    // right malloc call made
    void* malloc_ll = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(circ_list), 1, 0);
    void* calloc_ll = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(circ_list), 1, 0);
    assert(calloc_ll || malloc_ll);

    // free everything
    cleanup(&t);

    return 0;
}

int test_ec_destroy(alloc_tester* t1) {

    // 4 malloc calls succeed (for making list)
    t1->malloc_calloc_budget = 4;

    circ_list* ll = ta_create_ll_2();

    void* before[3] = { (void*)1, (void*)2, (void*)3 };
    bool after_freed[3] = {true, true, true};

    // do the destroy
    assert(!destroy_linked_list(ll, fake_free_func));

    // all data pointers freed
    bool freed[3];
    memset(freed, 0, sizeof(bool)*3);
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(fake_free.arr[j] == before[i]) {
                freed[i] = true;
            }
        }
    }
    assert(memcmp(after_freed, freed, 3* sizeof(bool)) == 0);

    // all memory freed
    assert(all_freed(t1));

    cleanup(&t);

    return 0;
}

int test_ec_shallow(alloc_tester* t1) {
    // 8 malloc calls succeed (for making list 2 full lists)
    t1->malloc_calloc_budget = 8;

    circ_list* ll = ta_create_ll_2();

    // make the 2nd list
    circ_list* ll2 = shallow_copy_linked_list(ll);
    assert(ll2);

    // check the calls
    assert(t1->malloc_calls + t1->calloc_calls == 8);

    void* m_ll_call = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(circ_list), 4, 0);
    void* c_ll_call = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(circ_list), 4, 0);
    void* m_ll_call_1 = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(l_node), 4, 0);
    void* c_ll_call_1 = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(l_node), 4, 0);
    void* m_ll_call_2 = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(l_node), 4, 1);
    void* c_ll_call_2 = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(l_node), 4, 1);
    void* m_ll_call_3 = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(l_node), 4, 2);
    void* c_ll_call_3 = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(l_node), 4, 2);
    assert(m_ll_call || c_ll_call);
    assert(m_ll_call_1 || c_ll_call_1);
    assert(m_ll_call_2 || c_ll_call_2);
    assert(m_ll_call_3 || c_ll_call_3);

    // check ll1
    assert(ll->size == 3);
    assert(ll->head->data == (void*)1);
    assert(ll->head->next->data == (void*)2);
    assert(ll->head->next->next->data == (void*)3);
    assert(ll->head->next->next->next == ll->head);

    // check ll2
    assert(ll2->size == 3);
    assert(ll2->head->data == (void*)1);
    assert(ll2->head->next->data == (void*)2);
    assert(ll2->head->next->next->data == (void*)3);
    assert(ll2->head->next->next->next == ll2->head);

    // check that ll1 and ll2 are not the same
    assert(ll != ll2);
    assert(ll->head != ll2->head);
    assert(ll->head->next != ll2->head->next);
    assert(ll->head->next->next != ll2->head->next->next);

    cleanup(&t);

    return 0;
}

int test_ec_shallow_fail(alloc_tester* t1) {
    // 7 malloc calls succeed (for making list 1 full list and the 2nd list fails)
    t1->malloc_calloc_budget = 7;

    circ_list* ll = ta_create_ll_2();

    // make the 2nd list
    circ_list* ll2 = shallow_copy_linked_list(ll);
    assert(!ll2);

    // check the calls
    assert(t1->malloc_calls + t1->calloc_calls == 8);

    // check ll is still ok
    assert(ll->size == 3);
    assert(ll->head->data == (void*)1);
    assert(ll->head->next->data == (void*)2);
    assert(ll->head->next->next->data == (void*)3);
    assert(ll->head->next->next->next == ll->head);

    // free the first list
    free(ll->head->next->next);
    free(ll->head->next);
    free(ll->head);
    free(ll);

    // check all memory freed
    assert(all_freed(t1));

    cleanup(&t);

    return 0;
}

int test_ec_deep(alloc_tester* t1) {
    // 8 malloc calls succeed (for making list 2 full lists)
    t1->malloc_calloc_budget = 8;

    circ_list* ll = ta_create_ll_2();

    // make the 2nd list
    circ_list* ll2 = deep_copy_linked_list(ll, fake_copy_func, fake_free_func);
    assert(ll2);

    // check the calls
    assert(t1->malloc_calls + t1->calloc_calls == 8);

    void* m_ll_call = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(circ_list), 4, 0);
    void* c_ll_call = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(circ_list), 4, 0);
    void* m_ll_call_1 = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(l_node), 4, 0);
    void* c_ll_call_1 = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(l_node), 4, 0);
    void* m_ll_call_2 = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(l_node), 4, 1);
    void* c_ll_call_2 = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(l_node), 4, 1);
    void* m_ll_call_3 = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(l_node), 4, 2);
    void* c_ll_call_3 = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(l_node), 4, 2);
    assert(m_ll_call || c_ll_call);
    assert(m_ll_call_1 || c_ll_call_1);
    assert(m_ll_call_2 || c_ll_call_2);
    assert(m_ll_call_3 || c_ll_call_3);

    // check ll1
    assert(ll->size == 3);
    assert(ll->head->data == (void*)1);
    assert(ll->head->next->data == (void*)2);
    assert(ll->head->next->next->data == (void*)3);
    assert(ll->head->next->next->next == ll->head);

    // check ll2
    assert(ll2->size == 3);
    assert(ll2->head->data == (void*)1);
    assert(ll2->head->next->data == (void*)2);
    assert(ll2->head->next->next->data == (void*)3);
    assert(ll2->head->next->next->next == ll2->head);

    // check that ll1 and ll2 are not the same
    assert(ll != ll2);
    assert(ll->head != ll2->head);
    assert(ll->head->next != ll2->head->next);
    assert(ll->head->next->next != ll2->head->next->next);

    // check that fake_copy was called
    bool expected_copied[3] = {true, true, true};
    void* before[3] = { (void*)1, (void*)2, (void*)3 };
    bool copied[3];
    memset(copied, 0, sizeof(bool)*3);
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(fake_copy.arr[j] == before[i]) {
                copied[i] = true;
            }
        }
    }
    assert(memcmp(expected_copied, copied, 3* sizeof(bool)) == 0);

    cleanup(&t);

    return 0;
}

int test_ec_deep_fail(alloc_tester* t1) {
    // 7 malloc calls succeed (for making list 1 full list and the 2nd list fails)
    t1->malloc_calloc_budget = 7;

    circ_list* ll = ta_create_ll_2();

    // make the 2nd list
    circ_list* ll2 = deep_copy_linked_list(ll, fake_copy_func, fake_free_func);
    assert(!ll2);

    // check the calls
    assert(t1->malloc_calls + t1->calloc_calls == 8);

    // check ll still ok
    assert(ll->size == 3);
    assert(ll->head->data == (void*)1);
    assert(ll->head->next->data == (void*)2);
    assert(ll->head->next->next->data == (void*)3);
    assert(ll->head->next->next->next == ll->head);

    // free ll 1
    free(ll->head->next->next);
    free(ll->head->next);
    free(ll->head);
    free(ll);

    // check all memory freed
    assert(all_freed(t1));

    // check that there's a call to fake free for each call to fake copy
    // freed[copy_idx] = true/false
    bool freed[3];
    bool expected_freed[3] = {true, true, true};
    memcpy(freed, expected_freed, sizeof(freed));
    for(int i = 0; i < 3; i++) {
        if(fake_copy.arr[i]) {
            freed[i] = false;
        }
    }
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(fake_free.arr[i] == fake_copy.arr[j]) {
                freed[j] = true;
            }
        }
    }
    assert(memcmp(expected_freed, freed, sizeof(freed)) == 0);

    cleanup(&t);

    return 0;
}

/**
 * [1,2,3]
 * add idx:0, dat: 4
 * add idx:1, dat: 5
 * [4,5,1,2,3]
 */
int test_ec_add(alloc_tester* t1) {
    // 6 malloc calls succeed (for making list 1 full list and adding 2 nodes
    t1->malloc_calloc_budget = 6;

    circ_list* ll = ta_create_ll_2();

    assert(!add_to_linked_list(ll, 0, (void*)4));
    assert(!add_to_linked_list(ll, 1, (void*)5));

    // check calls
    assert(t1->malloc_calls + t1->calloc_calls == 6);
    assert(t1->free_calls == 0);
    void* m_ll_call_1 = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(l_node), 2, 0);
    void* c_ll_call_1 = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(l_node), 2, 0);
    void* m_ll_call_2 = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(l_node), 2, 1);
    void* c_ll_call_2 = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(l_node), 2, 1);
    assert(m_ll_call_1 || c_ll_call_1);
    assert(m_ll_call_2 || c_ll_call_2);

    // check ll structure
    assert(ll->size == 5);
    assert(ll->head->data == (void*)4);
    assert(ll->head->next->data == (void*)5);
    assert(ll->head->next->next->data == (void*)1);
    assert(ll->head->next->next->next->data == (void*)2);
    assert(ll->head->next->next->next->next->data == (void*)3);
    assert(ll->head->next->next->next->next->next->data == (void*)4);

    cleanup(&t);

    return 0;
}


int test_ec_add_fail(alloc_tester* t1) {
    // 4 malloc calls succeed (for making list 1 full list
    t1->malloc_calloc_budget = 4;

    circ_list* ll = ta_create_ll_2();

    assert(add_to_linked_list(ll, 1, (void*)4));

    // check calls
    assert(t1->malloc_calls + t1->calloc_calls == 5);
    alloc_info* m_ll_call_1 = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(l_node), 2, 0);
    alloc_info* c_ll_call_1 = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(l_node), 2, 0);
    assert(!m_ll_call_1->success || !c_ll_call_1->success);

    // check ll structure not changed
    assert(ll->size == 3);
    assert(ll->head->data == (void*)1);
    assert(ll->head->next->data == (void*)2);
    assert(ll->head->next->next->data == (void*)3);
    assert(ll->head->next->next->next->data == (void*)1);

    cleanup(&t);

    return 0;
}

int test_ec_get(alloc_tester* t1) {
    // 4 malloc calls succeed (for making list 1 full list
    t1->malloc_calloc_budget = 4;

    circ_list* ll = ta_create_ll_2();

    void* data;
    assert(!get_from_linked_list(ll, 1, &data));
    assert(data == (void*)2);

    // check calls
    assert(t1->free_calls == 0);
    assert(t1->malloc_calls + t1->calloc_calls == 4);

    // check ll structure not changed
    assert(ll->size == 3);
    assert(ll->head->data == (void*)1);
    assert(ll->head->next->data == (void*)2);
    assert(ll->head->next->next->data == (void*)3);
    assert(ll->head->next->next->next->data == (void*)1);

    cleanup(&t);

    return 0;
}

int test_ec_rem(alloc_tester* t1) {

    // 4 malloc calls succeed (for making list 1 full list
    t1->malloc_calloc_budget = 4;

    circ_list* ll = ta_create_ll_2();

    void* data;
    assert(!remove_from_linked_list(ll, 1, &data));
    assert(data == (void*)2);

    // check calls
    assert(t1->free_calls == 1);
    assert(t1->malloc_calls + t1->calloc_calls == 4);
    alloc_info* free_ll_call = find_call_in_allocs_by_size(&t, FREE_ALLOC, sizeof(l_node), 1, 0);
    assert(!free_ll_call);

    // check ll structure not changed
    assert(ll->size == 2);
    assert(ll->head->data == (void*)1);
    assert(ll->head->next->data == (void*)3);
    assert(ll->head->next->next->data == (void*)1);

    cleanup(&t);

    return 0;

}

int test_ec_contains(alloc_tester* t1) {

    // 4 malloc calls succeed (for making list 1 full list
    t1->malloc_calloc_budget = 4;

    // [3,75,19]
    circ_list* ll = ta_create_ll_2();
    ll->head->data = (void*)3;
    ll->head->next->data = (void*)75;
    ll->head->next->next->data = (void*)19;

    void* data;
    assert(linked_list_contains(ll, (void*)2, fake_eq_func, &data));
    assert(data == (void*)3);
    assert(linked_list_contains(ll, (void*)74, fake_eq_func, &data));
    assert(data == (void*)75);
    assert(linked_list_contains(ll, (void*)18, fake_eq_func, &data));
    assert(data == (void*)19);
    assert(!linked_list_contains(ll, (void*)5, fake_eq_func, &data));
    assert(!linked_list_contains(ll, (void*)2222, fake_eq_func, &data));
    assert(!linked_list_contains(ll, (void*)10112, fake_eq_func, &data));

    cleanup(&t);

    return 0;
}

int test_ec_zip(alloc_tester* t1) {

    // 8 malloc calls succeed (for making list 2 full lists
    t1->malloc_calloc_budget = 8;

    // [1,3,5]
    circ_list* ll = ta_create_ll_2();
    ll->head->data = (void*)1;
    ll->head->next->data = (void*)3;
    ll->head->next->next->data = (void*)5;

    // [2,4,6]
    circ_list* ll2 = ta_create_ll_2();
    ll2->head->data = (void*)2;
    ll2->head->next->data = (void*)4;
    ll2->head->next->next->data = (void*)6;

    // no calls
    assert(t1->malloc_calls + t1->calloc_calls + t1->realloc_calls + t1->free_calls == 8);

    assert(!zip(ll, ll2));

    assert(ll->size == 6);
    assert(ll->head->data == (void*)1);
    assert(ll->head->next->data == (void*)2);
    assert(ll->head->next->next->data == (void*)3);
    assert(ll->head->next->next->next->data == (void*)4);
    assert(ll->head->next->next->next->next->data == (void*)5);
    assert(ll->head->next->next->next->next->next->data == (void*)6);

    assert(ll2->size == 6);
    assert(ll2->head->data == (void*)1);
    assert(ll2->head->next->data == (void*)2);
    assert(ll2->head->next->next->data == (void*)3);
    assert(ll2->head->next->next->next->data == (void*)4);
    assert(ll2->head->next->next->next->next->data == (void*)5);
    assert(ll2->head->next->next->next->next->next->data == (void*)6);

    cleanup(t1);

    return 0;
}
