//
// Created by lukew on 27.12.2022.
//

#ifndef MODERNC_DYNARRAY_TESTS_H
#define MODERNC_DYNARRAY_TESTS_H

#include "../mclib/dynarray.h"
#include "../mclib/utest.h"

TEST(dyn_array_length_souldbe4) {
    dyn_array_t(int) ints = dyn_array_init(params(int, 10, 20, 30, 40));
    size_t length = dyn_array_length(ints);
//    length = 3;
    ASSERT_TRUE(length == 4, "Length of dyn_array is not 4");
//    ASSERT_TRUE(ints[0] == 20, "number at index 0 should be 10");
}

TEST(dyn_array_length_souldbe7) {
    dyn_array_t(int) ints = dyn_array_init(params(int, 10, 20, 30, 40));
//    int v = 50;
//    dyn_array_add((void **) &ints, &v);
    dyn_array_add_many((void **) &ints, params(int, 50, 60 ,70));
    size_t length = dyn_array_length(ints);
    ASSERT_TRUE(length == 7, "Length of dyn_array is not 7");
}

#endif //MODERNC_DYNARRAY_TESTS_H
