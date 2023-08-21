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
// http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/

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

typedef enum allocator_error {
    NoError = 0, NotEnoughMemory, MemoryNullPointer
} allocator_error_t;

typedef struct arena {
    void* memory;
    size_t capacity;
    size_t memory_used;
    allocator_error_t error;
} arena_t;

arena_t arena_new(void* memory, size_t size);
void* arena_alloc(arena_t* allocator, size_t size);
void arena_clear(arena_t* allocator);

allocation_strategy_t arena_allocation_strategy(arena_t* allocator);

typedef struct proxy_allocator {
    allocation_strategy_t strategy;

    void* data;
    void (* update)(void* data, int event);
} proxy_allocator_t;

void* proxy_allocator_alloc(proxy_allocator_t* allocator, size_t size) {
    void* data = allocation_strategy_allocate(allocator->strategy, size);
    allocator->update(allocator->data, 0);
    return data;
}

void proxy_allocator_clear(proxy_allocator_t* allocator) {
    // TODO:
}

typedef struct allocator_ops {
    void* (*alloc)(struct allocator_ops** self, size_t size);
    void (*dealloc)(struct allocator_ops** self, void* ptr);
} allocator_ops_t;

typedef struct linear_allocator {
    allocator_ops_t ops;
    void* memory;
} linear_allocator_t;

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

void* __linear_allocator_alloc(allocator_ops_t** ops, size_t size) {
    linear_allocator_t* self = container_of(*ops, linear_allocator_t, ops);
    self->memory = NULL;
}

void linear_allocator_init(linear_allocator_t* self) {
    *self = (linear_allocator_t) {
            .ops = {
                    .alloc = &__linear_allocator_alloc
            },
            .memory = NULL
    };
}

void* linear_allocator_alloc(linear_allocator_t* self, size_t size) {
    allocator_ops_t* ops = &self->ops;
    return self->ops.alloc(&ops, size);
}

#ifdef MODERNC_ALLOCATOR_IMPLEMENTATION

void* malloc_allocation_callback(const void* _, size_t size, void* free_ptr) {
    if (free_ptr != NULL) {
        free(free_ptr);
    }

    return size > 0 ? malloc(size) : NULL;
}

allocation_strategy_t malloc_allocation_strategy() {
    return (allocation_strategy_t) {
            .allocator = NULL,
            .allocation_callback = &malloc_allocation_callback
    };
}

void* allocator_allocation_callback(const void* allocator, size_t size, void* free_ptr) {
        return arena_alloc(((arena_t *) allocator), size);
}

allocation_strategy_t arena_allocation_strategy(arena_t* allocator) {
    return (allocation_strategy_t) {
            .allocator = allocator,
            .allocation_callback = &allocator_allocation_callback
    };
}

arena_t arena_new(void* memory, size_t size) {
    return memory == NULL
        ? (arena_t) { .error = MemoryNullPointer }
        : (arena_t) {
            memory = memory,
            .capacity = size,
            .memory_used = 0
        };
}

void* arena_alloc(arena_t *allocator, size_t size) {
    if (allocator == NULL) {
        return NULL;
    }

    if (allocator->memory == NULL)
    {
        allocator->error = MemoryNullPointer;
        return NULL;
    }

    if (allocator->memory_used + size > allocator->capacity)
    {
        allocator->error = NotEnoughMemory;
        return NULL;
    }

    void* ptr = allocator->memory + allocator->memory_used;
    allocator->memory_used += size;
    return ptr;
}

void arena_clear(arena_t *allocator) {
    allocator->memory_used = 0;
}

#endif // MODERNC_ALLOCATOR_IMPLEMENTATION

#endif //MODERNC_ALLOCATOR_H
