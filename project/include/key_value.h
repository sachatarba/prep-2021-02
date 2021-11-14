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
    int has_from_value;
    int has_to_value;
    int has_date_value;
    int has_content_type_value;
    int has_boundary_value;
    int parts;

    char from_value[BUFFER_SIZE * 4];
    char to_value[BUFFER_SIZE * 4];
    char date_value[BUFFER_SIZE * 4];
    char content_type_value[BUFFER_SIZE * 4];
    char boundary_value[BUFFER_SIZE * 4];
} key_value;

key_value create_kv(key_value *values);

void set_value(key_value *values, char *value, size_t KEY);

void print_values(key_value values);

#endif // PROJECT_INCLUDE_KEY_VALUE_H