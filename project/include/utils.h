#ifndef PROJECT_INCLUDE_UTILS_H
#define PROJECT_INCLUDE_UTILS_H

#define BUFFER_SIZE 500
#define KEYS_COUNTER 4

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

int has_str_heading(char *current_str, int *ind_of_end_of_heading, size_t *KEY);

void correct_reading(char *current_str, char *value, int ind_of_end_of_heading);

#endif // PROJECT_INCLUDE_UTILS_H