# mclib - Dynamic Array

This is a generic implementation of a growable array/vector. If the storage of the array reaches its end, the
whole array gets resized and further elements can be added. This is an equivalent to `std::vector` in C++ or
`System.Collections.Generic.List` in C#.

## Overview

The Dynamic Array is just a simple array located somewhere on the Heap, which stores a header in front of 
its elements. The Header contains meta information like the length of the array or the size of the included
item blocks (due to it works with `void*` pointers to get generic like behaviours in C).

![Array Representation in Memory](img/DynamicArray_Picture_1.png)

The returned pointer points directly to the first element of the array. Therefore, it can be passed to
any other C function, that works with arrays, because the normal pointer/array arithmetic works just like
normal array arithmetic: `array_ptr[index] => *(array_ptr + index * size_of_block)`. The returned pointer
should not be changed, because we would loose access to the array's header and the allocated memory block
could not be freed correctly.

![Visualization of the inner Workings](img/DynamicArray_Picture_2.png)

## Usage

Be aware of that some methods need the address of your array pointer (add, insert, remove, ...).
If you just provide the array pointer to the function it might result in a `Segmentation Fault`!!!
(See Implementation Details).

### Import

This is a Single-Header-File Library. So you have to define the Header's Implementation in one of your
project's `.c` files (the best option is a file which does not change very often). This can be achieved
by defining following macro:

```c
#define MCLIB_DYNARRAY_IMPLEMENTATION
#include "mclib/dynarray.h"
```

In every other file you can import the header by just using the include:
```c
#include "mclib/dynarray.h"
```

### Creation

The datatype of the Dynamic Array is the same as a normal array (e.g. `int*`, `char*` or `person_t*`).
But you can also use the dyn_array_t() macro to make clear that this is a dynamic array.

The Dynamic Array Library makes use of the Params Library (for variadic parameters). Check it out to get
more clear about that. Through importing `dynarray.h` you have automatically access to `params.h`!

```c
// Create new dynamic Array
dyn_array_t(int) ints = dyn_array_init(params(int, 10, 20, 30));
// or, because dyn_array_t() is just a function-like macro
int* ints = dyn_array_init(params(int, 10, 20, 30));

// Create empty array
dyn_array_t(int) ints = dyn_array_init(params(int));
```

### Finalize

Because the elements are managed on the heap behind the scenes, the Dynamic Array has to be freed after using it!

```c
// Free the memory of the array after using it
dyn_array_delete(&ints);
```

### Add

There are several options for adding one or more elements. The Array automatically resizes itself if there is
not enough space for the elements.

```c
int a = 10;
dyn_array_add(&ints, &a);   // adding one element has to be done with a pointer due to void*

dyn_array_add_many(&ints, params(int, 40, 50, 60, 70, 80, 90)); // params automatically creates an array out of the given elements

// insert after the given index (must be less than the length of the array)
int value = 45;
dyn_array_insert_one_at(&ints, 4, &value);
dyn_array_insert_many_at(&ints, 4, params(int, 42, 44, 46, 48));
```

### Read/Update

The Dynamic Array acts like a normal array in C. So you can access the items by using the `[]` operator

```c
// Read element of a specific index
int item = ints[3];
ints[4] = 25;

// iterating over the array
for (int i = 0; i < dyn_array_length(ints); i++) {
    printf("%d ", ints[i]);
}

printf("\n");
```

### Remove

When one element gets removd, all elements behind that one gets shifted one forward.

```c
// Delete 3 elements beginning at index 4
dyn_array_delete_many(&ints, 4, 3);

// Delete the element at index 2
dyn_array_delete_one(&ints, 2);
```

### Get Several Meta Information

```c
// returns the actual count of the items in the array (= length)
size_t length = dyn_array_length(ints);

// get the count of how many elements could be stored before the array has to resize
// length <= capacity
size_t capacity = dyn_array_capacity(ints);

// returns the size needed for storing one element
size_t element_size = dyn_array_element_size(ints)
```

