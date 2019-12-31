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
TESTER COMMANDS:
patrick's
 gcc -Wl,--wrap=malloc,--wrap=free,--wrap=calloc,--wrap=realloc -D_POSIX_SOURCE -Wall -Werror -pedantic -std=c99 -g array_list.c grader.c wrap.c -o tests
then type
./tests
or 
valgrind ./tests

Other tester:
make run-valgrind

general purpose
make
*/


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
	array_list_t* list = (array_list_t*)malloc(sizeof(array_list_t));
	if(list == NULL){				//A
	return NULL;	
	                }				//A
	
	list -> entries = calloc(INITIAL_CAPACITY,sizeof(void*));

	if(list -> entries == NULL){			//B
	free(list);	
	return NULL;	
	                           }			//B
	list-> size = 0;
	list-> capacity = INITIAL_CAPACITY;	
	return list;
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
	if((arr == NULL)||(free_func==NULL)){		//A
			return 1;
					    }		//A
	int capacity = arr-> capacity;
	for(int i=0; i<capacity;i++){			//B
		free_func(*(arr->entries+i));
	}						//B
	free(arr->entries);
	free(arr);
	return 0;
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
	if(list_to_copy==NULL){					//A
		return NULL;
			      }					//A
	array_list_t* copy = calloc(1,sizeof(array_list_t));
	if(copy==NULL){						//B
		return NULL;
			}					//B
	void** tempentries = malloc((list_to_copy->capacity)*sizeof(void*));
	if(tempentries == NULL){				//C
	free(copy);	
	return NULL;	
	                           }				//C
	copy->entries=tempentries;
	copy->size=list_to_copy->size;
	copy->capacity=list_to_copy->capacity;
	
	for(int i=0;i<(copy->capacity);i++)	{		//D
		copy->entries[i]=list_to_copy->entries[i];
							}	//D

	return copy;
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
{//START
	//if they fail to provide an array
	if(arr==NULL)	{//A
		return 1;
			}//A

	int size = arr->size;
	int backupsize = 0;
	int capacity = arr->capacity;
	int backupcapacity = 0;
	int indexsize = index +1;
	//[(INDEX),NULL,CAPACITY]	
	if(size==0 && index <= 0 && capacity>=1){//B
		arr->size=1;
		arr->entries[0]=data;
		return 0;
						}//B
	//[(A,B,C,D)INDEX, NULL, NULL, CAPACITY]
	//if you are adding a value that outside of size but wont extend capacity
	if( (indexsize>size) && ((size+1)<=capacity)&&(index<capacity))	{//C
		arr->size=size+(indexsize-size);
		arr->entries[index]=data;
		return 0;
							}//C
	//[(INDEX),A->,B->,C->),NULL,NULL,CAPACITY]
	//if you are adding a value to the inside of size, causing a shift, 
	// but wont cause a change in capacity
	if( (indexsize<size) && ((size+1)<=capacity))	{//D
		arr->size=size+1;
		for(int i=(size-1);i>index-1;i--){//D.1
			arr->entries[i+1]=arr->entries[i];
						}//D.1
		arr->entries[index]=data;
		return 0;
							}//D
	//[(A,B,C,D,E,F,G)]INDEX<SIZE CAPACITY=SIZE
	//if you are adding a value at the outside of size and capacity, when 
	//size and capacity are equal.a shift will be required. but you will
	//make a new array.
	if((index<size)&&(size==capacity))	{//E
		backupsize = arr->size;
		backupcapacity = arr->capacity;
		arr->size=size+1;
		capacity = capacity*GROWTH_FACTOR;
		arr->capacity=capacity;
		//MAKING THE NEW ARRAY
		void** arrE = realloc(arr->entries,(arr->capacity) * sizeof(void*) );
		if (arrE == NULL)	{//E.1
			arr->size=backupsize;
			arr->capacity=backupcapacity;
                	return 1;
            				}//E.1
            	arr->entries = arrE;
		//FILLING THE NEW SPACE WITH NULLS
		for(int i=size;i<capacity;i++){//E.2
			arr->entries[i]=NULL;
						}//E.2
		//SHIFTING THE VALUES
		for(int i=(size-1);i>index-1;i--){//E.3
			arr->entries[i+1]=arr->entries[i];
						 }//E.3
		//PUTTING THE DATA IN
		arr->entries[index]=data;		
		return 0;
						}//E
	//[(A,B,C,D,E,F,G)]INDEX=SIZE CAPACITY=SIZE
	//if you are adding a value at the outside of size and capacity, when 
	//size and capacity are equal.a shift will not be required. but you will
	//make a new array.
	if((index==size) && (size==capacity))	{//F
		backupsize = arr->size;
		backupcapacity = arr->capacity;
		arr->size=size+1;
		capacity = capacity*GROWTH_FACTOR;
		arr->capacity=capacity;
		//MAKING THE NEW ARRAY
		void** arrE = realloc(arr->entries,(arr->capacity) * sizeof(void*) );
		if (arrE == NULL)	{//F.1
			arr->size=backupsize;
			arr->capacity=backupcapacity;
                	return 1;
            				}//F.1
            	arr->entries = arrE;
		//FILLING THE NEW SPACE WITH NULLS
		for(int i=size;i<capacity;i++){//F.2
			arr->entries[i]=NULL;
						}//F.2
		//PUTTING THE DATA IN
		arr->entries[index]=data;		
		return 0;
						}//F
	//[(A,B,C,D,E,F,G)]INDEX>SIZE CAPACITY=SIZE
	//if you are adding a value at the outside of size and capacity, when 
	//size and capacity are equal.a shift will not be required. but you will
	//make a new array.
	if((index>size)&&(size==capacity))	{//G
		backupsize = arr->size;
		backupcapacity = arr->capacity;
		arr->size=size+(indexsize-size);	
		while(capacity<=index)	{//G.1
		capacity = capacity*GROWTH_FACTOR;
					}//G.1
		arr->capacity=capacity;
		//MAKING THE NEW ARRAY
		void** arrE = realloc(arr->entries,(arr->capacity) * sizeof(void*) );
		if (arrE == NULL)	{//G.2
			arr->size=backupsize;
			arr->capacity=backupcapacity;
                	return 1;
            				}//G.2
            	arr->entries = arrE;
		//FILLING THE NEW SPACE WITH NULLS
		for(int i=size;i<capacity;i++){//G.3
			arr->entries[i]=NULL;
						}//G.3
		//PUTTING THE DATA IN
		arr->entries[index]=data;		
		return 0;
						}//G
	//[(A),NULL,NULL]INDEX>SIZE INDEX>CAPACITY
	//if you are adding a value at the outside of size and capacity, 
	//a shift will not be required. but you will make a new array.
	if((index>size-1)&&(index>capacity))	{//H
		backupsize = arr->size;
		backupcapacity = arr->capacity;
		arr->size=size+(indexsize-size);	
		while(capacity<=index)	{//H.1
		capacity = capacity*GROWTH_FACTOR;
					}//H.1
		arr->capacity=capacity;
		//MAKING THE NEW ARRAY
		void** arrE = realloc(arr->entries,(arr->capacity) * sizeof(void*) );
		if (arrE == NULL)	{//H.2
			arr->size=backupsize;
			arr->capacity=backupcapacity;
                	return 1;
            				}//H.2
            	arr->entries = arrE;
		//FILLING THE NEW SPACE WITH NULLS
		for(int i=size;i<capacity;i++){//H.3
			arr->entries[i]=NULL;
						}//H.3
		//PUTTING THE DATA IN
		arr->entries[index]=data;		
		return 0;
						}//H

	return 1;
}//END


