#ifndef PROJECT_INCLUDE_UTILS_H
#define PROJECT_INCLUDE_UTILS_H

#define BUFFER_SIZE 500

#include <stdlib.h>
#include <stddef.h>

int has_str_heading(char* current_str, int *ind_of_end_of_heading);

void correct_reading(char* current_str, char *value, size_t ind_of_end_of_heading);

#endif // PROJECT_INCLUDE_UTILS_H