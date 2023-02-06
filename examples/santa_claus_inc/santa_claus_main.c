#include "child_data_mgmt.h"
#include "tour_plan.h"

#include "../../mclib/macros.h"

int main() {
//    ChildDataMgmt *cdm = cdm_create();
//
//    ChildData child_infos[] = {
//            {"Linz", "Daniel", 0.9},
//            {"Leonding", "Susi", 0.4},
//            {"Leonding", "Herbert", 0.7},
//            {"Linz", "Maria", 0.8},
//            {"Traun", "Horst", 0.2},
//            {"Leonding", "Peter", 1.0}
//    };
//
//    cdm_add_data(cdm, &child_infos[0]);
//    cdm_add_data(cdm, &child_infos[1]);
//    cdm_add_data(cdm, &child_infos[2]);
//    cdm_add_data(cdm, &child_infos[3]);
//    cdm_add_data(cdm, &child_infos[4]);
//    cdm_add_data(cdm, &child_infos[5]);
//
//    TourPlan *plan = plan_createTour(cdm);
//    plan_print(plan);
//    plan_delete(plan);
//
//    cdm_delete(cdm);

    ChildDataMgmt *cdm;

    defer(cdm = cdm_create(), cdm_delete(cdm)) {
        ChildData child_infos[] = {
                {"Linz", "Daniel", 0.9},
                {"Leonding", "Susi", 0.4},
                {"Leonding", "Herbert", 0.7},
                {"Linz", "Maria", 0.8},
                {"Traun", "Horst", 0.2},
                {"Leonding", "Peter", 1.0}
        };

        cdm_add_data(cdm, &child_infos[0]);
        cdm_add_data(cdm, &child_infos[1]);
        cdm_add_data(cdm, &child_infos[2]);
        cdm_add_data(cdm, &child_infos[3]);
        cdm_add_data(cdm, &child_infos[4]);
        cdm_add_data(cdm, &child_infos[5]);

        TourPlan *plan = plan_createTour(cdm);

        scope(plan_delete(plan))
            plan_print(plan);
    }
}