//
// Created by lukew on 29.12.2022.
//

#ifndef MODERNC_MEMCHECK_H
#define MODERNC_MEMCHECK_H

#include <stdlib.h>
#include <stdbool.h>

#define MAX_ALLOCS 64

void* smalloc(size_t size, char* file, int line);
void* srealloc(void* adress, size_t size, char* file, int line);
void sfree(void* adress);
bool memcheck_validate();

#endif //MODERNC_MEMCHECK_H
