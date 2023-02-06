//
// Created by lukew on 01.01.2023.
//

#ifndef MODERNC_TOUR_PLAN_H
#define MODERNC_TOUR_PLAN_H

#include "child_data_mgmt.h"

typedef struct ChildPlan {
    const char* name;
    int presents;
} ChildPlan;

typedef struct CityPlan {
    ChildPlan* child_plans;
    const char* city;
} CityPlan;

typedef struct TourPlan {
    CityPlan* city_plans;
    int total_children;
    int total_presents;
} TourPlan;

/**
 * Creates a TourPlan structure
 */
TourPlan* plan_createTour(ChildDataMgmt *cdm);

/**
 * Prints the tour plan to the console
 * (hint: check the assignment description for the proper format)
 */
void plan_print(TourPlan *plan);

/**
 * Frees memory allocated by the plan (and its child elements)
 * (hint: some structure contain lists, you already implemented a list cleanup)
 */
void plan_delete(TourPlan *plan);

/**
 * Calculates the number of presents a kid receives based on how well he or she behaved
 * (hint: look at the test case to understand the calculation)
 */
int plan_calc_presents(double goodness);

#endif //MODERNC_TOUR_PLAN_H
