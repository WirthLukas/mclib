//
// Created by lukew on 09.01.2023.
//

#ifndef MODERNC_SMARTPTR_H
#define MODERNC_SMARTPTR_H

//#ifndef __GNUC__
//#error "this library needs the GCC __attribute__ feature"
//#endif

#include <stddef.h>
#include <malloc.h>
#include <assert.h>

typedef struct smart_ptr_meta {
    void (*dtor)(void*);
    void* ptr;
} smart_ptr_meta_t;

static inline smart_ptr_meta_t* get_meta(void* ptr) {
    return ptr - sizeof(smart_ptr_meta_t);  // or (smart_ptr_meta_t*)ptr - 1
}

void* smart_alloc(size_t size, void (*dtor)(void*)) {
    smart_ptr_meta_t* meta = malloc(sizeof(smart_ptr_meta_t) + size);
    meta->dtor = dtor;
    meta->ptr = meta + 1;
    return meta->ptr;
}

void smart_free(void* ptr) {
    if (ptr == NULL) return;

    smart_ptr_meta_t* meta = get_meta(ptr);
    assert(ptr == meta->ptr && "Ptr in header is not same as given ptr!");

    if (meta->dtor) {
        meta->dtor(ptr);
    }

    free(meta);
}

#define using __attribute__((cleanup(smart_free_stack)))

// __attribute__ gives as the pointer to a pointer
// but to ignore the warning of casting (eg int** to void**), whe just use void* and cast it in the function
static inline void smart_free_stack(void* ptr) {
    smart_free(*(void**)ptr);
}

#endif //MODERNC_SMARTPTR_H
