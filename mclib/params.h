//
// Created by lukew on 22.12.2022.
//

#ifndef MODERNC_PARAMS_H
#define MODERNC_PARAMS_H

#include <stddef.h>

typedef struct params_data {
    void* data;
    size_t length;
    size_t element_size;
} params_data_t;

#define TO_ARRAY(type, ...) (type[]) { __VA_ARGS__ }

// by https://gist.github.com/aprell/3722962
#define params(type, ...) \
    (params_data_t){ \
        .data = TO_ARRAY(type, __VA_ARGS__), \
        .element_size = sizeof(type),        \
        .length = sizeof(TO_ARRAY(type, __VA_ARGS__))/sizeof(type) \
    }

#define params_safe(type, ...) ({            \
    type data[] = { __VA_ARGS__ };           \
    (params_data_t) {                        \
        .data = data,                        \
        .element_size = sizeof(type),        \
        .length = sizeof(data)/sizeof(type)  \
    };                                       \
})                                           \


#endif //MODERNC_PARAMS_H
