#include "grader.h"
#include "list_array.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

// globals
fake_free_func_t fake_free;
fake_copy_func_t fake_copy;
unsigned int default_backing_capacity = 10;

// tests
student_test tests[NUM_TESTS] = {

    // create node - 12 pts
    {test_create_node_second_alloc_null,    "test_create_node_second_alloc_null",   5, false},
    {test_create_node,                      "test_create_node",                     7, false},

    // create list - 10 pts
    {test_create_list_third_alloc_null,     "test_create_list_third_alloc_null",    5, false},
    {test_create_list,                      "test_create_list",                     5, false},

    // set - 48 pts
    {test_set_null_params,                  "test_set_null_params",                 6, false},
    {test_set_easy_1,                       "test_set_easy_1",                      6, false},
    {test_set_easy_2,                       "test_set_easy_2",                      6, false},
    {test_set_easy_3,                       "test_set_easy_3",                      6, false},
    {test_set_easy_4,                       "test_set_easy_4",                      6, false},
    {test_set_hard_3,                       "test_set_hard_3",                      6, false},
    {test_set_hard_4,                       "test_set_hard_4",                      6, false},
    {test_set_hard_4_fail,                  "test_set_hard_4_fail",                 6, false},

    // get - 16 pts
    {test_get_2,                            "test_get_2",                           8, false},
    {test_get_3,                            "test_get_3",                           8, false},

    // destroy - 14 pts
    {test_destroy,                          "test_destroy",                         14, false},



};

int setup() {
    memset(&t, 0, sizeof(alloc_tester));
    memset(&fake_free, 0, sizeof(fake_free_func_t));
    memset(&fake_copy, 0, sizeof(fake_copy_func_t));
    return 0;
}

int teardown() {
    return 0;
}

/**
 * Creates list array with 0 nodes
 * - 1 call to malloc
 *
 * @return
 */
list_array* ta_create_list_array_empty() {

    // create empty list
    list_array* arr = malloc(sizeof(list_array));
    arr->node_backing_capacity = default_backing_capacity;

    // setup links
    arr->head = NULL;

    return arr;
}


/**
 * Creates list array with 1 nodes
 * - 3 calls to malloc
 *
 * @return
 */
list_array* ta_create_list_array_nodes_1() {

    // create list
    list_array* arr = malloc(sizeof(list_array));
    arr->node_backing_capacity = default_backing_capacity;
    arr->head = NULL;

    // node 0
    list_node* node0 = malloc(sizeof(list_node));
    node0->arr = calloc(default_backing_capacity, sizeof(void*));
    node0->next = NULL;

    // setup links
    arr->head = node0;
    node0->next = NULL;

    return arr;
}


/**
 * Creates list array with 3 nodes
 * - 7 call to malloc
 *
 * @return
 */
