/**
 * CS 2110 - Spring 2018 - Homework #10
 *
 * @author YOUR NAME HERE
 *
 * array_list.c: fill in all functions
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "array_list.h"


/**
 * Creates an empty array list
 * - array list's size should be 0, cuz it's empty
 * - allocates a backing array of size INITIAL_CAPACITY
 * - array list's capacity should be the size of the backing array
 * - the backing array should be empty (all entries should be NULL)
 * - if any memory allocation operations fail, return NULL and
 *   clean up any dangling pointers
 *
 * @return the newly created array list
 */
array_list_t* create_array_list(void)
{
  return NULL;
}


/**
 * Destroys an array list
 * - all memory allocated to the array list should be freed
 * - all memory allocated to each data entry in the array should be freed
 * - return non-zero if either parameter is NULL
 *
 * @param arr array list that you're destroying
 * @param free_func function that frees a data pointer
 * @return 0 if operation succeeded, non-zero otherwise
 */
int destroy_array_list(array_list_t* arr, list_op free_func)
{
  UNUSED_PARAM(arr);
  UNUSED_PARAM(free_func);

  return 1;
}

/**
 * Copies the array list (shallow copy)
 * - creates copy of the array_list_t struct
 * - creates copy of the array list's backing array
 * - DOES NOT create a copy of data pointed at by each data pointer
 * - if list_to_copy is NULL, return NULL
 * - if any memory allocation operations fail, return NULL and
 *   clean up any dangling pointers
 * - list_to_copy should not be changed by this function
 *
 * @param list_to_copy array list you're copying
 * @return the shallow copy of the given array list
 */
array_list_t* shallow_copy_array_list(array_list_t *list_to_copy)
{
  UNUSED_PARAM(list_to_copy);

  return NULL;
}


/**
 * Copies the array list (deep copy)
 * - creates copy of the array_list_t struct
 * - creates copy of the array list's backing array
 * - create a copy of data pointed at by each data pointer
 * - return NULL if any parameters are NULL
 * - if any memory allocation operations fail, return NULL and
 *   clean up any dangling pointers
 * - list_to_copy should not be changed by this function
 *
 * @param list_to_copy array list you're copying
 * @param copy_func function that returns a copy of the given data pointer
 * @param free_func function that frees a given data pointer
 * @return the deep copy of the given array list
 */
array_list_t* deep_copy_array_list(array_list_t *list_to_copy, list_copy copy_func, list_op free_func)
{
  UNUSED_PARAM(list_to_copy);
  UNUSED_PARAM(copy_func);
  UNUSED_PARAM(free_func);

  return NULL;
}

/**
 * Adds the given data at the given index
 * - if the index is negative, wrap around (ex: -1 becomes size - 1)
 * - if size + index < 0, keep wrapping around until a positive in bounds index is reached
 *   (ex: -5 with size 4 becomes index 4)
 * - adding in the middle of the array list (index < size)
 *     - increment size
 *     - if necessary, resize the backing array as described below
 *     - shift all elements with indices [index, size - 1] down by one
 *     - the indices of the shifted elements should now be [index + 1, size])
 *     - put data into index in the array list
 * - adding at the end of the array list (index == size)
 *     - increment size
 *     - if necessary, resize the backing array as described below
 *     - put data into the index in the array list
 * - adding after the end of the array list (index > size)
 *     - index now represents the last element in the array list, so size = index + 1
 *     - if necessary, resize the backing array as described below
 *     - put data into the index in the array list
 *     - Note: this operation will result in 1 or more empty slots from [old size, index - 1]
 * - if any of these operations would increase size to size >= capacity, you must
 *   increase the size of the backing array by GROWTH_FACTOR until size < capacity.
 *   Only then can you insert into the backing array
 * - if any memory allocation operations fail, do not change the array list and return non-zero
 *     - the array list should be in the same state it was in before add_to_array_list was called
 * - if arr is NULL, return non-zero
 * - data can be NULL
 *
 * @param arr array list you're adding to
 * @param index you're adding data to
 * @param data data you're putting into the array list
 * @return 0 if add operation succeeded, non-zero otherwise
 */
int add_to_array_list(array_list_t* arr, int index, void* data)
{
  UNUSED_PARAM(arr);
  UNUSED_PARAM(index);
  UNUSED_PARAM(data);

  return 1;
}


/**
 * Removes the data at the given index and returns it to the user
 * - if the index is negative, wrap around (ex: -1 becomes size - 1)
 * - if size + index < 0, keep wrapping around until a positive in bounds index is reached
 *   (ex: -5 with size 4 becomes index 4)
 * - removing from the middle of the array list (index < size - 1)
 *     - save the data pointer at index
 *     - shift all elements with indices [index + 1, size - 1] up by one
 *     - the indices of the shifted elements should now be [index, size - 2])
 *     - decrement the size and resize backing array if necessary
 *     - return the saved data pointer
 * - removing from the end of the array list (index == size - 1)
 *     - save the data pointer at index
 *     - decrement the size and resize backing array if necessary
 *     - return the saved data pointer
 * - removing from after the end of the array list (index > size - 1)
 *     - there's nothing to remove, so fail by returning NULL
 * - if any of these operations would decrease size to size < capacity / GROWTH_FACTOR ^ 2, you must
 *   shrink the backing array by GROWTH_FACTOR so that capacity = previous capacity / GROWTH_FACTOR
 * - if arr or data_out are NULL, return non-zero
 *
 * @param arr array list you're removing from
 * @param index you're removing from array list
 * @param data_out pointer where you store data pointer removed from array list
 * @return 0 if remove operation succeeded, non-zero otherwise
 */
int remove_from_array_list(array_list_t* arr, int index, void** data_out)
{
  UNUSED_PARAM(arr);
  UNUSED_PARAM(index);
  UNUSED_PARAM(data_out);

  return 1;
}


/**
 * Checks if array list contains data
 * - checks if each data pointer from indices [0, size - 1]
 *   equal the passed in data using eq_func
 * - terminate search on first match
 * - if data is found, return it through return_found
 * - if any parameters besides data are NULL, return false
 *
 * @param arr array list you're searching
 * @param data the data you're looking for
 * @param eq_func function used to check if two data pointers are equal  (returns non-zero if equal)
 * @param return_found pointer through which you return found data if found
 * @return true if array list contains data
 */
bool array_list_contains(array_list_t* arr, void* data, list_eq eq_func, void** return_found)
{
  UNUSED_PARAM(arr);
  UNUSED_PARAM(data);
  UNUSED_PARAM(eq_func);
  UNUSED_PARAM(return_found);

  return false;
}

/**
 * Trims backing array's size so it's the same as the size of the array
 * - if any memory allocation operations fail, do not change the array list and return false
 * - the array list should be in the same state it was in before add_at_index was called
 * - if arr is NULL, return non-zero
 *
 * @param arr array list who's backing array you're trimming
 * @return 0 if remove operation succeeded, non-zero otherwise
 */
int trim_to_size(array_list_t* arr)
{
  UNUSED_PARAM(arr);

  return false;
}

