/**
 * CS 2110 - Spring 2018 - Homework #10
 *
 * @author YOUR NAME HERE
 *
 * circ_list.c: fill in all functions
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "circ_list.h"


/**
 * Creates a linked list node
 * - node should contain the data that's passed in
 * - next and prev pointers should be NULL
 *   (this node should not be part of any list)
 * - if any memory allocation operations fail, return NULL and
 *   clean up any dangling pointers
 * - data can be NULL
 *
 * @param data you're putting into this node
 * @return the newly created linked list node
 */
static l_node *create_node(void *data)
{
  UNUSED_PARAM(data);
  return NULL;
}


/**
 * Creates an empty linked list
 * - linked list's size should be 0 (cuz it's empty)
 * - linked list's head should be NULL (cuz it's empty)
 * - if any memory allocation operations fail, return NULL and
 *   clean up any dangling pointers
 *
 * @return the newly created linked list
 */
circ_list *create_linked_list(void)
{
  UNUSED_PARAM(create_node);
  return NULL;
}

/**
 * Destroys a linked list
 * - all memory allocated to the linked list should be freed
 * - all memory allocated to each data entry in the linked list should be freed
 * - return non-zero if either parameters is NULL
 *
 * @param arr linked list that you're destroying
 * @param free_func function that frees a data pointer
 * @return 0 if operation succeeded, non-zero otherwise
 */
int destroy_linked_list(circ_list* arr, list_op free_func)
{
  UNUSED_PARAM(arr);
  UNUSED_PARAM(free_func);

  return 1;
}


/**
 * Copies the linked list (shallow copy)
 * - creates a copy of the circ_list struct
 * - creates a copy of each of the linked list's nodes
 * - DOES NOT create a copy of the data pointed at by each data pointer
 * - if any memory allocation operations fail, return NULL and
 *   clean up any dangling pointers
 * - list_to_copy should not be changed by this function
 * - if list_to_copy is NULL, return NULL
 *
 * @param list_to_copy linked list you're copying
 * @return the shallow copy of the given linked list
 */
circ_list *shallow_copy_linked_list(circ_list *list_to_copy)
{
  UNUSED_PARAM(list_to_copy);

  return NULL;
}


/**
 * Creates a copy of the linked list (deep copy)
 * - creates a copy of the circ_list struct
 * - creates a copy of each of the linked list's nodes
 * - creates a copy of the data pointed at by each data pointer
 * - if any memory allocation operations fail, return NULL and
 *   clean up any dangling pointers
 * - list_to_copy should not be changed by this function
 * - if any parameters are NULL, return NULL
 *
 * @param list_to_copy linked list you're copying
 * @param copy_func function that returns a copy of the given data pointer
 * @param free_func function that frees a given data pointer
 * @return the deep copy of the given linked list
 */
circ_list *deep_copy_linked_list(circ_list *list_to_copy, list_copy copy_func, list_op free_func)
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
 * - if the index is greater than size - 1, wrap around (ex: size becomes 0)
 * - if index - size > size, keep wrapping around until a positive in bounds index is reached
 *   (ex: idx: 12, size:5 becomes idx:2)
 * - adding to an empty linked list
 *     - increment size
 *     - create a new node with the given data
 *     - set new node as head
 *     - set new nodes next and prev pointers to this node
 * - adding to the middle of the linked list
 *     - increment size
 *     - create a new node with the given data
 *     - traverse linked list to the (index - 1) node
 *     - insert new node between the (index - 1) node and (index) node
 * - if any memory allocation operations fail, do not change the linked list and return non-zero
 *     - the linked list should be in the same state it was in before add_to_linked_list was called
 * - if list is NULL, return non-zero
 * - data can be NULL
 *
 * @param list linked list that you're adding to
 * @param index you're adding data to
 * @param data you're putting into the linked list
 * @return 0 if add operation succeeded, non-zero otherwise
 */
int add_to_linked_list(circ_list *list, int index, void *data)
{
  UNUSED_PARAM(list);
  UNUSED_PARAM(index);
  UNUSED_PARAM(data);

  return false;
}


