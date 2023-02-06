//
// Created by lukew on 29.12.2022.
//

#include "memcheck.h"
#include <stdio.h>

struct MemAlloc {
    void *pntr;
    bool freed;
    char* file;
    int line;
};

static struct MemAlloc allocs[MAX_ALLOCS];
static bool freed_unowned_mem = false;
static bool double_free = false;

void _do_reset() {
    for (int i = 0; i < MAX_ALLOCS; i++){
        allocs[i].pntr = 0;
        allocs[i].freed = false;
    }
    freed_unowned_mem = false;
    double_free = false;
}

int _get_next_free_idx(void *address) {
    for (int i = 0; i < MAX_ALLOCS; i++){
        if (allocs[i].pntr == 0 || (allocs[i].pntr == address && allocs[i].freed == true)){
            return i;
        }
    }
    printf("Exceeded max allocs\n");
    return -1;
}

void* smalloc(size_t size, char* file, int line) {
    void *pntr = malloc(size);
    int idx = _get_next_free_idx(pntr);
    allocs[idx].pntr = pntr;
    allocs[idx].freed = false;
    allocs[idx].file = file;
    allocs[idx].line = line;
    return pntr;
}

void* srealloc(void* adress, size_t size, char* file, int line) {
    void* nptr = realloc(adress, size);

    // realloc allocates new memory only if the adress is NULL
    // otherwise old one is freed and new one is returned
    if (adress == NULL) {
        int idx = _get_next_free_idx(nptr);
        allocs[idx].pntr = nptr;
        allocs[idx].freed = false;
        allocs[idx].file = file;
        allocs[idx].line = line;
    }

    return nptr;
}

void sfree(void *address){
    int i = 0;
    while (allocs[i].pntr != address && i < MAX_ALLOCS){
        i++;
    }
    if (i == MAX_ALLOCS - 1){
        printf("Attempted to free unowned memory %p\n", address);
        freed_unowned_mem = true;
        return;
    }
    if (allocs[i].freed){
        printf("Attempted to free already freed memory %p\n", address);
        double_free = true;
        return;
    }
    allocs[i].freed = true;
    free(address);
}

bool memcheck_validate() {
    if (freed_unowned_mem || double_free){
        _do_reset();
        return false;
    }
    int not_freed_cnt = 0;
    for (int i = 0; i < MAX_ALLOCS; i++){
        if (allocs[i].pntr != 0 && !allocs[i].freed){
            printf("Pointer created in file %s at %d not freed\n", allocs[i].file, allocs[i].line);
            not_freed_cnt++;
        }
    }
    _do_reset();
    if (not_freed_cnt > 0) {
        printf("%d allocations have not been freed\n", not_freed_cnt);
        return false;
    }
    return true;
}