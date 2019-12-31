/*
 * CS 2110 Spring 2018
 * Author: Zach Waters
 */

/* we need this for uintptr_t */
#include <stdint.h>
/* we need this for memcpy/memset */
#include <string.h>
/* we need this to print out stuff*/
#include <stdio.h>
/* we need this for my_sbrk */
#include "my_sbrk.h"
/* we need this for the metadata_t struct and my_malloc_err enum definitions */
#include "my_malloc.h"

/* Our freelist structure - our freelist is represented as two doubly linked lists
 * the address_list orders the free blocks in ascending address
 * the size_list orders the free blocks by size
 */

metadata_t *address_list;
metadata_t *size_list;

/* Set on every invocation of my_malloc()/my_free()/my_realloc()/
 * my_calloc() to indicate success or the type of failure. See
 * the definition of the my_malloc_err enum in my_malloc.h for details.
 * Similar to errno(3).
 */
enum my_malloc_err my_malloc_errno;

//PROTOTYPES:
void removefromaddress(metadata_t* current);
void removefromsize(metadata_t* current);
void* split (int wantsize,metadata_t* current);
void addtosize(metadata_t* current);
void addtoaddress(metadata_t* current);

/* MALLOC
 * See my_malloc.h for documentation
 */
void *my_malloc(size_t size) {//START
	UNUSED_PARAMETER(size);
	//if size==0, throw error
	if(size==0)		{//A
		my_malloc_errno = NO_ERROR;
		return NULL;
				}//A
				unsigned int realsize = TOTAL_METADATA_SIZE+size;
	//if size>SBK_SIZE, throw error
	if(realsize>SBRK_SIZE)	{//B
		my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
		return NULL;
				}//B
	if(size_list==NULL)	{//C
		metadata_t* temp = my_sbrk(SBRK_SIZE);
		if(temp==NULL)	{//C.1
			my_malloc_errno = OUT_OF_MEMORY;
			return NULL;//C.1
		}
		size_list=temp;
		address_list=size_list;
		size_list->prev_size = NULL;
		size_list->prev_addr = NULL;
		size_list->next_size = NULL;
		size_list->next_addr = NULL;
		size_list->size = SBRK_SIZE;
			}//C

			metadata_t* current = size_list;
	//while(current != NULL && (current->size) <= realsize){//D
	while(current != NULL){//D
		if(current->size==realsize){//D.1
			////////////////////////////////
			//deleting the block from addr and size
			///////////////////////////////
			removefromaddress(current);
			removefromsize(current);
			////////////////////////////////
			//getting the canaries
			////////////////////////////////
			unsigned long mycanary = ((uintptr_t)current ^ CANARY_MAGIC_NUMBER) + 1;
			current->canary=mycanary;
			unsigned long *trailing_canary = (unsigned long *)((uint8_t *)current + current->size - sizeof(unsigned long));
			*trailing_canary=mycanary;			

			///////////////////////////////
			//returning pointer to body
			////////////////////////////////
			current->next_addr = NULL;
			current->prev_addr = NULL;
			current->next_size = NULL;
			current->prev_size = NULL;

			//void* bodypointer = ((uint8_t *)current + TOTAL_METADATA_SIZE - sizeof(unsigned long));
			my_malloc_errno = NO_ERROR;
			return (uint8_t*)current+ sizeof(metadata_t);
			///////////////////////////////
						}//D.1
		//if(((current->size - size)>=MIN_BLOCK_SIZE)||((current->size)<realsize)){//D.2
			if((long)(current-> size - realsize) >= (long)MIN_BLOCK_SIZE){
			current = split(size,current);
			void* bodypointer = ((uint8_t *)current + TOTAL_METADATA_SIZE - sizeof(unsigned long));
			my_malloc_errno = NO_ERROR;
			return bodypointer;
			}//D.2
		 	current = current->next_size;
		}//D
			current = size_list;
						my_malloc_errno = NO_ERROR;
						return NULL;
}//END

/* REALLOC
 * See my_malloc.h for documentation
 */
void *my_realloc(void *ptr, size_t size) {
	UNUSED_PARAMETER(ptr);
	UNUSED_PARAMETER(size);
	if(size == 0){
		my_free(ptr);
		return NULL;
	}
	if(ptr == NULL){
		return my_malloc(size);
	}
	if(size>SBRK_SIZE) {
		my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
		return NULL;
	}
	return NULL;
}

/* CALLOC
 * See my_malloc.h for documentation
 */
void *my_calloc(size_t nmemb, size_t size) {
	UNUSED_PARAMETER(nmemb);
	UNUSED_PARAMETER(size);
	void* temp = my_malloc(nmemb * size);
	char* mypointer=temp;	
	for(unsigned int i=0;i<size;i++){
		*mypointer = 0;
		mypointer=mypointer + 1;
	}
	return temp;
}

/* FREE
 * See my_malloc.h for documentation
 */
void my_free(void *ptr) {
	UNUSED_PARAMETER(ptr);
	if(ptr == NULL){
		my_malloc_errno= NO_ERROR;
		return;
	}
	metadata_t* freepoint =  (metadata_t*)ptr - 1;
	unsigned long mycanary = ((uintptr_t)freepoint ^ CANARY_MAGIC_NUMBER) + 1;
	if(mycanary!=(freepoint->canary)){
		my_malloc_errno = CANARY_CORRUPTED;
		return;
	}
	unsigned long *trailing_canary = (unsigned long *)((uint8_t *)freepoint + freepoint->size - sizeof(unsigned long));
	if(mycanary!=*trailing_canary){
		my_malloc_errno = CANARY_CORRUPTED;
		return;
	}
	my_malloc_errno = NO_ERROR;
	addtosize(freepoint);
	addtoaddress(freepoint);
}

