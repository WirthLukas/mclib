//
// Created by lukew on 19.12.2022.
//

#ifndef MODERNC_DYNARRAY_H
#define MODERNC_DYNARRAY_H

#if defined(MCLIB_REALLOC) && !defined(MCLIB_FREE) || !defined(MCLIB_REALLOC) && defined(MCLIB_FREE)
#error "You must define both MCLIB_REALLOC and MCLIB_FREE, or neither."
#endif

#if !defined(MCLIB_REALLOC) && !defined(MCLIB_FREE)

#include <malloc.h>
#define MCLIB_REALLOC(ptr, size) realloc(ptr, size)
#define MCLIB_FREE(ptr) free(ptr)

#endif

#include <stddef.h> // NULL, size_t
#include "params.h"

#define dyn_array_t(T) T*
#define DYN_ARRAY_DEFAULT_CAPACITY 4

size_t dyn_array_length(void* arr);
size_t dyn_array_capacity(void* arr);
size_t dyn_array_element_size(void* arr);

void* dyn_array_init(params_data_t params);
void dyn_array_add(void* ptr_to_array, void* value);
void dyn_array_add_many(void* ptr_to_array, params_data_t params);
void dyn_array_insert_one_at(void* ptr_to_array, size_t index, void* value);
void dyn_array_insert_many_at(void* ptr_to_array, size_t index, params_data_t params);
void dyn_array_delete_one(void* ptr_to_array, size_t index);
void dyn_array_delete_many(void* ptr_to_array, size_t index, int n);
void dyn_array_delete(void* ptr_to_array);

#include "macros.h"
#define using_dyn_array(dyn_array) scope(dyn_array_delete(&dyn_array))

typedef struct dyn_array_iterator {
    void** array;           // double pointer in order to know if the actual array pointer is set to NULL
    size_t current_index;
} dyn_array_iterator_t;

dyn_array_iterator_t dyn_array_iterator_new(void** arr);
void* dyn_array_iterator_next(dyn_array_iterator_t* iterator);
void dyn_array_iterator_reset(dyn_array_iterator_t* iterator);

#ifdef MCLIB_DYNARRAY_IMPLEMENTATION

//
// Created by lukew on 19.12.2022.
//

#include <string.h>
#include <assert.h>

typedef struct dyn_array_header {
    size_t length;
    size_t capacity;
    size_t element_size;
} dyn_array_header_t;

static inline dyn_array_header_t* dyn_array_header(const void* arr) {
    //    assert(header->ptr == arr && "Did not get the correct header! Perhaps you have to pass `&array` instead of `array` to the function or your array is not an array created by dyn_array_init");
    return (dyn_array_header_t*)arr - 1;
}

size_t dyn_array_length(void* arr) {
    return arr != NULL ? dyn_array_header(arr)->length : 0;
}

size_t dyn_array_capacity(void* arr) {
    return arr != NULL ? dyn_array_header(arr)->capacity : 0;
}

size_t dyn_array_element_size(void* arr) {
    return arr != NULL ? dyn_array_header(arr)->element_size : 0;
}

void* dyn_array_grow(void* arr, size_t element_size, size_t add_length) {
    size_t needed_length = dyn_array_length(arr) + add_length;
    size_t capacity = dyn_array_capacity(arr);

    if (arr != NULL && needed_length <= capacity)
        return arr;

    // https://stackoverflow.com/questions/2987207/why-do-i-get-a-c-malloc-assertion-failure
    // it may occur that the doubling the capacity once is not enough to fit the needed length (especially fo add_many)
    // through this error it might come to failures with malloc
//    size_t new_capacity = capacity > 0 ? capacity : DYN_ARRAY_DEFAULT_CAPACITY;
//
//    while (new_capacity < needed_length) {
//        new_capacity = new_capacity * 2;
//    }
    // the loop above can be ommitted by simple math calculations
    // this is a formula that calculates how often we have to double capacity to fit needed length
    // this can be recreated by starting with the equation: needed_length < x * 2 * capacity
    // results in: needed_length / (2 * capacity) < x
    // => therefore each number greater than needed_length / (2 * capacity) can be used
    // through the integer division: adding 1 is the perfect number for x
    size_t new_capacity = capacity > 0 ? (needed_length / (2 * capacity) + 1) * 2 * capacity : DYN_ARRAY_DEFAULT_CAPACITY;

    void* new_arr = MCLIB_REALLOC(arr ? dyn_array_header(arr) : NULL, element_size * new_capacity + sizeof(dyn_array_header_t));
    new_arr = (char *)new_arr + sizeof(dyn_array_header_t);
    dyn_array_header_t* header = dyn_array_header(new_arr);

    if (arr == NULL) {
        header->length = 0;
        header->element_size = element_size;
    }

    header->capacity = new_capacity;
    return new_arr;
}

