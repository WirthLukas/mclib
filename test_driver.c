//
// Created by lukew on 27.12.2022.
//

#define MCLIB_DYNARRAY_IMPLEMENTATION
#include "tests/dynarray_tests.h"

int main() {
    TestSuite dyn_array_suite = {
            .tests = (Test[]) {
                ADD_TEST(dyn_array_length_souldbe4),
                ADD_TEST(dyn_array_length_souldbe7) },
            .count = 2,
            .name = "Dyn Array Tests"
    };

    run_tests(dyn_array_suite);
}