//
// Created by lukew on 01.01.2023.
//

#include "tests_santa_claus.h"
#include "../../mclib/utest.h"

int main() {
    run_tests((TestSuite) {
            .tests = (Test[]){
                    ADD_TEST(create_list),
                    ADD_TEST(list_add_elements),
                    ADD_TEST(list_add_element_after)
            },
            .count = 3,
            .name = "List Tests"
    });

    run_tests((TestSuite) {
       .tests = (Test[]){
               ADD_TEST(create_cdm),
               ADD_TEST(cdm_add)
       },
       .count = 2,
       .name = "ChildDataMgmt Tests"
    });

    run_tests((TestSuite) {
            .tests = (Test[]){
                    ADD_TEST(plan_calc_presents_test),
                    ADD_TEST(plan_create_plan)
            },
            .count = 2,
            .name = "Tour Plan Tests"
    });
}