void removefromaddress(metadata_t* current)
{
			//IF ONLY BLOCK
	if(current->prev_addr==NULL&&current->next_addr==NULL){		
		address_list=NULL;	
	}
			//IF BLOCK TO LEFT IS EMPTY
	else if(current->prev_addr==NULL){
		address_list = current->next_addr;
		((current->next_addr)->prev_addr)=NULL;
	}
			//IF BLOCK TO RIGHT IS EMPTY
	else if(current->next_addr==NULL){
		(current->prev_addr)->next_addr=NULL;
	}
			//IS IN MIDDLE OF NORMAL ARRAY
	else	{
		(current->prev_addr)->next_addr=current->next_addr;
		(current->next_addr)->prev_addr=current->prev_addr;
	}
}

void removefromsize(metadata_t* current)
{
			//IF ONLY BLOCK
	if(current->prev_size==NULL&&current->next_size==NULL){		
		size_list=NULL;	
	}
			//IF BLOCK TO LEFT IS EMPTY
	else if(current->prev_size==NULL){
		size_list = current->next_size;
		current->next_size->prev_size=NULL;
	}
			//IF BLOCK TO RIGHT IS EMPTY
	else if(current->next_size==NULL){
		current->prev_size->next_size=NULL;
	}
			//IS IN MIDDLE OF NORMAL ARRAY
	else	{
		current->prev_size->next_size=current->next_size;
		current->next_size->prev_size=current->prev_size;
	}
}

void addtosize(metadata_t* current) {
	//IF SIZE LIST IS EMPTY
	if(size_list==NULL){//A
		size_list=current;
			   }//A
	//ITERATING THROUGH LINKED LIST
			   metadata_t* shifter = size_list;
	if(current->size<shifter->size) {
	 	current->prev_size=NULL;
	 	current->next_size=shifter;
	 	shifter->prev_size=current;
	 	size_list=current;
	}
	while((shifter -> next_size != NULL)&&(shifter -> next_size -> size < current -> size)){//B
		shifter = shifter -> next_size;
	}
	if(shifter -> next_size == NULL){
		current->next_size=NULL;
		current->prev_size=shifter;
		shifter->next_size=current;
	} 
	else if(shifter -> next_size != NULL) {
	 	current->next_size=shifter;
	 	current->prev_size=shifter->prev_size;
	 	current->prev_size->next_size=current;
	 	shifter->prev_size=current;
	}
}


void addtoaddress(metadata_t* current) {
	//IF SIZE LIST IS EMPTY
	if(address_list==NULL){//A
		address_list=current;
			   }//A
	//ITERATING THROUGH LINKED LIST
			   metadata_t* shifter = address_list;
	if(current<shifter) {
	 	current->prev_addr=NULL;
	 	current->next_addr=shifter;
	 	shifter->prev_addr=current;
	 	address_list=current;
	}
	while((shifter -> next_addr != NULL)&&(shifter -> next_addr < current)){//B
		shifter = shifter -> next_addr;
	}
	if(shifter -> next_addr == NULL){
		current->next_addr=NULL;
		current->next_addr=shifter;
		shifter->next_addr=current;
	} 
	else if(shifter -> next_addr != NULL) {
	 	current->next_addr=shifter;
	 	current->prev_addr=shifter->prev_addr;
	 	current->prev_addr->next_addr=current;
	 	shifter->prev_addr=current;
	}
}
	 void* split (int wantsize,metadata_t* current) {		
			 	UNUSED_PARAMETER(wantsize);
			 	UNUSED_PARAMETER(current);
			 	int wantrealsize = wantsize+TOTAL_METADATA_SIZE;
			 	//printf("\n \n %d \n \n ", wantrealsize);
	//SETTING UP THE NEW METADATA
			 	metadata_t* new_metapoint = (metadata_t*)((uint8_t *)current+(current->size-wantrealsize));

	//SETTING UP SIZE
			 	new_metapoint->size=wantrealsize;
	//SETTING UP HEAD CANARY
			 	unsigned long mycanary = ((uintptr_t)new_metapoint ^ CANARY_MAGIC_NUMBER) + 1;
			 	new_metapoint->canary=mycanary;
	//SETTING UP TAIL CANARY
			 	unsigned long *trailing_canary = (unsigned long *)((uint8_t *)new_metapoint + new_metapoint->size - sizeof(unsigned long));
			 	*trailing_canary=mycanary;
	//CLEARING UP POINTERS
			 	new_metapoint->next_addr = NULL;
			 	new_metapoint->prev_addr = NULL;
			 	new_metapoint->next_size = NULL;
			 	new_metapoint->prev_size = NULL;			
	//SHRINKING THE OLD BLOCKS SIZE	
			 	current->size=(long)(current->size)-wantrealsize;
			 	//printf("\n \n %lu \n \n ", current->size);
	//REMOVING IT
			 	removefromsize(current);
	//ADDING IT BACK
			 	addtosize(current);
			 	return new_metapoint;
			 }
