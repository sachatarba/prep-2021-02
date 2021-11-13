#include "key_value.h"

void set_value(char *value, size_t KEY) {
    if (KEY == 0) {
        values.from_value_len = strlen(value);
        strcpy(values.from_value, value);

    } else if (KEY == 1) {
        values.to_value_len = strlen(value);
        strcpy(values.from_value, value);
    
    } else if (KEY == 2) {
        values.date_value_len = strlen(value);
        strcpy(values.date_value, value);
    
    } else if (KEY == 3) {
        values.content_type_value_len = strlen(value);
        strcpy(values.from_value, value);
    
    } else if (KEY == 4) {
        values.boundary_value_len = strlen(value);
        strcpy(values.boundary_value, value);
    }
}

void print_values(key_value values) {
    printf("%s|%s|%s|%i", values.from_value, values.to_value, values.date_value, values.parts);
}
