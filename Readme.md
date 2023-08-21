# mclib

An extending C standard library with more modern approaches

* Single Header Files (SHF) (find out more about that in this blog: https://nicolashollmann.de/blog/single-header-libraries/)
* Error Handling
* Allocation Aware

## Installation

Copy the mclib folder into your project and you can use every header file.
A more detailed description will come soon...

## Features

### Dynamic Arrays (dynarray)

Find out more in the [documentation](docs/DynamicArray.md)

```c
    #define MCLIB_DYNARRAY_IMPLEMENTATION
    #include "mclib/dynarray.h"

    // or simple int* ints = .....
    dyn_array_t(int) ints = dyn_array_init(params(int, 10, 20, 30));

    dyn_array_add_many(&ints, params(int, 40, 50, 60, 70, 80, 90));

    int value = 45;
    dyn_array_insert_one_at(&ints, 4, &value);
    dyn_array_insert_many_at(&ints, 4, params(int, 42, 44, 46, 48));
    
    for   (int i = 0; i < dyn_array_length(ints); i++) {
        printf("%d ", ints[i]);
    }
    
    printf("\n");

    dyn_array_delete(&ints);
```

### Variadic Parameters with automatic length calculation

```c
void params_test(params_data_t data) {
    printf("\nLength: %ld\nelement_size: %ld\n", data.length, data.element_size);
    printf("items: ");

    for (int i = 0; i < data.length; i++) {
        printf("%d ", ((int*)data.data)[i]);
    }

    printf("\n");
}

void main() {
    params_test(params(int, 2, 10, 100, 7, 9));
}
```

### Linear Allocator / Arenas

```c
const int cap = 300;
void* memory = malloc(cap);
arena_t arena = arena_new(cap, memory);

person_t* p = arena_alloc(&arena, sizeof(person_t) * 1);
```

### String Builder

```c
string_builder_t* builder = string_builder_new(10, malloc_allocation_strategy());

using_string_builder(builder) {
    string_builder_add_string(builder, "Hello ", 0);
    string_builder_add_string(builder, "Darkness my old Friend", 0);

    size_t builder_length = string_builder_length(builder);
    STRING_BUILDER_STRING(text, builder);
    string_builder_to_string(builder, text, builder_length);
    println("String: %s", text);
    printf("Length: %ld\n", strlen(text));
}
```

### Unit Tests

```c
TEST(dyn_array_length_souldbe7) {
    dyn_array_t(int) ints = dyn_array_init(params(int, 10, 20, 30, 40));
    dyn_array_add_many((void **) &ints, params(int, 50, 60 ,70));
    size_t length = dyn_array_length(ints);
    ASSERT_TRUE(length == 7, "Length of dyn_array is not 7");
}

int main() {
    TestSuite dyn_array_suite = {
            .tests = (Test[]) {
                ADD_TEST(dyn_array_length_souldbe7) },
            .count = 1,
            .name = "Dyn Array Tests"
    };

    run_tests(dyn_array_suite);
}
```

### Macros and Language "Extensions"

Find out more in the [documentation](docs/Macros.md)

```c
// gets automatically freed at the end of the scope
using int* ptr = smart_alloc(sizeof(int), &dtor);

/// ---------------------------------------------

const int cap = 300;
void* memory = malloc(cap);
arena_t allocator = arena_new(cap, memory);

// automatically calls allocator_delete and free after the source code inside scope has executed
scope((allocator_delete(&allocator), free(memory))) {
    printf("String Builer\n=================\n");
    string_builder_t* builder = string_builder_new(10, arena_allocation_strategy(&allocator));
    
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

// ---------------------

int a;
scanf("%d", refof a);
int p = refof a;
printf("&d\n", valof p);
```

## Example Projects

...can be found in the [examples folder](examples).

## Contribute

Feel free to give me advice on design or implementation decisions or contribute your features :D

## Licence

MIT