/**
{										       //START
    if (arr == NULL)	 {							           //A
        return 1;
   			 }							           //A
	int size = arr->size;                                                             
	int capacity = arr->capacity;
	int indexsize = index + 1;
    if (size == 0 && index <= 0) {							   //B
        arr->size = 1;
        arr->entries[0] = data;
        return 0;
    				} 							   //B
    while (index < 0) {							        	   //D
            index = size + index;
        }										   //D

    if (indexsize > size) {								   //E
        if (index < capacity) {								   //F
            arr->size = indexsize;
            arr->entries[index] = data;
            return 0;
	} 										   //F
	else {										   //G
            while (arr->capacity < indexsize) {						   //H
                arr->capacity = arr->capacity * GROWTH_FACTOR;
            }										   //H
            void** newarr = realloc(arr->entries,(arr->capacity) * sizeof(void*) );
            if (newarr == NULL) { 							   //I
                return 1;
            }										   //I
            arr->entries = newarr;
            for(int i = size; i < capacity; i++)  {					   //J
                arr->entries[i] = NULL;
            }										   //J
            arr->entries[index] = data;
            arr->size = indexsize;
            return 0;
        }										   //G
    }											   //F
    if (size + 1 >= capacity) {							           //K
        void** temparr = realloc(arr->entries, sizeof(void*) * (capacity * GROWTH_FACTOR));
        if (temparr == NULL) {								   //L
            return 1;
        }										   //L
        arr->entries = temparr;
        arr->capacity = capacity * GROWTH_FACTOR;
        for(int i = size; i < capacity; i++) {						   //M
            arr->entries[i] = NULL;
        }										   //M
    }											   //K
    for (int i = size; index < i; i--) {						   //N
        arr->entries[i] = arr->entries[i-1];
    }											   //N
    arr->entries[index] = data;
    arr->size++;
    return 0;
}											 //END
*/




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
{//START
	//if the inputs are invalid	
	if((arr==NULL) || (data_out==NULL))	{//A
		return 1;
						}//A
	//if the array is of size zero
	if(arr->size==0)	{//B
		*data_out=NULL;
		return 1;
				}//B
	//if the index is out of range
	if(index > ((arr->size)-1))	{//C
		*data_out=NULL;
		return 1;
					}//C
	int size = arr->size;
	//if the index is less than the size
	if(index<size)	{//D
		*data_out=arr->entries[index];
		for(int i=index;i<size-1;i++)	{//D.1
			arr->entries[i]=arr->entries[i+1];						
						}//D.1
		arr->entries[size-1]=NULL;
		arr->size=size-1;
		//Checks if the backing array needs to be shrunk
		if(size<(arr->capacity/(GROWTH_FACTOR*GROWTH_FACTOR))){//D.2
			int backupcapacity = arr->capacity;
			int backupsize = arr->size;
			arr->capacity=(arr->capacity)/(GROWTH_FACTOR);
			//MAKING THE NEW ARRAY
			void** arrD = realloc(arr->entries,(arr->capacity) * sizeof(void*) );
			if (arrD == NULL)	{//D.3
				arr->size=backupsize;
				arr->capacity=backupcapacity;
                		return 1;
            					}//D.3
            		arr->entries = arrD;
			//FILLING THE NEW SPACE WITH NULLS
			for(int i=size;i<arr->capacity;i++)	{//4.3
				arr->entries[i]=NULL;
						}//D.4
									}//D.2
		return 0;
			}//D
return 1;
}//END

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
	if(arr==NULL)	{				//A
		return 1;	
			}				//A
	int size = arr->size;
	void** temp = realloc(arr->entries,size * sizeof(void*));
	if(temp==NULL)	{				//B
		return 1;	
			}				//B
	arr->capacity=size;
	arr->entries=temp;	
	return 0;
}


