#include "list_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Creates a list_node and returns a pointer to the newly created node.
 * Make sure to clear out the memory in the backing array before returning the
 * list_node and set the next pointer to NULL.
 *
 * Make sure to clean up any partially allocated stuctures if malloc fails!
 *
 * All memory allocations should be optimal, do not allocate more blocks than
 * you need or larger blocks thatn you need to.
 *
 * @param backing_capacity backing capacity for the array the node has.
 * @returns a pointer to the newly created node or NULL if malloc fails
 */
list_node *create_node(unsigned int backing_capacity) {

    list_node* newNode = malloc(sizeof(list_node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode -> arr = calloc(backing_capacity,sizeof(void*));
    if(newNode -> arr == NULL){			//B
	free(newNode);	
	return NULL;	
	                           }		//B
    newNode->next = NULL;
    return newNode;
}

/*
 * Creates a list_array with one node (pointed to by head).
 * Make sure to clean up any partially allocated structures if malloc fails!
 *
 * All memory allocations should be optimal, do not allocate more blocks than you
 * need or larger blocks thatn you need to.
 *
 * @param backin_capacity backing capacity for all of the nodes in the list_array
 * @returns a pointer to the newly created list_array
*/
list_array *create_list(unsigned int backing_capacity) {

    list_array* newList = malloc(sizeof(list_array));
    if (newList == NULL) {
        return NULL;
    }
    list_node* newNode = create_node(backing_capacity);
	if(newNode==NULL){
	free(newList);
	return NULL;
			}
    newList->head = newNode;
    newList->node_backing_capacity = backing_capacity;
    return newList;
	
}


/*
 * Sets data at the specified index. If the index is out of bounds of any of the
 * nodes, add more nodes to the list_array until the index is in bounds.
 *
 * If the list is NULL or existing_data is NULL return SET_FAILURE
 *
 * If malloc fails while you are adding nodes to the list_array before you have
 * enough nodes to reach the desired index, you do not have to remove and
 * deallocate the newly added nodes (they should remain on the list), return
 * SET_FAILURE.
 *
 * If you were able to successfully get to the specified index return the data
 * at that index through existing_data parameter, set the data at that index
 * to the data passed in, and return SET_SUCCESS (you only have to set the
 * existing_data pointer on success).
 *
 * All memory allocations should be optimal, do not allocate more blocks than you
 * need or larger blocks thatn you need to.
 *
 * HINT:
 * if the index is greater than the backing_capacity, you subtract the
 * backing_capacity from index and check the next node (add a node if it
 * doesn't exist) and repeat until index is in the range of backing_capacity
 * and set the appropriate element in that node's array.
 *
 * @param list the list to add to
 * @param index the index to set in the list_array
 * @param data the data to put into to the list (NULL is valid data)
 * @param existing_data a double pointer used to return the data that was in the
 * specified index before it is overwritten
 * @returns SET_SUCCESS on success (successfully set the index), and SET_FAILURE
 * on a failure (list is NULL, exisiting_data is NULL, or malloc fails)
 */
int set_list(list_array *list, unsigned int index, void * data, void** exisiting_data) {
	if( (list==NULL) || (exisiting_data==NULL)){
		return SET_FAILURE;
						}
	list_node* current = list->head;
	unsigned int nodeamount = list->node_backing_capacity;
	
	while(index>nodeamount && current != NULL)	{
		current = current->next;
		index = index-nodeamount;
				}
		
	if(index<nodeamount)		{
		exisiting_data=current->arr[index];
		current->arr[index]=data;
		return SET_SUCCESS;
					}
    UNUSED_PARAM(list);
    UNUSED_PARAM(index);
    UNUSED_PARAM(data);
    UNUSED_PARAM(exisiting_data);
    return SET_SUCCESS;
}

/*
 * Returns data at the specified index or NULL if the index is out bounds
 * of the list_array.
 *
 * @param list the list
 * @param index the index to get data from
 * @returns the data at the index or NULL if the index is out of bounds
 */
void *get_list(list_array *list, unsigned int index) {
    UNUSED_PARAM(list);
    UNUSED_PARAM(index);
    return NULL;
}

/*
 * Destroys and frees all data associated with the list_array
 *
 * @param list the list_array to free
 * @param free_func the function used to free user data
 */
void destroy_list_array(list_array *list, data_op free_func) {
    UNUSED_PARAM(list);
    UNUSED_PARAM(free_func);
}