/**
 * Gets the data pointer at the given index
 * - if the index is negative, wrap around (ex: -1 becomes size - 1)
 * - if size + index < 0, keep wrapping around until a positive in bounds index is reached
 *   (ex: -5 with size 4 becomes index 4)
 * - if the index is greater than size - 1, wrap around (ex: size becomes 0)
 * - if index - size > size, keep wrapping around until a positive in bounds index is reached
 *   (ex: idx: 12, size:5 becomes idx:2)
 * - if list or data_out are NULL, return non-zero
 *
 * @param list linked list you're getting data from
 * @param index you're getting data at
 * @param data_out pointer where you store data pointer removed from linked list
 * @return 0 if get operation succeeded, non-zero otherwise
 */
int get_from_linked_list(circ_list *list, int index, void** data_out)
{
  UNUSED_PARAM(list);
  UNUSED_PARAM(index);
  UNUSED_PARAM(data_out);

  return 1;
}


/**
 * Removes the data at the given index and returns it to the user
 * - if the index is negative, wrap around (ex: -1 becomes size - 1)
 * - if size + index < 0, keep wrapping around until a positive in bounds index is reached
 *   (ex: -5 with size 4 becomes index 4)
 * - if the index is greater than size - 1, wrap around (ex: size becomes 0)
 * - if index - size > size, keep wrapping around until a positive in bounds index is reached
 *   (ex: idx: 12, size:5 becomes idx:2)
 * - removing from a size = 1 linked list
 *     - decrement size
 *     - save the data pointer
 *     - free the node
 *     - reset the head pointer to NULL
 * - removing from the middle of a linked list
 *     - decrement size
 *     - save the data pointer
 *     - remove this node from the linked list
 *         - set index+1's prev to index-1
 *         - set index-1's next to index+1
 *     - free the node
 * - removing from the head of the linked list (index 0)
 *     - same as "removing from the middle of a linked list"
 *     - head is changed to the node that used to be at index 1
 *     - if there was no node at index 1, set head to NULL
 * - if list or data_out are NULL, return non-zero
 *
 * @param list linked list you're removing from
 * @param index you're removing from linked list
 * @param data_out pointer where you store data pointer removed from linked list
 * @return 0 if remove operation succeeded, non-zero otherwise
 */
int remove_from_linked_list(circ_list *list, int index, void** data_out)
{
  UNUSED_PARAM(list);
  UNUSED_PARAM(index);
  UNUSED_PARAM(data_out);

  return 1;
}


/**
 * Checks if linked list contains data
 * - checks if each data pointer in each node equals the passed in data using eq_func
 * - terminate search on first match
 * - if data is found, return it through return_found
 * - if any parameter besides data is NULL, return false
 *
 * @param list linked list you're searching
 * @param data the data you're looking for
 * @param eq_func function used to check if two data pointers are equal (returns non-zero if equal)
 * @param return_found pointer through which you return found data if found
 * @return true if linked list contains data
 */
bool linked_list_contains(circ_list *list, void *data, list_eq eq_func, void **return_found)
{
  UNUSED_PARAM(list);
  UNUSED_PARAM(data);
  UNUSED_PARAM(eq_func);
  UNUSED_PARAM(return_found);

  return false;
}


/**
 * Zips two linked lists together
 * Example:
 *   a0 -> a1 -> a2 -> a3
 *   b0 -> b1 -> b2 -> b3
 * zip(a,b) becomes:
 *   a0 -> b0 -> a1 -> b1 -> a2 -> b2 -> a3 -> b3
 * - list_1 and list_2 should both be modified to point to the zipped list
 * - you can assume that list_1 and list_2 are equal in size
 * - if either parameter is NULL, return non-zero
 *
 * @param list_1 1st linked list you're zipping
 * @param list_2 2nd linked list you're zipping
 * @return 0 if zip operation succeeded, non-zero otherwise
 */
int zip(circ_list *list_1, circ_list *list_2)
{
  UNUSED_PARAM(list_1);
  UNUSED_PARAM(list_2);

  return 1;
}

