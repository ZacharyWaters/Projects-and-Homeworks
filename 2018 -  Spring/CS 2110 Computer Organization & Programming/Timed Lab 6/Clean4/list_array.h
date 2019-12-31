#ifndef _LIST_ARRAY_H_
#define _LIST_ARRAY_H_

/* DO NOT MODIFY THIS FILE
   all of your cod should be in list_array.c */

#define UNUSED_PARAM(x) (void)(x)

#define SET_SUCCESS 0

#define SET_FAILURE 1

typedef void (data_op)(void *);

// definition of each of the nodes in our linked array
typedef struct _list_node {
    // pointer to the next node in the list
    struct _list_node *next;
    // pointer to the backing array for this node
    void **arr;

} list_node;


// definition for the list_array struture
typedef struct _list_array {
    // pointer to the first node in the list
    struct _list_node* head;
    // the backing capacity for any given node
    unsigned int node_backing_capacity;

} list_array;

list_node *create_node(unsigned int);

list_array *create_list(unsigned int);

int set_list(list_array *, unsigned int, void *, void**);

void *get_list(list_array *, unsigned int);

void destroy_list_array(list_array *, data_op);

#endif /* _LIST_ARRAY_H */