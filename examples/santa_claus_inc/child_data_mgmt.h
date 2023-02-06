//
// Created by lukew on 01.01.2023.
//

#ifndef MODERNC_CHILD_DATA_MGMT_H
#define MODERNC_CHILD_DATA_MGMT_H

typedef struct ChildData {
    const char* city;
    const char* name;
    double goodness;
} ChildData;

typedef struct ChildDataMgmt {
    ChildData* data;
} ChildDataMgmt;

/**
 * Creates a structure for child data management
 */
ChildDataMgmt* cdm_create();

/**
 * Uses the child management logic to process child data
 * (hint: insert it, sorted by town into the internal list)
 */
void cdm_add_data(ChildDataMgmt *cdm, ChildData *data);

/**
 * Returns a pointer to the first element of the properly sorted list
 * (hint: "sorting" should have already happened when inserting)
 */
ChildData* cdm_get_sorted_data(ChildDataMgmt *cdm);

/**
 * Used to clean up memory allocated when creating the child management structure
 */
void cdm_delete(ChildDataMgmt *cdm);

#endif //MODERNC_CHILD_DATA_MGMT_H
