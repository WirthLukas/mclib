#include <stdio.h>
#include "memcheck.h"

#include "mclib/params.h"

#define MCLIB_DYNARRAY_IMPLEMENTATION
 #define MCLIB_REALLOC(ptr, size) srealloc(ptr, size, __FILE__, __LINE__)
//#define MCLIB_REALLOC(ptr, size) smalloc(size, __FILE__, __LINE__)
#define MCLIB_FREE(ptr) sfree(ptr)
#include "mclib/dynarray.h"
#include "mclib/smartptr.h"

#define MODERNC_ALLOCATOR_IMPLEMENTATION
#include "mclib/allocator.h"

#include "mclib/macros.h"

#define MODERNC_STR_IMPLEMENTATION
#include "mclib/str.h"

void params_test(params_data_t data) {
//    params_data_t x = params(int, 1, 2, 3, 4, 5, 6);
    printf("\nLength: %ld\nelement_size: %ld\n", data.length, data.element_size);
    printf("items: ");

    for (int i = 0; i < data.length; i++) {
        printf("%d ", ((int*)data.data)[i]);
    }

    printf("\n");
    printf("Size: %ld\n....length: %ld\n....element_size: %ld\n....data: %ld\n", sizeof(data), sizeof(data.length),
           sizeof(data.element_size), sizeof(data.data));

    printf("Adress of Params: %p\nAdress of data %p\n\n", &data, data.data);
}

float minf(float a, float b) {
    return a < b ? a : b;
}

int mini(int a, int b) {
    return a < b ? a : b;
}

#define min(a, b) _Generic((a), float: minf(a, b), int: mini(a, b))

void dyn_array_test() {
    dyn_array_t(int) ints = dyn_array_init(params(int, 10, 20, 30));

//    int a = 10;
//    printf("add %d\n", a);
//    dyn_array_add((void **) &ints, &a);
//    a = 20;
//    printf("add %d\n", a);
//    dyn_array_add((void **) &ints, &a);

    dyn_array_add_many(&ints, params(int, 40, 50, 60, 70, 80, 90));
    for (int i = 0; i < dyn_array_length(ints); i++) {
        printf("%d ", ints[i]);
    }

    printf("\n");

    int value = 45;
    dyn_array_insert_one_at(&ints, 4, &value);
    dyn_array_insert_many_at(&ints, 4, params(int, 42, 44, 46, 48));

    for (int i = 0; i < dyn_array_length(ints); i++) {
        printf("%d ", ints[i]);
    }

    printf("\n");

    dyn_array_delete_many(&ints, 4, 3);

    for (int i = 0; i < dyn_array_length(ints); i++) {
        printf("%d ", ints[i]);
    }

    printf("\n");

    dyn_array_delete_many(&ints, 4, 2);

    for (int i = 0; i < dyn_array_length(ints); i++) {
        printf("%d ", ints[i]);
    }

    printf("\n");

    ints[2] = 100;

    for (int i = 0; i < dyn_array_length(ints); i++) {
        printf("%d ", ints[i]);
    }

    printf("\n");

    printf("Iterator: ");
    dyn_array_iterator_t iterator = dyn_array_iterator_new((void **) &ints);
    void* current;

    while((current = dyn_array_iterator_next(&iterator))) {
        printf("%d ", *(int*)current);
    }

    printf("\n");

    printf("Iterator: ");
    while((current = dyn_array_iterator_next(&iterator))) {
        printf("%d ", *(int*)current);
    }

    printf("\n");

    dyn_array_iterator_reset(&iterator);
    printf("Iterator after reset: ");
    while((current = dyn_array_iterator_next(&iterator))) {
        printf("%d ", *(int*)current);
    }

    printf("\n");

//    dyn_array_delete((void **) &ints);

    for (int i = 0; i < dyn_array_length(ints); i++) {
        printf("%d ", ints[i]);
    }

    printf("\n");
    printf("Iterator: ");

    while((current = dyn_array_iterator_next(&iterator))) {
        printf("%d ", *(int*)current);
    }

    printf("\n");

    dyn_array_delete(&ints);
}