void* dyn_array_init(params_data_t params) {
    void* arr = dyn_array_grow(NULL, params.element_size, params.length);
    dyn_array_add_many(&arr, params);
    return arr;
}

void dyn_array_ensure_capacity(void* ptr_to_array, size_t add_length) {
    void* arr = *(void**)ptr_to_array;
    dyn_array_header_t* header = dyn_array_header(arr);

    if (arr == NULL || header->length + add_length > header->capacity) {
//        *arr = dyn_array_grow(*arr, sizeof(*arr), add_length);
        *(void**)ptr_to_array = dyn_array_grow(arr, header->element_size, add_length);
    }
}

//#define dyn_array_add(arr, element) dyn_array_ensure_capacity((arr), 1); (*(arr))[dyn_array_header(*(arr))->length++] = (element);

void dyn_array_add(void* ptr_to_array, void* value) {
    dyn_array_ensure_capacity(ptr_to_array, 1);
    dyn_array_header_t* header = dyn_array_header(*(void**)ptr_to_array);

    // [length] adds length * pointer_size to the pointer. But sizeof void* is 1. Therefore, you have to consider the  original elementsize
//    void* p_to_element = &(*arr)[header->length * header->element_size];        // same meaning but raises warning
//    void* p_to_element = (*arr) + header->length * header->element_size;
//    memcpy(p_to_element, value, header->element_size);
//    header->length++;

    memcpy(*(void**)ptr_to_array + header->length++ * header->element_size, value, header->element_size);
}

void dyn_array_add_many(void* ptr_to_array, params_data_t params) {
//    for (int i = 0; i < params.length; i++) {
//        dyn_array_add(arr, params.data + i * params.element_size);
//    }

    dyn_array_ensure_capacity(ptr_to_array, params.length);
    dyn_array_header_t* header = dyn_array_header(*(void**)ptr_to_array);

    for (size_t i = 0; i < params.length; i++, header->length++) {
        memcpy(
                *(void**)ptr_to_array + header->length * header->element_size,
                params.data + i * params.element_size,
                header->element_size);
    }
}

void dyn_array_insert_one_at(void* ptr_to_array, size_t index, void* value) {
    dyn_array_header_t* header = dyn_array_header(*(void**)ptr_to_array);

    // inserting at the end of the array means index == array.length
    // therefore it should be allowed
    if (index > header->length) {
        return;
    }

    dyn_array_ensure_capacity(ptr_to_array, 1);

    memmove(
            *(void**)ptr_to_array + (index +  1) * header->element_size,
            *(void**)ptr_to_array + index * header->element_size,
            header->element_size * (header->length - index));

    memcpy(*(void**)ptr_to_array + index * header->element_size, value, header->element_size);
    header->length++;
}

void dyn_array_insert_many_at(void* ptr_to_array, size_t index, params_data_t params) {
    dyn_array_ensure_capacity(ptr_to_array, params.length);
    dyn_array_header_t* header = dyn_array_header(*(void**)ptr_to_array);

    memmove(
            *(void**)ptr_to_array + (index +  params.length) * header->element_size,
            *(void**)ptr_to_array + index * header->element_size,
            header->element_size * (header->length - index));

    for (size_t i = 0; i < params.length; i++) {
        memcpy(
                *(void**)ptr_to_array + (index + i) * header->element_size,
                params.data + i * params.element_size,
                header->element_size);
    }

    header->length += params.length;
}

void dyn_array_delete_one(void* ptr_to_array, size_t index) {
    dyn_array_delete_many(ptr_to_array, index, 1);
}

void dyn_array_delete_many(void* ptr_to_array, size_t index, int n) {
    dyn_array_header_t* header = dyn_array_header(*(void**)ptr_to_array);

    if (index > header->length) {
        return;
    }

    memmove(*(void**)ptr_to_array + index * header->element_size,
            *(void**)ptr_to_array + (index + n) * header->element_size,
            header->element_size * (header->length - n - index));
    header->length -= n;
}

void dyn_array_delete(void* ptr_to_array) {
    MCLIB_FREE(dyn_array_header(*(void**)ptr_to_array));
    *(void**)ptr_to_array = NULL;
}

dyn_array_iterator_t dyn_array_iterator_new(void** arr) {
    return (dyn_array_iterator_t) { .array = arr, .current_index = 0 };
}

void* dyn_array_iterator_next(dyn_array_iterator_t* iterator) {
    dyn_array_header_t* header = dyn_array_header(*(iterator->array));
    return *(iterator->array) != NULL && iterator->current_index < header->length ? *(iterator->array) + iterator->current_index++ * header->element_size : NULL;
}

void dyn_array_iterator_reset(dyn_array_iterator_t* iterator) {
    iterator->current_index = 0;
}

#endif //MCLIB_DYNARRAY_IMPLEMENTATION

#endif //MODERNC_DYNARRAY_H
