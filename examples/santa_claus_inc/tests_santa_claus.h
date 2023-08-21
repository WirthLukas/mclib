//
// Created by lukew on 01.01.2023.
//

#ifndef MODERNC_TESTS_SANTA_CLAUS_H
#define MODERNC_TESTS_SANTA_CLAUS_H

#include "child_data_mgmt.h"
#include "tour_plan.h"
#include "../../mclib/utest.h"
#include "../../mclib/dynarray.h"
#include "../../mclib/macros.h"
#include <string.h>

typedef struct Dummy{
    int val;
} Dummy;

// DYN ARRAY Tests
TEST(create_list)
{
    void** ints = dyn_array_init(params(void*));
    ASSERT_FALSE(ints == NULL, "List has to be created");
    dyn_array_delete((void **) &ints);
}

TEST(list_add_elements)
{
    Dummy* list = dyn_array_init(params(Dummy, {1},{2}));
    Dummy dummies[] = {{3}};

    using_dyn_array(list) {
        dyn_array_add(refof list, refof dummies[0]);

        ASSERT_EQUALS_DECIMAL(1, list[0].val, "");
        ASSERT_EQUALS_DECIMAL(2, list[1].val, "");
        ASSERT_EQUALS_DECIMAL(3, list[2].val, "");
        ASSERT_TRUE(dyn_array_length(list) == 3, "Last node has a null pointer as next");
    }
}

TEST(list_add_element_after)
{
    Dummy* list = dyn_array_init(params(Dummy, { 1 }));

    Dummy dummies[] = {
            {1},
            {2},
            {3}
    };

    dyn_array_insert_one_at((void **) &list, 1 , &dummies[1]);
    dyn_array_insert_one_at((void **) &list, 2 , &dummies[2]);
    Dummy first_dummy = list[0];
    Dummy second_dummy = list[1];
    Dummy third_dummy = list[2];

    ASSERT_EQUALS_DECIMAL(1, first_dummy.val, "");
    ASSERT_EQUALS_DECIMAL(2, second_dummy.val, "");
    ASSERT_EQUALS_DECIMAL(3, third_dummy.val, "");
    ASSERT_TRUE(dyn_array_length(list) == 3, "Last node has a null pointer as next");

    dyn_array_delete((void **) &list);
}

// ChildDataMgmt Tests
TEST(create_cdm)
{
    ChildDataMgmt *cdm = cdm_create();
    ASSERT_FALSE(cdm == 0, "ChildDataMgmt has to be created");
    cdm_delete(cdm);
}

TEST(cdm_add)
{
    ChildDataMgmt *cdm = cdm_create();

    ChildData child_infos[] = {
            {"TestCity1", "Tester1", 0.9},
            {"TestCity2", "Tester2", 0.6},
            {"TestCity1", "Tester3", 0.3}
    };
    cdm_add_data(cdm, &child_infos[0]);
    cdm_add_data(cdm, &child_infos[1]);
    cdm_add_data(cdm, &child_infos[2]);
    ChildData first_child = cdm->data[0];
    ChildData second_child = cdm->data[1];
    ChildData third_child = cdm->data[2];

//    ASSERT_TRUE(strcmp("Tester1", first_child.name) == 0, "");
//    ASSERT_TRUE(strcmp("TestCity1", first_child.city) == 0, "");
//    ASSERT_TRUE(strcmp("Tester3", second_child.name) == 0, "");
//    ASSERT_TRUE(strcmp("TestCity1", second_child.city) == 0, "");
//    ASSERT_TRUE(strcmp("Tester2", third_child.name) == 0, "");
//    ASSERT_TRUE(strcmp("TestCity2", third_child.city) == 0, "");
//    ASSERT_TRUE(dyn_array_length(cdm->data) == 3, "Only three elements in the list");
    ASSERT_EQUALS_STR("Tester1", first_child.name, "");
    ASSERT_EQUALS_STR("TestCity1", first_child.city, "");
    ASSERT_EQUALS_STR("Tester3", second_child.name, "");
    ASSERT_EQUALS_STR("TestCity1", second_child.city, "");
    ASSERT_EQUALS_STR("Tester2", third_child.name, "");
    ASSERT_EQUALS_STR("TestCity2", third_child.city, "");
    ASSERT_EQUALS_DECIMAL(dyn_array_length(cdm->data), 3, "Only three elements in the list");

    cdm_delete(cdm);
}

//TourPlan Tests
TEST(plan_calc_presents_test)
{
    ASSERT_TRUE(0 == plan_calc_presents(-312.8), "");
    ASSERT_TRUE(0 == plan_calc_presents(0.0), "");
    ASSERT_TRUE(0 == plan_calc_presents(0.1), "");
    ASSERT_TRUE(0 == plan_calc_presents(0.2), "");
    ASSERT_TRUE(0 == plan_calc_presents(0.3), "");
    ASSERT_TRUE(0 == plan_calc_presents(0.4), "");
    ASSERT_TRUE(0 == plan_calc_presents(0.5), "");
    ASSERT_TRUE(1 == plan_calc_presents(0.6), "");
    ASSERT_TRUE(2 == plan_calc_presents(0.7), "");
    ASSERT_TRUE(3 == plan_calc_presents(0.8), "");
    ASSERT_TRUE(4 == plan_calc_presents(0.9), "");
    ASSERT_TRUE(5 == plan_calc_presents(1.0), "");
    ASSERT_TRUE(5 == plan_calc_presents(6.2), "");
}

TEST(plan_create_plan)
{
    ChildDataMgmt *cdm = cdm_create();
    ChildData child_infos[] = {
            {"TestCity1", "Tester1", 0.9},
            {"TestCity2", "Tester2", 0.6},
            {"TestCity1", "Tester3", 0.3}
    };
    cdm_add_data(cdm, &child_infos[0]);
    cdm_add_data(cdm, &child_infos[1]);
    cdm_add_data(cdm, &child_infos[2]);

    TourPlan *plan = plan_createTour(cdm);
    ASSERT_FALSE(plan == 0, "TourPlan has to be created");
    ASSERT_TRUE(3 == plan->total_children, "");
    ASSERT_TRUE(5 == plan->total_presents, "");

    CityPlan* city_plans = plan->city_plans;
    CityPlan first_city = city_plans[0];
    CityPlan second_city = city_plans[1];
    ASSERT_TRUE(dyn_array_length(city_plans) == 2, "Only two cities are in the plan");

    ASSERT_TRUE(strcmp("TestCity1", first_city.city) == 0, "");
    ChildPlan* child_plans = first_city.child_plans;
    ChildPlan first_child = child_plans[0];
    ChildPlan second_child = child_plans[1];
    ASSERT_TRUE(dyn_array_length(child_plans) == 2, "Only two children in this city");

    ASSERT_TRUE(strcmp("Tester1", first_child.name) == 0, "");
    ASSERT_TRUE(4 == first_child.presents, "");
    ASSERT_TRUE(strcmp("Tester3", second_child.name) == 0, "");
    ASSERT_TRUE(0 == second_child.presents, "");

    ASSERT_TRUE(strcmp("TestCity2", second_city.city) == 0, "");
    child_plans = second_city.child_plans;
    first_child = child_plans[0];
    ASSERT_TRUE(dyn_array_length(child_plans) == 1, "Only a single child in this city");
    ASSERT_TRUE(strcmp("Tester2", first_child.name) == 0, "");
    ASSERT_TRUE(1 == first_child.presents, "");

    plan_delete(plan);
    cdm_delete(cdm);
}

#endif //MODERNC_TESTS_SANTA_CLAUS_H