### Misc (Using, Iterator)

There is also a using macro for dynamic arrays (see generall language extensions).
With that, the finalizer gets automatically called (like using statements in C#). For more details
read the Macros documentation.

```c
dyn_array_t(int) ints = dyn_array_init(params(int, 10, 20, 30));

using_dyn_array(ints) {
    dyn_array_add_many(&ints, params(int, 40, 50, 60, 70, 80, 90));
    
    for (int i = 0; i < dyn_array_length(ints); i++) {
        printf("%d ", ints[i]);
    }
    
    printf("\n");
}
```

And there is also an iterator for Dynamic Arrays:

```c
dyn_array_iterator_t iterator = dyn_array_iterator_new((void **) &ints);
void* current;

while((current = dyn_array_iterator_next(&iterator))) {
    printf("%d ", *(int*)current);
}

printf("\n");

dyn_array_iterator_reset(&iterator);
```

### Custom Realloc and Free function

By default, the Dynamic Array uses realloc and free from the standard malloc.h library. But through
Macro definition you can tell Dynamic Array to use your custom realloc and free functions. This is possible,
because the through Single-Header-Files the implementation of the header gets inserted into one of your
project's `.c` files by the preprocessor (exactly in that one where you define the MCLIB_DYNARRAY_IMPLEMENTATION macro).
Therefore, it is possible to define certain parts of the source code to be variable. This can be done by using
macros. The programmer can define custom values for these macros and the preprocessor inserts it in the
source code. (read more about that in https://nicolashollmann.de/blog/single-header-libraries/)

Therefore, the Dynamic Array source code uses the `MCLIB_REALLOC` macro for (re)allocation tasks or the
`MCLIB_FREE` macro for freeing allocated memory. You can use your custom realloc and free function
 by defining both macros.

```c
// in the .c file, where you define MCLIB_DYNARRAY_IMPLEMENTATION you can define MCLIB_REALLOC and MCLIB_FREE
// the preprocessor will insert these functions into the source code of dynarray.h (which gets inserted in this file)
#define MCLIB_DYNARRAY_IMPLEMENTATION
#define MCLIB_REALLOC(ptr, size) srealloc(ptr, size, __FILE__, __LINE__)
#define MCLIB_FREE(ptr) sfree(ptr)
#include "mclib/dynarray.h"
```

**But you have to define both!** Otherwise, it will use the standard realloc and free function!

## Implementation Details

### void*

In C each pointer needs a datatype, which describes the size of the memory block to which its points to.
So for example: For `int* ptr` the ptr variable is typically 8 bytes long and points to memory where a int value
is stored. Therefore, the memory block is 4 bytes long. For `char* ptr` the memory block is 1 byte long.

In cases where you don't know the exact type of the pointer you can use `void*` as the datatype, which means
something like `pointer to a currently not known datatype`. So `void` means `no datatype information` in C.

But you can easily cast it to other pointer types:

```c
void* ptr = malloc(4);  // pointer to a 4 bytes long memory block
int number = *(int*)ptr;    // casts the void pointer to an int pointer and reads its value in one line
// So in the above line we tell the program to interpret the memory block of ptr as an integer number

char letter = *(char*)ptr;  // in this case only the first byte of the 4 bytes long memory will be used
// now this can be interpreted as a character or just as 1 byte long memory

float floating_point_number = *(float*)ptr; // now the memory gets interpreted as a floating point value

void value = *ptr;  // this is not possible, due to void is not a usable datatype 

void** ptr_to_ptr = &ptr;
printf("%p\n", *ptr_to_ptr);        // value of void** is possible, because void* is a pointer => contains  anaddress as value
```

In C all pointer gets automatically casted to `void*`. So you can provide your pointer to a function without
the need to cast it:

```c
int n = 10;
int* ptr = &n;
void* generic_ptr = ptr;        // no (void*)ptr needed
void** ptr_to_ptr = (void**) &ptr //here a cast is needed to avoid compiler warnings
```

Therefore, you can simply write `dyn_array_add(&ints, &value)` because the add function makes use of the 
void* type for the pointer to the array pointer.

To avoid `(void**)` cast for every function call, I changed the parameter type to `void*`. Therefore,
you can often find code like that `*(void**)ptr_to_array` in many functions. We want the pointer
to the array pointer (for reallocation, etc.) but we only specified `void*` as the type of ptr_to_array.
So we have to cast it to void** (what should be provided by the function caller) before we can use the 
value of the pointer to a pointer (which is in this case the array pointer of dyn_array_init).

But through that the compiler gives no feedback if the pragramer writes `dyn_array_add(ints, ...` or 
`dyn_array_add(&ints, ...`. But the first version will lead to a segmentation fault in most cases.
So i don't like the solution yet and i am trying to find another one. But so you dont have to cast your 
array to (void**) anytime.

### Array/Pointer Arithmetic

Another complication of void* arrays is, that you can not use the index operator.

```c
int numbers[] = { 10, 20, 30, 40}; //numbers is a pointer to the array on the stack
int* numbers = (int[]) { 10, 20, 30, 40 }; // same meaning as the line above

int item = numbers[2];      // is simple pointer arithmetic behind the scenes
int item = *(number + 2 * sizeof(int)); // same meaning as the line above
```

So to access an element of an array, the c compiler adds just the index to the array pointer. 
So after that we have the memory address of the element we wanted. And now we can just extract the 
value of the memory with the `*`-Operator. But not every datatype needs the equal space on the memory
so the compiler has to consider the datatype size in the calculation. But this happens automatically,
because the compiler knows the datatype and therefore also the size of the datatype. So the generic
formula to calculate the memory address of an element on a given index is `ptr + index * sizeof(datatype)`.

But for void* arrays the compiler does not know the size of the datatype and therefore it calculates with
size equal to 1 Byte (like for chars). So to get the right memory address we have to code the formula 
ourselfs with the correct size of the datatype. And this gets stored in the header when someone creates
a dynamic array with `dyn_array_init` (or more specifically in `dyn_array_grow` behind the scenes).

The following code example shows some calculations:

```c
// same meaning as &arr[index] but raises warning;
void* ptr_to_element = &(arr)[index * header->element_size];
void* ptr_to_element = (arr) + index * header->element_size;    // self implemented calculation

// could be the source code for dyn_array_add
memcpy(p_to_element, value, header->element_size);
header->length++;
```

### memcpy

We don't get the value as an argument to the add function. We get the pointer to the value as void*.
Therefore, we can't simply store the address of the value (could be deletet due to its mabye on the stack).
And we can't have access because we don't get the value of a void pointer and we can't cast it, because in
add we don't know the datatype.

So we have to copy byte by byte the information of the memory where the value is stored to our array.
This is a common task when you work with void* pointers and therefore when you want to implement a
generic datastructure in C.

```c
// this is the copy process of the value memory to the end of the array memory after 
// it was ensured that the array provides enough space for the new added value
// and in this line the length of the array gets automatically increased by one, due to the ++ operator
memcpy(*(void**)ptr_to_array + header->length++ * header->element_size, value, header->element_size);
```

### Other Implementation possibilities for Dyn Array Functions

It would also be possible to use macros for each function of Dynamic Array, which uses "generic" parameters.
For example the following add implementation:

```c
#define dyn_array_add(arr, element) dyn_array_ensure_capacity((arr), 1); (*(arr))[dyn_array_header(*(arr))->length++] = (element);
```

Because the preprocessor copies the code to the caller function, we can use the `[]`-operator, because
at this context the compiler knows the concrete type of the array. But I wanted to challenge myself and
tried to implement the functions with void* parameters. And I thought it would be more secure.

### Growth Algorithm

A common approach is to double the capacity (= the whole allocated memory) of the array, when the space 
reaches the end. So the capacity is always a 2^n value and the malloc (or realloc) calls are reduced compared
to always increase the size by one for each adding of new elements.

So the following line works just fine for increasing the capacity for adding one element.
If the capacity is 0 the Default Capacity will be used (this scenario happens in dyn_array_init).

```c
size_t new_capacity = capacity > 0 ? 2 * capacity : DYN_ARRAY_DEFAULT_CAPACITY;
```

But the problem happens in add_many. Imagine the scenario, where you have 4 elements stored in the 
dynamic array. And then you add 5 further elements through (add_many). The capacity gets doubled due to 
our algorithm described above. Then the capacity of the array is 8, but 4 elements are currently stored.
So by adding our 5 new elements we get out of the array's range. And this could lead to some strange
malloc errors like: https://stackoverflow.com/questions/2987207/why-do-i-get-a-c-malloc-assertion-failure

So we have to double the array's capacity as long as the space is not enough for all elements added by 
add_many. This could be done by a simple while loop:

```c
size_t new_capacity = capacity > 0 ? capacity : DYN_ARRAY_DEFAULT_CAPACITY;

while (new_capacity < needed_length) {
    new_capacity = new_capacity * 2;
}
```

And here we come to a nice and simple example where math can help the programmer a lot and optimizes the
source code. By describing our logic into a mathematical formula and reshape it a little we get the exact
power of two, which has to be multiplied with capacity

```
// the loop above can be ommitted by simple math calculations
// this is a formula that calculates how often we have to double capacity to fit needed length
// this can be recreated by starting with the equation:
needed_length < x * 2 * capacity

// reshaping:
needed_length < x * 2 * capacity   / (2* capacity)  // can never be less than zero because capacity is alwas at least Default_Capacity
needed_length / (2*capacity) < x

// results in: needed_length / (2 * capacity) < x
// => therefore each number greater than needed_length / (2 * capacity) can be used
// through the integer division: adding 1 is the perfect number for x
```

The above calculation could be coded as:

```c
size_t new_capacity = capacity > 0 ? (needed_length / (2 * capacity) + 1) * 2 * capacity : DYN_ARRAY_DEFAULT_CAPACITY;

// and finally the reallocation of the array
void* new_arr = MCLIB_REALLOC(arr ? dyn_array_header(arr) : NULL, element_size * new_capacity + sizeof(dyn_array_header_t));
```

### Storing a Header in front of the data

We store a header with emta information of the array in front of its values. This needs some consideration
when allocating the array.

![Visualization of the inner Workings](img/DynamicArray_Picture_2.png)

This can be seen in following code snipped and is part of dyn_array_grow:

```c
// resize the array with realloc
// size = number of element * size of elements (the array part of the memory) + sizeof(header)
void* new_arr = MCLIB_REALLOC(arr ? dyn_array_header(arr) : NULL, element_size * new_capacity + sizeof(dyn_array_header_t));

// then we move the pointer (= the address stored in new_arr) so that it points to the actual data
// therefore the array gets casted to a char pointer so that the compiler calculates with 1 byte
// (could be omitted due to void* is also calculated with 1 byte)
// and we move the pointer to the end of the header (= adding the size of the header)
new_arr = (char *)new_arr + sizeof(dyn_array_header_t);

// the above operation gets simply reverted in dyn_array_header
dyn_array_header_t* header = dyn_array_header(new_arr);

if (arr == NULL) {
    header->length = 0;
    header->element_size = element_size;
}

header->capacity = new_capacity;
return new_arr;
```

## Resources and Inspirations

### Slides of the Modern C Talk
* https://accu.org/conf-docs/PDFs_2021/luca_sass_modern_c_and_what_we_can_learn_from_it.pdf

### Various Dynamic Array implementations
* https://github.com/tezc/sc
* https://github.com/happyfish100/libfastcommon
* https://github.com/nothings/stb
* https://github.com/eignnx/dynarray
* https://github.com/michaelg29/yt-challenges/tree/master/22%20-%20Generic%20Lists