#include <assert.h>
#include "grader.h"
#include "array_list.h"

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

};

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
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 1);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 1);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 1);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 1);
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
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 3);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 3);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 3);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 3);

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
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 2);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 2);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 2);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 2);

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
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 1);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 1);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 1);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 1);

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
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 3);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 3);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 3);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 3);

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
    void* malloc_array = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(array_list_t), 2);
    void* calloc_array = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(array_list_t), 2);
    void* malloc_back = find_call_in_allocs_by_size(&t, M_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 2);
    void* calloc_back = find_call_in_allocs_by_size(&t, C_ALLOC, sizeof(void*)*INITIAL_CAPACITY, 2);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*20, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*40, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*40, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*10, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*10, 1);

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
    void* realloc_array = find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*10, 1);

    assert(realloc_array);

    cleanup(&t);

    return 0;
}


void fake_free_func(void* data) {
    fake_free.arr[fake_free.size++] = data;
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
    assert(find_call_in_allocs_by_size(&t, RE_ALLOC, sizeof(void*)*10, 1));

    cleanup(&t);

    return 0;
}
