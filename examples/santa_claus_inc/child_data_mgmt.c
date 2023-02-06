//
// Created by lukew on 01.01.2023.
//

#include "child_data_mgmt.h"
#define MCLIB_DYNARRAY_IMPLEMENTATION
#include "../../mclib/dynarray.h"
#include <stdbool.h>
#include <string.h>

ChildDataMgmt* cdm_create() {
    ChildDataMgmt* cdm = malloc(sizeof(ChildDataMgmt));
    cdm->data = dyn_array_init(params(ChildData));
    return cdm;
}

void cdm_add_data(ChildDataMgmt *cdm, ChildData *data) {
    size_t length = dyn_array_length(cdm->data);
    bool city_found = false;
    bool end_of_same_city = false;
    size_t index = 0;

    for (; index < length && !city_found && !end_of_same_city; index++) {
        ChildData item = cdm->data[index];

        if (!city_found && strcmp(item.city, data->city) == 0) {
            city_found = true;
        }

        if (city_found && strcmp(item.city, data->city) != 0) {
            end_of_same_city = true;
        }
    }

    dyn_array_insert_one_at(&cdm->data, index, data);
}

ChildData* cdm_get_sorted_data(ChildDataMgmt *cdm) {
    if (dyn_array_length(cdm) == 0) return NULL;

    return &cdm->data[0];
}

/**
 * Used to clean up memory allocated when creating the child management structure
 */
void cdm_delete(ChildDataMgmt *cdm) {
    dyn_array_delete(&cdm->data);
    free(cdm);
}