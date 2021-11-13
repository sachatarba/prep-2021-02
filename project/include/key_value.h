#ifndef PROJECT_INCLUDE_KEY_VALUE_H
#define PROJECT_INCLUDE_KEY_VALUE_H

#define BUFFER_SIZE 500
#define FROM 0
#define TO 1
#define DATE 2
#define CONTENT_TYPE 3
#define BOUNDARY 4

#include <string.h>
#include <stdio.h>

typedef struct key_value {
    size_t from_value_len;
    size_t to_value_len;
    size_t date_value_len;
    size_t content_type_value_len;
    size_t boundary_value_len;
    size_t parts;

    char from_value[BUFFER_SIZE * 4];
    char to_value[BUFFER_SIZE * 4];
    char date_value[BUFFER_SIZE * 4];
    char content_type_value[BUFFER_SIZE * 4];
    char boundary_value[BUFFER_SIZE * 4];
} key_value;

key_value values;

void set_value(char *value, size_t KEY);

void print_values(key_value values);

#endif // PROJECT_INCLUDE_KEY_VALUE_H