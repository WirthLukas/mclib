//
// Created by lukew on 17.08.2023.
//

#ifndef MODERNC_ANIMALS_H
#define MODERNC_ANIMALS_H

#include <stdlib.h>
#include "../../mclib/macros.h"

// https://stackoverflow.com/questions/1114349/struct-inheritance-in-c
// http://www.deleveld.dds.nl/inherit.htm

#define pack_animal_body const char* name; int max_speed; int load;
typedef struct pack_animal { pack_animal_body } pack_animal_t;

//pack_animal_t* pack_animal_new(pack_animal_t* const this, const char* name, int max_speed) {
//    *this = (pack_animal_t) {
//        .name = name,
//        .load = 0,
//        .max_speed = max_speed
//    };
//
//    return this;
//}

#define extends(type, fields) union { type base; struct { fields }; };

typedef struct camel {
//    union {
//        pack_animal_t base;
//        struct { pack_animal_body };
//    };
    extends(pack_animal_t, pack_animal_body)

    const char* color;
} camel_t;

//camel_t* camel_new(camel_t* const this, const char* name, int max_speed, const char* color) {
//    pack_animal_new(this->base, name, max_speed);
//    this->color = color;
//    return this;
//}

typedef void (*TestFunc)(void* context);

typedef struct {
    void* context;
    TestFunc func;
} mach_ma_mal;

typedef struct {
    int line;
    int calls;
} data;

void data_func(void* context) {
    data* data = context;
}

void foo() {
//    camel_t* camel = camel_new(malloc(sizeof(camel_t)), "rudi", 100, "red");

    mach_ma_mal m = {
            .func = &data_func
    };

    m.func(m.context);

    camel_t  camel = {
        .max_speed = 10
    };

    camel.load = 10;
    camel.name = "rudi";

    pack_animal_t* base = &camel.base;
}

#endif //MODERNC_ANIMALS_H
