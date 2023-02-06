//
// Created by lukew on 01.01.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tour_plan.h"
#include "../../mclib/dynarray.h"

TourPlan* plan_createTour(ChildDataMgmt *cdm) {
    TourPlan* tp = malloc(sizeof(TourPlan));
    size_t length = dyn_array_length(cdm->data);
    CityPlan* city_plans = dyn_array_init(params(CityPlan));
    const char* last_city = "";

    for (size_t i = 0; i < length; i++) {
        ChildData data = cdm->data[i];
        int presents = plan_calc_presents(data.goodness);
        tp->total_presents += presents;
        tp->total_children++;

        ChildPlan childPlan = {
                .name = data.name,
                .presents = presents
        };

        if (last_city != data.city) {
            last_city = data.city;
            CityPlan city_plan = {
                    .city = data.city,
                    .child_plans = dyn_array_init(params(ChildPlan, childPlan))
            };
            dyn_array_add((void **) &city_plans, &city_plan);
        } else {
            CityPlan cityPlan = city_plans[dyn_array_length(city_plans) - 1];
            dyn_array_add((void **) &cityPlan.child_plans, &childPlan);
        }
    }

    tp->city_plans = city_plans;
    return tp;
}

void plan_print(TourPlan *plan) {
    printf("Shipping %d presents to %d children!\n", plan->total_presents, plan->total_children);
    printf("***\n");

    for (size_t i = 0; i < dyn_array_length(plan->city_plans); i++) {
        CityPlan city_plan = plan->city_plans[i];
        printf("Shipment to %s\n", city_plan.city);

        for (size_t j = 0; j < dyn_array_length(city_plan.child_plans); j++) {
            ChildPlan child_plan = city_plan.child_plans[j];
            printf("%s: %d presents\n", child_plan.name, child_plan.presents);
        }

        printf("***\n");
    }
}

void plan_delete(TourPlan *plan) {
    for (size_t i = 0; i < dyn_array_length(plan->city_plans); i++) {
        dyn_array_delete((void **) &plan->city_plans[i]);
    }

    dyn_array_delete((void **) &plan->city_plans);
    free(plan);
}

int plan_calc_presents(double goodness) {
    if (goodness < 0.6) {
        return 0;
    } else if (goodness >= 1) {
        return 5;
    } else if (goodness < 1 && goodness >= 0.9) {
        return 4;
    }
    else if (goodness < 0.9 && goodness >= 0.8) {
        return 3;
    }
    else if (goodness < 0.8 && goodness >= 0.7) {
        return 2;
    }
    else if (goodness < 0.7 && goodness >= 0.6) {
        return 1;
    }
}