#include "circ_list.h"
#include "array_list.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void nopFree(void *ptr){
    (void)(ptr);
    return;
}
int equals(const void* data_a, const void* data_b){
    //Only works with ints, please don't use elsewhere
    const int* a = data_a;
    const int* b = data_b;
    if(*a == *b){
        return 1;
    }
    return 0;
}
// normal create_list with no malloc failures
int test_create_empty_list() {
    // test create_list
    array_list_t* list = create_array_list();
    assert(list != NULL);
    assert(list->capacity == INITIAL_CAPACITY);
    assert(list->size == 0);

    free(list->entries);
    free(list);

    return 0;
}
int test_destroy_empty_list(){
    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Destroy the list
    assert(destroy_array_list(list, &free) == 0);

    return 0;
}
int test_destroy_list_one(){
    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Add a single item
    int *a = malloc(sizeof(int));
    *a = 2;
    assert(add_to_array_list(list, 0, a) == 0);

    //Destroy the list
    assert(destroy_array_list(list, &free) == 0);

    return 0;
}
int test_add_then_get_one(){
    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Add a single item
    int *a = malloc(sizeof(int));
    *a = 2;
    assert(add_to_array_list(list, 0, a) == 0);

    //Get item at 0
    assert(*a == *((int*)(list->entries[0])));

    //Destroy the list
    assert(destroy_array_list(list, &free) == 0);

    return 0;
}
int test_add_outside_capacity(){
    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Add a single item
    int *a = malloc(sizeof(int));
    *a = 42;
    assert(add_to_array_list(list, 11, a) == 0);

    //Get item at 11
    assert(*a == *((int*)(list->entries[11])));

    //Check for correct new Capacity
    assert(list->capacity == 20);

    //Destroy the list
    assert(destroy_array_list(list, &free) == 0);

    return 0;
}
int test_remove_shrink(){
    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Add two items
    int *a = malloc(sizeof(int));
    *a = 42;
    assert(add_to_array_list(list, 0, a) == 0);
    int *b = malloc(sizeof(int));
    *b = 69;
    assert(add_to_array_list(list, 1, b) == 0);

    //Remove single item
    void** data_out = malloc(sizeof(void*));
    assert(remove_from_array_list(list, 1, data_out) == 0);

    //Destroy the list
    assert(destroy_array_list(list, &free) == 0);
    free(*data_out);
    free(data_out);

    return 0;
}
int test_add_4_and_check(){
    // Create stuff to add
    int* a = malloc(sizeof(int));
    int* b = malloc(sizeof(int));
    int* c = malloc(sizeof(int));
    int* d = malloc(sizeof(int));
    *a = 0;
    *b = 1;
    *c = 2;
    *d = 3;

    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Add all of the items
    assert(add_to_array_list(list, 0, a) == 0);
    assert(add_to_array_list(list, 1, b) == 0);
    assert(add_to_array_list(list, 2, c) == 0);
    assert(add_to_array_list(list, 3, d) == 0);

    //Check if they are where they should be
    assert(*a == *((int*)(list->entries[0])));
    assert(*b == *((int*)(list->entries[1])));
    assert(*c == *((int*)(list->entries[2])));
    assert(*d == *((int*)(list->entries[3])));

    //Destroy the list
    assert(destroy_array_list(list, &free) == 0);
    return 0;
}
int add_4_remove_ind_0(){
    // Create stuff to add
    int* a = malloc(sizeof(int));
    int* b = malloc(sizeof(int));
    int* c = malloc(sizeof(int));
    int* d = malloc(sizeof(int));
    *a = 0;
    *b = 1;
    *c = 2;
    *d = 3;

    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Add all of the items
    assert(add_to_array_list(list, 0, a) == 0);
    assert(add_to_array_list(list, 1, b) == 0);
    assert(add_to_array_list(list, 2, c) == 0);
    assert(add_to_array_list(list, 3, d) == 0);

    //Remove single item
    void** data_out = malloc(sizeof(void*));
    assert(remove_from_array_list(list, 0, data_out) == 0);
    free(*data_out);
    free(data_out);

    //Check Remaining items
    assert(*b == *((int*)(list->entries[0])));
    assert(*c == *((int*)(list->entries[1])));
    assert(*d == *((int*)(list->entries[2])));
    assert(NULL == list->entries[3]);

    //Destroy the list
    assert(destroy_array_list(list, &free) == 0);
    return 0;
}
int test_add_middle(){
    // Create stuff to add
    int* a = malloc(sizeof(int));
    int* b = malloc(sizeof(int));
    int* c = malloc(sizeof(int));
    *a = 0;
    *b = 1;
    *c = 2;

    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Add all of the items
    assert(add_to_array_list(list, 0, a) == 0);
    assert(add_to_array_list(list, 1, b) == 0);
    assert(add_to_array_list(list, 0, c) == 0);

    //Check items
    assert(*c == *((int*)(list->entries[0])));
    assert(*a == *((int*)(list->entries[1])));
    assert(*b == *((int*)(list->entries[2])));

    //Destroy the list
    assert(destroy_array_list(list, &free) == 0);
    return 0;
}
int test_shallow_copy(){
    // Create stuff to add
    int* a = malloc(sizeof(int));
    int* b = malloc(sizeof(int));
    int* c = malloc(sizeof(int));
    *a = 0;
    *b = 1;
    *c = 2;

    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Add all of the items
    assert(add_to_array_list(list, 0, a) == 0);
    assert(add_to_array_list(list, 1, b) == 0);
    assert(add_to_array_list(list, 2, c) == 0);

    //Shallow Copy
    array_list_t *list2 = shallow_copy_array_list(list);

    //Check items
    assert(*a == *((int*)(list2->entries[0])));
    assert(*b == *((int*)(list2->entries[1])));
    assert(*c == *((int*)(list2->entries[2])));

    //Free lists
    assert(destroy_array_list(list2, &free) == 0);
    assert(destroy_array_list(list, &nopFree) == 0);
    return 0;
}
int test_trim_list(){
    // Create stuff to add
    int* a = malloc(sizeof(int));
    int* b = malloc(sizeof(int));
    int* c = malloc(sizeof(int));
    *a = 0;
    *b = 1;
    *c = 2;

    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Add all of the items
    assert(add_to_array_list(list, 0, a) == 0);
    assert(add_to_array_list(list, 1, b) == 0);
    assert(add_to_array_list(list, 2, c) == 0);

    //Trim
    assert(trim_to_size(list) == 0);
    assert(list->capacity == list->size);

    //Check items
    assert(*a == *((int*)(list->entries[0])));
    assert(*b == *((int*)(list->entries[1])));
    assert(*c == *((int*)(list->entries[2])));

    //Free lists
    assert(destroy_array_list(list, &free) == 0);
    return 0;
}
int test_list_contains(){
    // Create stuff to add
    int* a = malloc(sizeof(int));
    *a = 0;

    //Create a new list
    array_list_t *list = create_array_list();
    assert(list != NULL);

    //Add item
    assert(add_to_array_list(list, 0, a) == 0);

    //Contains
    void** return_found = malloc(sizeof(void*));
    assert(array_list_contains(list, a, &equals ,return_found) != 0);
    free(return_found);

    //Free lists
    assert(destroy_array_list(list, &free) == 0);
    return 0;
}
int main() {
    printf("Test: Create Empty List - ");
    test_create_empty_list();
    printf("Passed\n");

    printf("Test: Destroy Empty List - ");
    test_destroy_empty_list();
    printf("Passed\n");

    printf("Test: Destroy List One Item - ");
    test_destroy_list_one();
    printf("Passed\n");

    printf("Test: Add Then Get - ");
    test_add_then_get_one();
    printf("Passed\n");

    printf("Test: Add Outside Capacity - ");
    test_add_outside_capacity();
    printf("Passed\n");

    printf("Test: Remove Shrink Capacity - ");
    test_remove_shrink();
    printf("Passed\n");

    printf("Test: Add Multiple - ");
    test_add_4_and_check();
    printf("Passed\n");

    printf("Test: Add Remove Ind 0 - ");
    add_4_remove_ind_0();
    printf("Passed\n");

    printf("Test: Add Middle - ");
    test_add_middle();
    printf("Passed\n");

    printf("Test: Shallow Copy - ");
    test_shallow_copy();
    printf("Passed\n");

    printf("Test: Trim - ");
    test_trim_list();
    printf("Passed\n");

    //printf("Test: Contains - ");
    //test_list_contains();
    //printf("Passed\n");

    return 0;
}
