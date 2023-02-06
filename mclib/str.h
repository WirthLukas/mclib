//
// Created by lukew on 12.01.2023.
//

#ifndef MODERNC_STR_H
#define MODERNC_STR_H

#include "allocator.h"
#include <stddef.h>
#include <stdbool.h>

//typedef struct str {
//    const char* data;
//    size_t size;
//} string;
//
//string str(const char* data);
//bool str_valid(string str);
//bool str_match(string a, string b);
//bool str_contains(string haystack, string needle);
//string str_sub(string src, size_t begin, size_t end);
//string str_find_first(string haystack, string needle);
//string str_find_last(string haystack, string needle);
//string str_remove_prefix(string src, string prefix);
//string str_remove_suffix(string src, string suffix);
//
//string str_pop_first_split(string* src, string split_by);

typedef struct string_builder {
    // current chunk data
    char* data;
    size_t length;
    size_t capacity;

    // data for whole string
    struct string_builder* previous;
    unsigned int offset;

    allocation_strategy_t allocation_strategy;
} string_builder_t;

string_builder_t* string_builder_new(size_t capacity, allocation_strategy_t allocation_strategy);
void string_builder_destroy(string_builder_t* builder);

void string_builder_add_char(string_builder_t* builder, char c);
void string_builder_add_string(string_builder_t* builder, const char* str, size_t len);
size_t string_builder_length(const string_builder_t* builder);
void string_builder_to_string(string_builder_t* builder, char buffer[], size_t length);

#define STRING_BUILDER_STRING(variable_name, builder) char variable_name[string_builder_length(builder) + 1]

#define BUFFER_LENGTH_FOR_STRING_BUILDER(builder) [string_builder_length(builder) + 1]

#include "macros.h"
#define using_string_builder(string_builder) scope(string_builder_destroy(string_builder))

#ifdef MODERNC_STR_IMPLEMENTATION

string_builder_t* string_builder_new(size_t capacity, allocation_strategy_t allocation_strategy) {
//    string_builder_t* builder = calloc(1, sizeof(string_builder_t));

//    string_builder_t* builder = allocation_strategy.allocation_callback(
//            allocation_strategy.allocator,
//            sizeof(string_builder_t),
//            NULL);

    string_builder_t* builder = allocation_strategy_allocate(allocation_strategy, sizeof(string_builder_t));

//    builder->data = malloc(sizeof(char) * capacity);
//    builder->data = allocation_strategy.allocation_callback(
//            allocation_strategy.allocator,
//            sizeof(char) * capacity,
//            NULL);

    builder->data = allocation_strategy_allocate(allocation_strategy, sizeof(char) * capacity);

    builder->capacity = capacity;
    builder->allocation_strategy = allocation_strategy;
    builder->length = 0;
    builder->offset = 0;
    builder->previous = NULL;

    return builder;
}

void string_builder_destroy(string_builder_t* builder) {
    while(builder != NULL) {
        string_builder_t* previous = builder->previous;
//        const void* allocator = builder->allocation_strategy.allocator;
//        allocation_callback allocation_callback = builder->allocation_strategy.allocation_callback;
//        allocation_callback(allocator, 0, builder->data);
//        allocation_callback(allocator, 0, builder);
//        free(builder->data);
//        free(builder);

        allocation_strategy_free(builder->allocation_strategy, builder->data);
        allocation_strategy_free(builder->allocation_strategy, builder);
        builder = previous;
    }
}

void string_builder_grow_by(string_builder_t* builder, size_t capacity) {
//    string_builder_t* new_builder = calloc(1, sizeof(string_builder_t));
//    const void* allocator = builder->allocation_strategy.allocator;
//    allocation_callback allocation_callback = builder->allocation_strategy.allocation_callback;

//    string_builder_t* new_builder = allocation_callback(allocator, sizeof(string_builder_t), NULL);
    string_builder_t* new_builder = allocation_strategy_allocate(builder->allocation_strategy, sizeof(string_builder_t));

    if (new_builder == NULL) {
        // TODO!!!
    }

//    new_builder->data = builder->data;
//    new_builder->capacity = builder->capacity;
//    new_builder->length = builder->length;
//    new_builder->previous = builder->previous;
//    new_builder->offset = builder->offset;
//    new_builder->allocation_callback = builder->allocation_callback;
    memmove(new_builder, builder, sizeof(string_builder_t));

    builder->offset = builder->offset + builder->length;
//    builder->data = malloc(sizeof(char) * capacity);
//    builder->data = allocation_callback(allocator, sizeof(char) * capacity, NULL);
    builder->data = allocation_strategy_allocate(builder->allocation_strategy, sizeof(char) * capacity);
    builder->capacity = capacity;
    builder->previous = new_builder;
    builder->length = 0;
}

void string_builder_add_string(string_builder_t* builder, const char* str, size_t len) {
    if (len <= 0) {
        len = strlen(str);
    }

    if (builder->length + len <= builder->capacity) {
        memcpy(builder->data + builder->length, str, len);
        builder->length += len;
    } else {
        size_t index_str = 0;

//        size_t difference = builder->capacity - builder->length;
//        memcpy(builder->data + builder->length, str, difference);
//        builder->length += difference;

        while (index_str < len) {
            while (builder->length < builder->capacity && index_str < len) {
                builder->data[builder->length] = str[index_str];
                builder->length++;
                index_str++;
            }

            string_builder_grow_by(builder, builder->capacity);
        }

//        memcpy(builder->data + builder->length, str + difference, len - difference);

//        while (builder->length < builder->capacity && index_str < len) {
//            builder->data[builder->length] = str[index_str];
//            builder->length++;
//            index_str++;
//        }
    }
}

void string_builder_add_char(string_builder_t* builder, char c) {
    if (builder->length + 1 <= builder->capacity) {
        builder->data[builder->length++] = c;
    } else {
        string_builder_grow_by(builder, builder->capacity);
        builder->data[builder->length++] = c;
    }
}

size_t string_builder_length(const string_builder_t* builder) {
    return builder->length + builder->offset;
}

void string_builder_to_string(string_builder_t* builder, char buffer[], size_t length) {
    if (length < builder->offset + builder->length) {
        return;
    }

    while (builder != NULL) {
        memcpy(buffer + builder->offset, builder->data, builder->length);
        builder = builder->previous;
    }

    buffer[length] = '\0';
}

#endif // MODERNC_STR_IMPLEMENTATION

#endif //MODERNC_STR_H
