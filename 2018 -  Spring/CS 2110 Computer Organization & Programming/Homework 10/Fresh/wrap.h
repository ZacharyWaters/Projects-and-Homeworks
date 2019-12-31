#include <stdbool.h>
#include <stdio.h>

#ifndef HW10_WRAP_H
#define HW10_WRAP_H

#define HISTORY_SIZE 100

#define TERM_RED     "\x1b[31m"
#define TERM_GREEN   "\x1b[32m"
#define TERM_RESET   "\x1b[0m"

enum ALLOC_TYPE {M_ALLOC, C_ALLOC, RE_ALLOC, FREE_ALLOC};

typedef struct _alloc_info {

    // struct bookkeeping
    enum ALLOC_TYPE type;
    bool valid;

    // did this malloc/call/realloc succeed?
    bool success;

    // was alloc'ed memory freed
    bool freed;

    // malloc/calloc/realloc
    size_t alloc_size;

    // realloc/free
    void *alloc_ptr;

    // actual malloc/realloc/calloc'ed address
    void* actual_addr;

} alloc_info;

typedef struct _alloc_tester {

    // num successful calls
    int malloc_calloc_budget, realloc_budget;

    // call counts
    int malloc_calls, calloc_calls, realloc_calls, free_calls;

    // size of alloc info array
    int alloc_size;
    alloc_info allocs[HISTORY_SIZE];

} alloc_tester;

// wrappers
void* __wrap_malloc (size_t size);
void* __wrap_calloc(size_t nitems, size_t size);
void* __wrap_realloc(void *ptr, size_t size);
void __wrap_free(void *ptr);

int push_onto_allocs(alloc_tester* t1, alloc_info* info);

void cleanup(alloc_tester* info);
alloc_info* find_call_in_allocs_by_size(alloc_tester* tester, enum ALLOC_TYPE type, size_t size, int n, int start);
//bool in_last_n(malloc_info* info, size_t val, int n);
//bool cleaned(malloc_info* info);
bool invalid_frees(alloc_tester* tester);
bool all_freed(alloc_tester* tester);

extern void *__real_malloc(size_t size);
extern void __real_free(void *ptr);
extern void *__real_calloc(size_t nmemb, size_t size);
extern void *__real_realloc(void *ptr, size_t size);

#define IS_ALLOC(x) (((x) == M_ALLOC || (x) == RE_ALLOC || (x) == C_ALLOC))

#endif //HW10_WRAP_H
