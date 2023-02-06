//
// Created by lukew on 12.01.2023.
//

#ifndef MODERNC_ALLOCATOR_H
#define MODERNC_ALLOCATOR_H

// https://github.com/orangeduck/tgc
// https://accu.org/conf-docs/PDFs_2021/luca_sass_modern_c_and_what_we_can_learn_from_it.pdf
// https://github.com/Snaipe/libcsptr

// https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator
// https://github.com/thejefflarson/arena

// https://www.gamedev.net/articles/programming/general-and-gameplay-programming/c-custom-memory-allocation-r3010/

#include <stddef.h>

typedef void* (*allocation_callback)(const void* allocator, size_t amount_to_alloc, void* ptr_to_free);

typedef struct allocation_strategy {
    const void* allocator;
    allocation_callback allocation_callback;
} allocation_strategy_t;

allocation_strategy_t malloc_allocation_strategy();

#define allocation_strategy_allocate(path_to_allocation_strategy, size) (path_to_allocation_strategy).allocation_callback((path_to_allocation_strategy).allocator, size, 0)
#define allocation_strategy_free(path_to_allocation_strategy, free_ptr) (path_to_allocation_strategy).allocation_callback((path_to_allocation_strategy).allocator, 0, free_ptr)
#define allocation_strategy_alloc_and_free(path_to_allocation_strategy, size, free_ptr) (path_to_allocation_strategy).allocator_callback((path_to_allocation_strategy).allocator, size, free_ptr)

typedef struct allocator {
    void* memory;
    size_t capacity;
    size_t memory_used;
} allocator_t;

//allocator_t allocator_new(size_t size, void* memory);
void* allocator_get(allocator_t* allocator, size_t size);
void allocator_clear(allocator_t* allocator);
void allocator_delete(allocator_t* allocator);

allocation_strategy_t linear_allocator_allocation_strategy(allocator_t* allocator);

#define allocator_new(size, given_memory) \
    ((allocator_t) {                \
        .memory = given_memory,           \
        .capacity = size,           \
        .memory_used = 0            \
    })

#include "macros.h"

#define using_linear_allocator(allocator) scope(allocator_delete(allocator))

#ifdef MODERNC_ALLOCATOR_IMPLEMENTATION

void* malloc_allocation_callback(const void* _, size_t size, void* free_ptr) {
    if (free_ptr != NULL) {
        free(free_ptr);
    }

    if (size > 0) {
        return malloc(size);
    }

    return NULL;
}

allocation_strategy_t malloc_allocation_strategy() {
    return (allocation_strategy_t) {
            .allocator = NULL,
            .allocation_callback = &malloc_allocation_callback
    };
}

void* allocator_allocation_callback(const void* allocator, size_t size, void* free_ptr) {
    if (size > 0) {
        return allocator_get(((allocator_t*)allocator), size);
    }

    return NULL;
}

allocation_strategy_t linear_allocator_allocation_strategy(allocator_t* allocator) {
    return (allocation_strategy_t) {
            .allocator = allocator,
            .allocation_callback = &allocator_allocation_callback
    };
}

//allocator_t allocator_new(size_t size, void* memory) {
//    if (memory == NULL || size <= 0) {
//        //error
//    }
//
//    allocator_t allocator = {
//            .memory = memory,
//            .capacity = size,
//            .memory_used = 0
//    };
//
//    return allocator;
//}


void* allocator_get(allocator_t* allocator, size_t size) {
    if (size <= 0 || allocator == NULL || allocator->memory_used + size > allocator->capacity) {
        return NULL;
    }

    void* ptr = allocator->memory + allocator->memory_used;
    allocator->memory_used += size;
    return ptr;
}

void allocator_clear(allocator_t* allocator) {
//    allocator->start_of_free_mem = allocator->memory;
    allocator->memory_used = 0;
}

void allocator_delete(allocator_t* allocator) {
//    free(allocator->memory);
//    allocation_callback->memory = NULL;
//    allocation_callback->start_of_free_mem = NULL;
//    allocation_callback->capacity = 0;
//    *allocation_callback = (allocator_t){
//            .memory = NULL,
//            .capacity = 0,
//            .start_of_free_mem = NULL
//    };
    *allocator = (allocator_t){ 0 };
}

#endif // MODERNC_ALLOCATOR_IMPLEMENTATION

#endif //MODERNC_ALLOCATOR_H