list_array* ta_create_list_array_nodes_3() {

    // create list
    list_array* arr = malloc(sizeof(list_array));
    arr->node_backing_capacity = default_backing_capacity;

    // node 0
    list_node* node0 = malloc(sizeof(list_node));
    node0->arr = calloc(default_backing_capacity, sizeof(void*));

    // node 1
    list_node* node1 = malloc(sizeof(list_node));
    node1->arr = calloc(default_backing_capacity, sizeof(void*));

    // node 2
    list_node* node2 = malloc(sizeof(list_node));
    node2->arr = calloc(default_backing_capacity, sizeof(void*));

    // setup links
    arr->head = node0;
    node0->next = node1;
    node1->next = node2;
    node2->next = NULL;

    return arr;
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

int test_create_node_second_alloc_null(alloc_tester* t1) {

    // 1 malloc calls succeeds
    t1->malloc_calloc_budget = 1;

    // create the node
    list_node* node = create_node(default_backing_capacity);

    // should've return NULL because it failed
    assert(node == NULL);

    // malloc/calloc called 2 times (because it failed on 2nd call)
    assert(t1->malloc_calls + t1->calloc_calls == 2);

    // right malloc calls made
    assert(t1->alloc_size == 3);
    bool found_node_back = false;
    bool found_node = false;
    for(int i = 0; i < 3; i++) {
        if(t1->allocs[i].alloc_size == sizeof(list_node)) found_node = true;
        if(t1->allocs[i].alloc_size == (sizeof(void*)*default_backing_capacity)) found_node_back = true;
    }
    assert(found_node_back && found_node);

    // free has been called once
    assert(t1->free_calls == 1);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // all memory should be freed
    assert(all_freed(&t));

    // free everything
    cleanup(&t);

    return 0;
}

int test_create_node(alloc_tester* t1) {

    // 2 malloc calls succeed
    t1->malloc_calloc_budget = 2;

    // create the node
    list_node* node = create_node(default_backing_capacity);

    //  good node
    assert(node);
    assert(node->next == NULL);
    assert(node->arr);

    // malloc/calloc called 2 times
    assert(t1->malloc_calls + t1->calloc_calls == 2);

    // right malloc calls made
    assert(t1->alloc_size == 2);
    bool found_node_back = false;
    bool found_node = false;
    for(int i = 0; i < 3; i++) {
        if(t1->allocs[i].alloc_size == sizeof(list_node)) found_node = true;
        if(t1->allocs[i].alloc_size == (sizeof(void*)*default_backing_capacity)) found_node_back = true;
    }
    assert(found_node_back && found_node);

    // free has not been called
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // free everything
    cleanup(&t);

    return 0;

}

int test_create_list_third_alloc_null(alloc_tester* t1) {

    // 2 malloc calls succeed
    t1->malloc_calloc_budget = 2;

    // create the list
    list_array* arr = create_list(default_backing_capacity);

    // should've return NULL because it failed
    assert(arr == NULL);

    // malloc/calloc called 3 times (because it failed on third call)
    assert(t1->malloc_calls + t1->calloc_calls == 3);

    // free has been called twice
    assert(t1->free_calls == 2);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // all memory should be freed
    assert(all_freed(&t));

    // right malloc/calloc calls made (3 malloc/callocs + 2 frees)
    assert(t1->alloc_size == 5);
    bool found_back = false;
    bool found_list_array = false;
    bool found_list_node = false;
    for(int i = 0; i < 5; i++) {
        if(t1->allocs[i].alloc_size == sizeof(list_array)) found_list_array = true;
        if(t1->allocs[i].alloc_size == sizeof(list_node)) found_list_node = true;
        if(t1->allocs[i].alloc_size == (sizeof(void*)*default_backing_capacity)) found_back = true;
    }
    assert(found_back && found_list_array && found_list_node);

    // free everything
    cleanup(&t);

    return 0;
}

int test_create_list(alloc_tester* t1) {

    // 3 malloc calls succeed
    t1->malloc_calloc_budget = 3;

    // create the list
    list_array* arr = create_list(default_backing_capacity);

    // list should be good
    assert(arr);

    // malloc/calloc called 3 times (2 for node, 1 for list array)
    assert(t1->malloc_calls + t1->calloc_calls == 3);

    // free has been called no times
    assert(t1->free_calls == 0);

    // no invalid frees made
    assert(!invalid_frees(&t));

    // right malloc/calloc calls made (3 malloc/callocs + 2 frees)
    assert(t1->alloc_size == 3);
    bool found_back = false;
    bool found_list_array = false;
    bool found_list_node = false;
    for(int i = 0; i < 3; i++) {
        if(t1->allocs[i].alloc_size == sizeof(list_array)) found_list_array = true;
        if(t1->allocs[i].alloc_size == sizeof(list_node)) found_list_node = true;
        if(t1->allocs[i].alloc_size == (sizeof(void*)*default_backing_capacity)) found_back = true;
    }
    assert(found_back && found_list_array && found_list_node);

    // list array has right values
    void* backing[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    assert(arr->head);
    assert(arr->node_backing_capacity = default_backing_capacity);
    assert(arr->head->next == NULL);
    assert(memcmp(arr->head->arr, backing, sizeof(void*)*10) == 0);

    // free everything
    cleanup(&t);

    return 0;
}

int test_set_null_params(alloc_tester* t1) {

    assert(set_list(NULL, 0, NULL, NULL) == SET_FAILURE);

    // no allocs
    assert(t1->alloc_size == 0);

    // free everything
    cleanup(&t);

    return 0;
}

// before: nodes:1, idx:0, data_in:11
// after: nodes:1, data_out:3
int test_set_easy_1(alloc_tester* t1) {

    // 3 malloc calls succeed
    t1->malloc_calloc_budget = 3;

    // setup
    list_array* arr = ta_create_list_array_nodes_1();
    void* node0_arr_before[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    void* node0_arr_after[10] = { (void*)11, (void*)1, (void*)4, (void*)1, (void*)5,
                              (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    memcpy(arr->head->arr, node0_arr_before, sizeof(void*)*10);
    list_array arr_before = *arr;
    list_node node0_before = *arr->head;

    // run the set
    void* ret;
    assert(set_list(arr, 0, (void*)11, &ret) == SET_SUCCESS);

    // test post conditions
    assert(ret == (void*)3);
    assert(arr->head == arr_before.head);
    assert(arr->node_backing_capacity == arr_before.node_backing_capacity);
    assert(arr->head->next == node0_before.next);
    assert(arr->head->arr == node0_before.arr);


    assert(memcmp(node0_arr_after, arr->head->arr, sizeof(void*)*10) == 0);


    // only 3 allocs
    assert(t1->alloc_size == 3);

    // free everything
    cleanup(&t);

    return 0;
}

// before: nodes:1, idx:0, data_in:NULL
// after: nodes:1, data_out:3
int test_set_easy_2(alloc_tester* t1) {

    // 3 malloc calls succeed
    t1->malloc_calloc_budget = 3;

    // setup
    list_array* arr = ta_create_list_array_nodes_1();
    void* node0_arr_before[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                   (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    void* node0_arr_after[10] = { NULL, (void*)1, (void*)4, (void*)1, (void*)5,
                                  (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    memcpy(arr->head->arr, node0_arr_before, sizeof(void*)*10);
    list_array arr_before = *arr;
    list_node node0_before = *arr->head;

    // run the set
    void* ret;
    assert(set_list(arr, 0, NULL, &ret) == SET_SUCCESS);

    // test post conditions
    assert(ret == (void*)3);
    assert(arr->head == arr_before.head);
    assert(arr->node_backing_capacity == arr_before.node_backing_capacity);
    assert(arr->head->next == node0_before.next);
    assert(arr->head->arr == node0_before.arr);


    assert(memcmp(node0_arr_after, arr->head->arr, sizeof(void*)*10) == 0);


    // only 3 allocs
    assert(t1->alloc_size == 3);

    // free everything
    cleanup(&t);

    return 0;
}

// before: nodes:1, idx:9, data_in:33
// after: nodes:1, data_out:3
int test_set_easy_3(alloc_tester* t1) {

    // 3 malloc calls succeed
    t1->malloc_calloc_budget = 3;

    // setup
    list_array* arr = ta_create_list_array_nodes_1();
    void* node0_arr_before[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                   (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    void* node0_arr_after[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                  (void*)9, (void*)2, (void*)6, (void*)5, (void*)33};
    memcpy(arr->head->arr, node0_arr_before, sizeof(void*)*10);
    list_array arr_before = *arr;
    list_node node0_before = *arr->head;

    // run the set
    void* ret;
    assert(set_list(arr, 9, (void*)33, &ret) == SET_SUCCESS);

    // test post conditions
    assert(ret == (void*)3);
    assert(arr->head == arr_before.head);
    assert(arr->node_backing_capacity == arr_before.node_backing_capacity);
    assert(arr->head->next == node0_before.next);
    assert(arr->head->arr == node0_before.arr);


    assert(memcmp(node0_arr_after, arr->head->arr, sizeof(void*)*10) == 0);


    // only 3 allocs
    assert(t1->alloc_size == 3);

    // free everything
    cleanup(&t);

    return 0;
}

// before: nodes:3, idx:12, data_in:55
// after: nodes:3, data_out:9
int test_set_easy_4(alloc_tester* t1) {

    // 7 malloc calls succeed
    t1->malloc_calloc_budget = 7;

    // arrays
    void* node0_arr_before[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                   (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    void* node1_arr_before[10] = { (void*)5, (void*)8, (void*)9, (void*)7, (void*)9,
                                   (void*)3, (void*)2, (void*)3, (void*)8, (void*)4};
    void* node2_arr_before[10] = { (void*)6, (void*)2, (void*)6, (void*)4, (void*)3,
                                   (void*)3, (void*)8, (void*)3, (void*)2, (void*)7};

    void* node0_arr_after[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                  (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    void* node1_arr_after[10] = { (void*)5, (void*)8, (void*)55, (void*)7, (void*)9,
                                  (void*)3, (void*)2, (void*)3, (void*)8, (void*)4};
    void* node2_arr_after[10] = { (void*)6, (void*)2, (void*)6, (void*)4, (void*)3,
                                  (void*)3, (void*)8, (void*)3, (void*)2, (void*)7};

    // setup
    list_array* arr = ta_create_list_array_nodes_3();
    memcpy(arr->head->arr, node0_arr_before, sizeof(void*)*10);
    memcpy(arr->head->next->arr, node1_arr_before, sizeof(void*)*10);
    memcpy(arr->head->next->next->arr, node2_arr_before, sizeof(void*)*10);


    list_array arr_before = *arr;
    list_node node0_before = *arr->head;
    list_node node1_before = *arr->head->next;
    list_node node2_before = *arr->head->next->next;

    // run the set
    void* ret;
    assert(set_list(arr, 12, (void*)55, &ret) == SET_SUCCESS);

    list_node* node0_after = arr->head;
    list_node* node1_after = arr->head->next;
    list_node* node2_after = arr->head->next->next;

    // test post conditions
    assert(ret == (void*)9);
    assert(arr->head == arr_before.head);
    assert(arr->node_backing_capacity == arr_before.node_backing_capacity);
    assert(node0_after->next == node0_before.next);
    assert(node0_after->arr == node0_before.arr);
    assert(node1_after->next == node1_before.next);
    assert(node1_after->arr == node1_before.arr);
    assert(node2_after->next == node2_before.next);
    assert(node2_after->arr == node2_before.arr);


    assert(memcmp(node0_arr_after, node0_after->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node1_arr_after, node1_after->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node2_arr_after, node2_after->arr, sizeof(void*)*10) == 0);


    // only 7 allocs (for list array with 3 nodes)
    assert(t1->alloc_size == 7);

    // free everything
    cleanup(&t);

    return 0;
}

// TEST NOTE USED
// before: nodes:0, idx:7, data_in:77
// after: nodes:1, data_out:NULL
int test_set_hard_1(alloc_tester* t1) {

    // 3 malloc calls succeed
    t1->malloc_calloc_budget = 3;

    // arrays
    void* node0_arr_after[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, (void*)77, NULL, NULL };

    // setup
    list_array* arr = ta_create_list_array_empty();

    // run the set
    void* ret;
    assert(set_list(arr, 7, (void*)77, &ret) == SET_SUCCESS);

    // test post conditions
    assert(ret == NULL);
    assert(arr->node_backing_capacity == default_backing_capacity);
    assert(arr->head);
    assert(!arr->head->next);

    bool found_back = false;
    bool found_list_node = false;
    for(int i = 3; i >= 1; i--) {
        if(t1->allocs[i].alloc_size == sizeof(list_node)) found_list_node = true;
        if(t1->allocs[i].alloc_size == (sizeof(void*)*default_backing_capacity)) found_back = true;
    }
    assert(found_back && found_list_node);
    assert(memcmp(node0_arr_after, arr->head->arr, sizeof(void*)*10) == 0);


    // only 3 allocs
    assert(t1->alloc_size == 3);

    // free everything
    cleanup(&t);

    return 0;
}

// TEST_NOT_USED
// before: nodes:0, idx:27, data_in:88
// after: nodes:3, data_out:NULL
int test_set_hard_2(alloc_tester* t1) {

    // 7 malloc calls succeed
    t1->malloc_calloc_budget = 7;

    // arrays
    void* node0_node1_arr_after[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    void* node2_arr_after[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, (void*)88, NULL, NULL };

    // setup
    list_array* arr = ta_create_list_array_empty();

    // run the set
    void* ret;
    assert(set_list(arr, 27, (void*)88, &ret) == SET_SUCCESS);

    // test post conditions
    list_node* node0 = arr->head;
    list_node* node1 = arr->head->next;
    list_node* node2 = arr->head->next->next;

    assert(ret == NULL);
    assert(arr->node_backing_capacity == default_backing_capacity);
    assert(node0);
    assert(node1);
    assert(node2);

    int found_back = 0;
    int found_list_node = 0;
    for(int i = 6; i >= 3; i--) {
        if(t1->allocs[i].alloc_size == sizeof(list_node)) found_list_node++;
        if(t1->allocs[i].alloc_size == (sizeof(void*)*default_backing_capacity)) found_back++;
    }
    assert(found_back == 2);
    assert(found_list_node == 2);

    assert(memcmp(node0_node1_arr_after, node0->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node0_node1_arr_after, node1->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node2_arr_after, node2->arr, sizeof(void*)*10) == 0);


    // only 7 allocs
    assert(t1->alloc_size == 7);

    // free everything
    cleanup(&t);

    return 0;
}

// before: nodes:1, idx:36, data_in:99
// after: nodes:4, data_out:NULL
int test_set_hard_3(alloc_tester* t1) {

    // 9 malloc calls succeed
    t1->malloc_calloc_budget = 9;

    // arrays
    void* node0_arr_before[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                   (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    void* node1_node2_arr_after[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    void* node3_arr_after[10] = { NULL, NULL, NULL, NULL, NULL, NULL, (void*)99, NULL, NULL, NULL };

    // setup
    list_array* arr = ta_create_list_array_nodes_1();
    memcpy(arr->head->arr, node0_arr_before, 10* sizeof(void*));

    // run the set
    void* ret;
    assert(set_list(arr, 36, (void*)99, &ret) == SET_SUCCESS);

    // test post conditions
    list_node* node0 = arr->head;
    list_node* node1 = node0->next;
    list_node* node2 = node1->next;
    list_node* node3 = node2->next;

    assert(ret == NULL);
    assert(arr->node_backing_capacity == default_backing_capacity);
    assert(node0);
    assert(node1);
    assert(node2);
    assert(node3);

    int found_back = 0;
    int found_list_node = 0;
    for(int i = 6; i >= 3; i--) {
        if(t1->allocs[i].alloc_size == sizeof(list_node)) found_list_node++;
        if(t1->allocs[i].alloc_size == (sizeof(void*)*default_backing_capacity)) found_back++;
    }
    assert(found_back == 2);
    assert(found_list_node == 2);

    assert(memcmp(node0_arr_before, node0->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node1_node2_arr_after, node1->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node1_node2_arr_after, node2->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node3_arr_after, node3->arr, sizeof(void*)*10) == 0);


    // only 7 allocs
    assert(t1->alloc_size == 9);

    // free everything
    cleanup(&t);

    return 0;
}

// before: nodes:3, idx:71, data_in:111
// after: nodes:8, data_out:NULL
int test_set_hard_4(alloc_tester* t1) {

    // 17 malloc calls succeed
    t1->malloc_calloc_budget = 17;

    // arrays
    void* node0_arr_before[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                   (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    void* node1_arr_before[10] = { (void*)5, (void*)8, (void*)9, (void*)7, (void*)9,
                                   (void*)3, (void*)2, (void*)3, (void*)8, (void*)4};
    void* node2_arr_before[10] = { (void*)6, (void*)2, (void*)6, (void*)4, (void*)3,
                                   (void*)3, (void*)8, (void*)3, (void*)2, (void*)7};

    void* node3456_arr_after[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    void* node7_arr_after[10] = { NULL, (void*)111, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

    // setup
    list_array* arr = ta_create_list_array_nodes_3();
    memcpy(arr->head->arr, node0_arr_before, 10* sizeof(void*));
    memcpy(arr->head->next->arr, node1_arr_before, 10* sizeof(void*));
    memcpy(arr->head->next->next->arr, node2_arr_before, 10* sizeof(void*));

    // run the set
    void* ret;
    assert(set_list(arr, 71, (void*)111, &ret) == SET_SUCCESS);

    // test post conditions
    list_node* node0 = arr->head;
    list_node* node1 = node0->next;
    list_node* node2 = node1->next;
    list_node* node3 = node2->next;
    list_node* node4 = node3->next;
    list_node* node5 = node4->next;
    list_node* node6 = node5->next;
    list_node* node7 = node6->next;


    assert(ret == NULL);
    assert(arr->node_backing_capacity == default_backing_capacity);
    assert(node0);
    assert(node1);
    assert(node2);
    assert(node3);
    assert(node4);
    assert(node5);
    assert(node6);
    assert(node7);

    int found_back = 0;
    int found_list_node = 0;
    for(int i = 6; i >= 3; i--) {
        if(t1->allocs[i].alloc_size == sizeof(list_node)) found_list_node++;
        if(t1->allocs[i].alloc_size == (sizeof(void*)*default_backing_capacity)) found_back++;
    }
    assert(found_back == 2);
    assert(found_list_node == 2);

    assert(memcmp(node0_arr_before, node0->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node1_arr_before, node1->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node2_arr_before, node2->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node3456_arr_after, node3->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node3456_arr_after, node4->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node3456_arr_after, node5->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node3456_arr_after, node6->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node7_arr_after, node7->arr, sizeof(void*)*10) == 0);


    // only 17 allocs
    assert(t1->alloc_size == 17);

    // free everything
    cleanup(&t);

    return 0;
}

// before: nodes:3, idx:71, data_in:111
// 2nd node created failed
// after: nodes:4, data_out:NULL
int test_set_hard_4_fail(alloc_tester* t1) {

    // 10 malloc calls succeed (fails on second create node)
    t1->malloc_calloc_budget = 10;

    // arrays
    void* node0_arr_before[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                   (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    void* node1_arr_before[10] = { (void*)5, (void*)8, (void*)9, (void*)7, (void*)9,
                                   (void*)3, (void*)2, (void*)3, (void*)8, (void*)4};
    void* node2_arr_before[10] = { (void*)6, (void*)2, (void*)6, (void*)4, (void*)3,
                                   (void*)3, (void*)8, (void*)3, (void*)2, (void*)7};

    void* node3_arr_after[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

    // setup
    list_array* arr = ta_create_list_array_nodes_3();
    memcpy(arr->head->arr, node0_arr_before, 10* sizeof(void*));
    memcpy(arr->head->next->arr, node1_arr_before, 10* sizeof(void*));
    memcpy(arr->head->next->next->arr, node2_arr_before, 10* sizeof(void*));

    // run the set
    void* ret;
    assert(set_list(arr, 71, (void*)111, &ret) == SET_FAILURE);

    // test post conditions
    list_node* node0 = arr->head;
    list_node* node1 = node0->next;
    list_node* node2 = node1->next;
    list_node* node3 = node2->next;

    assert(arr->node_backing_capacity == default_backing_capacity);
    assert(node0);
    assert(node1);
    assert(node2);
    assert(node3);

    int found_back = 0;
    int found_list_node = 0;
    for(int i = 10; i >= 7; i--) {
        if(t1->allocs[i].alloc_size == sizeof(list_node)) found_list_node++;
        if(t1->allocs[i].alloc_size == (sizeof(void*)*default_backing_capacity)) found_back++;
    }
    assert(found_back == 2);
    assert(found_list_node == 2);

    assert(memcmp(node0_arr_before, node0->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node1_arr_before, node1->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node2_arr_before, node2->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node3_arr_after, node3->arr, sizeof(void*)*10) == 0);

    // check the free
    assert(t1->allocs[11].type == FREE_ALLOC);
    assert(!t1->allocs[10].success);
    assert(t1->allocs[9].freed);

    // only 17 allocs
    assert(t1->alloc_size == 12);
    assert(t1->free_calls == 1);

    // free everything
    cleanup(&t);

    return 0;
}

// TEST NOT USED
// nodes:0, idx 1, data_out: NULL
int test_get_1(alloc_tester* t1) {

    // 1 malloc calls succeed
    t1->malloc_calloc_budget = 1;

    // setup
    list_array* arr = ta_create_list_array_empty();

    // run the set
    void* ret = get_list(arr, 1);

    // test post conditions
    assert(ret == NULL);
    assert(arr->head == NULL);
    assert(arr->node_backing_capacity == default_backing_capacity);


    // only 1 alloc1
    assert(t1->alloc_size == 1);

    // free everything
    cleanup(&t);

    return 0;
}

// nodes:1, idx 76, data_out: NULL
int test_get_2(alloc_tester* t1) {

    // 3 malloc calls succeed
    t1->malloc_calloc_budget = 3;

    // setup
    list_array* arr = ta_create_list_array_nodes_1();
    void* node0_arr_before[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                   (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};

    memcpy(arr->head->arr, node0_arr_before, sizeof(void*)*10);

    // run the get
    void* ret = get_list(arr, 76);

    // test post conditions
    assert(ret == NULL);
    assert(arr->node_backing_capacity == default_backing_capacity);
    assert(arr->head->next == NULL);


    assert(memcmp(node0_arr_before, arr->head->arr, sizeof(void*)*10) == 0);


    // only 3 allocs
    assert(t1->alloc_size == 3);

    // free everything
    cleanup(&t);

    return 0;
}

// nodes:3, idx:27, data_out:222
int test_get_3(alloc_tester* t1) {

    // 7 malloc calls succeed
    t1->malloc_calloc_budget = 7;

    // arrays
    void* node0_arr_before[10] = { (void*)3, (void*)1, (void*)4, (void*)1, (void*)5,
                                   (void*)9, (void*)2, (void*)6, (void*)5, (void*)3};
    void* node1_arr_before[10] = { (void*)5, (void*)8, (void*)9, (void*)7, (void*)9,
                                   (void*)3, (void*)2, (void*)3, (void*)8, (void*)4};
    void* node2_arr_before[10] = { (void*)6, (void*)2, (void*)6, (void*)4, (void*)3,
                                   (void*)3, (void*)8, (void*)3, (void*)2, (void*)7};

    // setup
    list_array* arr = ta_create_list_array_nodes_3();
    memcpy(arr->head->arr, node0_arr_before, sizeof(void*)*10);
    memcpy(arr->head->next->arr, node1_arr_before, sizeof(void*)*10);
    memcpy(arr->head->next->next->arr, node2_arr_before, sizeof(void*)*10);

    // run the get
    void* ret = get_list(arr, 27);

    list_node* node0_after = arr->head;
    list_node* node1_after = arr->head->next;
    list_node* node2_after = arr->head->next->next;

    // test post conditions
    assert(ret == (void*)3);
    assert(node0_after);
    assert(node1_after);
    assert(node2_after);

    assert(memcmp(node0_arr_before, node0_after->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node1_arr_before, node1_after->arr, sizeof(void*)*10) == 0);
    assert(memcmp(node2_arr_before, node2_after->arr, sizeof(void*)*10) == 0);


    // only 7 allocs (for list array with 3 nodes)
    assert(t1->alloc_size == 7);

    // free everything
    cleanup(&t);

    return 0;
}

int test_destroy(alloc_tester* t1) {

    t1->malloc_calloc_budget = 7;
    list_array* arr = ta_create_list_array_nodes_3();
    assert(t1->alloc_size == 7);

    void* node0_arr_before[10] = { (void*)1, (void*)2, (void*)3, (void*)4, (void*)5,
                                   (void*)6, (void*)7, (void*)8, (void*)9, (void*)10};
    void* node1_arr_before[10] = { (void*)11, (void*)12, (void*)13, (void*)14, (void*)15,
                                   (void*)16, (void*)17, (void*)18, (void*)19, (void*)20};
    void* node2_arr_before[10] = { (void*)21, (void*)22, (void*)23, (void*)24, (void*)25,
                                   (void*)26, (void*)27, (void*)28, (void*)29, (void*)30};
    void* combined[30];
    memcpy(combined, node0_arr_before, sizeof(void*)*10);
    memcpy(combined + 10, node1_arr_before, sizeof(void*)*10);
    memcpy(combined + 20, node2_arr_before, sizeof(void*)*10);

    bool after_freed[30] = { true, true, true, true, true, true, true, true, true, true,
                             true, true, true, true, true, true, true, true, true, true,
                             true, true, true, true, true, true, true, true, true, true };


    // setup before list
    memcpy(arr->head->arr, node0_arr_before, sizeof(void*)*10);
    memcpy(arr->head->next->arr, node1_arr_before, sizeof(void*)*10);
    memcpy(arr->head->next->next->arr, node2_arr_before, sizeof(void*)*10);

    // do the destroy
    destroy_list_array(arr, fake_free_func);

    // all data pointers freed
    bool freed[30];
    memset(freed, 0, sizeof(bool)*30);
    for(int i = 0; i < 30; i++) {
        for(int j = 0; j < 30; j++) {
            if(fake_free.arr[j] == combined[i]) {
                freed[i] = true;
            }
        }
    }
    assert(memcmp(after_freed, freed, sizeof(bool)*30) == 0);


    // all memory freed
    assert(all_freed(t1));

    cleanup(&t);

    return 0;
}

