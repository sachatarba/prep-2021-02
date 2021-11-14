#include "key_value.h"

key_value create_kv(key_value *values) {
    values->has_from_value = 0;
    values->has_to_value = 0;
    values->has_date_value = 0;
    values->has_content_type_value = 0;
    values->has_boundary_value = 0;
    values->parts = 1;
    return *values;
}
void set_value(key_value *values, char *value, size_t KEY) {
    if (value == NULL) {
        return;
    }

    if (KEY == FROM && values->has_from_value == 0) {
        values->has_from_value = 1;
        strcpy(values->from_value, value);

    } else if (KEY == TO && values->has_to_value == 0) {
        values->has_to_value = 1;
        strcpy(values->to_value, value);
    
    } else if (KEY == DATE && values->has_date_value == 0) {
        values->has_date_value = 1;
        strcpy(values->date_value, value);
    
    } else if (KEY == CONTENT_TYPE && values->has_content_type_value == 0) {
        values->has_content_type_value = 1;
        strcpy(values->content_type_value, value);
    
    } else if (KEY == BOUNDARY && values->has_boundary_value == 0) {
        values->has_boundary_value = 1;
        strcpy(values->boundary_value, value);
    }
}

void print_values(key_value values) {
    printf("%s|%s|%s|%i", values.from_value, values.to_value, values.date_value, values.parts);
}