// string builder https://nachtimwald.com/2017/02/26/efficient-c-string-builder/
// https://stackoverflow.com/questions/34438357/cmake-one-build-directory-for-multiple-projects
// https://stackoverflow.com/questions/36443085/is-there-a-way-to-work-on-multiple-projects-in-clion

void dtor(void* ptr) {
    printf("Pointer %p with value %d\n", ptr, *(int*)ptr);
}

void* sb_smalloc_allocator(const void* _, size_t size, void* free_ptr) {
    if (free_ptr != NULL) {
        smart_free(free_ptr);
    }

    if (size > 0) {
        return smart_alloc(size, (void (*)(void *)) &string_builder_destroy);
    }

    return NULL;
}

#define println(format_text, ...) printf(format_text "\n", __VA_ARGS__ )

int main() {
//    vector_main();
//    int a[] = { 1, 2, 3, 4};
//
//    foreach(int* i, a) {
//        printf("%d\n", *i);
//    }

    dyn_array_test();
//    params_data_t p = params(int, 2, 10, 100, 7, 9);
//    params_data_t p = params_safe(int, 2, 10, 100, 7, 9);
//    printf("Params in main\nAdress: %p\n Adress of data: %p\n", refof p, p.data);
    printf("Params in Function:\n");
//    params_test(p);
    params_test(params_safe(int, 2, 10, 100, 7, 9));
    memcheck_validate();

    using int* ptr = smart_alloc(sizeof(int), &dtor);
    *ptr = 10;
    using int* ptr2 = smart_alloc(sizeof(int), &dtor);
    *ptr2 = 30;

    const int cap = 300;
//    using void* memory = smart_alloc(cap, NULL);
    void* memory = malloc(cap);
    allocator_t allocator = allocator_new(cap, memory);

    scope((allocator_delete(&allocator), free(memory))) {
        printf("String Builer\n=================\n");
//    string_builder_t* builder = string_builder_new(10, malloc_allocation_strategy());
        string_builder_t* builder = string_builder_new(10, linear_allocator_allocation_strategy(&allocator));
//    string_builder_t* builder = string_builder_new(10, (allocation_strategy_t) {
//        .allocator = refof allocator,
//        .allocation_callback = refof allocator_allocator
//    });

        using_string_builder(builder) {
            string_builder_add_string(builder, "Hello ", 0);
            string_builder_add_string(builder, "Darkness my old Friend", 0);

            size_t builder_length = string_builder_length(builder);
            STRING_BUILDER_STRING(text, builder);
            string_builder_to_string(builder, text, builder_length);
            println("String: %s", text);
            printf("Length: %ld\n", strlen(text));
        }
    }

//
//    allocator_t arena = allocator_new(300);
//
////    string_builder_t* builder = string_builder_new(10, (allocation_wrapper_t) {
////            .allocator = &arena,
////            .allocation_callback = &allocator_allocator
////    });
//
//    string_builder_add_string(builder, "Hello ", 0);
//    string_builder_add_string(builder, "Darkness my old Friend", 0);
////    string_builder_add_string(builder, "my ", 0);
////    string_builder_add_char(builder, 'm');
////    string_builder_add_char(builder, 'y');
////    string_builder_add_char(builder, ' ');
////    string_builder_add_string(builder, "old ", 0);
////    string_builder_add_string(builder, "Friend", 0);
//
//    size_t builder_length = string_builder_length(builder);
//    printf("Stringbuilder Length: %ld\n", builder_length);
////    char text[builder_length + 1];
//    STRING_BUILDER_STRING(text, builder);
//    string_builder_to_string(builder, text, sizeof(text)/ sizeof(char) - 1);
////    char text2[builder_length + 1];
//    char text2 BUFFER_LENGTH_FOR_STRING_BUILDER(builder);
//    string_builder_to_string(builder, text2, builder_length);
////    printf("String: %s\n", text);
//    println("String: %s", text);
//    printf("String2: %s\n", text2);
//    printf("String3 %s" "\n" "\n" "\n", text);
//    printf("Length: %ld\n", strlen(text));
//
//    string_builder_destroy(builder);
//
//    allocator_delete(&arena);

    return 0;
}
