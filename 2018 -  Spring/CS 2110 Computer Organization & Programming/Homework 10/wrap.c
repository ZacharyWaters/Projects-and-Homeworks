#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrap.h"

alloc_tester t;

int push_onto_allocs(alloc_tester* t1, alloc_info* info) {
    if(t1->alloc_size == HISTORY_SIZE) {
        fprintf(stderr, "ERROR! alloc_size == HISTORY_SIZE");
        return 1;
    } else {
        t1->allocs[t1->alloc_size] = *info;
        t1->alloc_size++;
        return 0;
    }
}

void* __wrap_calloc(size_t nitems, size_t size) {

    // one more calloc call
    t.calloc_calls++;

    // setup info object
    alloc_info info;
    memset(&info, 0, sizeof(alloc_info));
    info.type = C_ALLOC;
    info.valid = true;
    info.alloc_size = nitems * size;

    // not enough malloc/calloc calls left so fail
    if(t.malloc_calloc_budget < t.malloc_calls + t.calloc_calls) {
        info.success = false;
        push_onto_allocs(&t, &info);
        return NULL;
    }

    // continue on and complete the call
    info.success = true;
    info.actual_addr = __real_calloc(nitems, size);
    push_onto_allocs(&t, &info);
    return info.actual_addr;
}

void *__wrap_realloc(void *ptr, size_t size) {

    // one more realloc call
    t.realloc_calls++;

    // setup info object
    alloc_info info;
    memset(&info, 0, sizeof(alloc_info));
    info.type = RE_ALLOC;
    info.valid = true;
    info.alloc_size = size;
    info.alloc_ptr = ptr;

    // not enough realloc calls left so fail
    if(t.realloc_budget < t.realloc_calls) {
        info.success = false;
        push_onto_allocs(&t, &info);
        return NULL;
    }

    // find the call with this address
    alloc_info* matching_call = NULL;
    for(int i = t.alloc_size - 1; i >= 0; i--) {
        alloc_info* curr = &t.allocs[i];
        if(curr->valid && curr->actual_addr == ptr) {
            matching_call = curr;
            break;
        }
    }

    // fails if no matching previous alloc call
    if(!matching_call) {
        info.success = false;
        push_onto_allocs(&t, &info);
        return NULL;
    }

    // mark last malloc call as freed
    matching_call->freed = true;

    // continue on and complete the call
    info.success = true;
    info.actual_addr = __real_realloc(ptr, size);
    push_onto_allocs(&t, &info);
    return info.actual_addr;
}

void* __wrap_malloc (size_t size) {

    // one more calloc call
    t.malloc_calls++;

    // setup info object
    alloc_info info;
    memset(&info, 0, sizeof(alloc_info));
    info.type = M_ALLOC;
    info.valid = true;
    info.alloc_size = size;

    // not enough malloc/calloc calls left so fail
    if(t.malloc_calloc_budget < t.malloc_calls + t.calloc_calls) {
        info.success = false;
        push_onto_allocs(&t, &info);
        return NULL;
    }

    // continue on and complete the call
    info.success = true;
    info.actual_addr = __real_malloc(size);
    push_onto_allocs(&t, &info);
    return info.actual_addr;
}

void __wrap_free(void *ptr) {

    // one more free call
    t.free_calls++;

    // setup info object
    alloc_info info;
    memset(&info, 0, sizeof(alloc_info));
    info.type = FREE_ALLOC;
    info.valid = true;
    info.alloc_ptr = ptr;

    // try to free this ptr
    alloc_info* found_info = NULL;
    for(int i = 0; i < t.alloc_size; i++) {

        alloc_info* curr = &t.allocs[i];
        if(curr->valid && curr->actual_addr == ptr) {
            found_info = curr;
            break;
        }
    }

    // bad free
    if(!found_info) {
        info.success = false;

    // good free
    } else {
        info.success = true;
        found_info->freed = true;
        __real_free(ptr);
    }

    push_onto_allocs(&t, &info);
}

void cleanup(alloc_tester* info) {
    for(int i = 0; i < info->alloc_size; i++) {
        alloc_info* curr = &info->allocs[i];
        if(curr->valid && curr->success && !curr->freed && IS_ALLOC(curr->type)) {
            __real_free(curr->actual_addr);
        }
    }
}

alloc_info* find_call_in_allocs_by_size(alloc_tester* tester, enum ALLOC_TYPE type, size_t size, int n) {
    alloc_info* info = NULL;
    for(int i = tester->alloc_size - 1; i >= tester->alloc_size - n; i--) {
        alloc_info* curr = &tester->allocs[i];
        if(curr->valid && curr->type == type && curr->alloc_size == size) {
            info = curr;
            break;
        }
    }
    return info;
}

bool all_freed(alloc_tester* tester) {
    for(int i = 0; i < tester->alloc_size; i++) {
        alloc_info* curr = &tester->allocs[i];
        if(curr->valid && curr->success && IS_ALLOC(curr->type) && !curr->freed) return false;
    }
    return true;
}

// do we have invalid frees?
bool invalid_frees(alloc_tester* tester) {
    for(int i = 0; i < tester->alloc_size; i++) {
        alloc_info* curr = &tester->allocs[i];
        if(curr->valid && !curr->success && curr->type == FREE_ALLOC) return true;
    }
    return false;
}
