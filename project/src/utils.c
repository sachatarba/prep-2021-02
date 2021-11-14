#include "utils.h"

int has_str_heading(char *current_str, int *ind_of_end_of_heading, size_t *KEY) {
    const char *KEYS[] = {"From:", "To:", "Date:", "Content-Type:"};
    for (size_t current_KEY = 0; current_KEY < KEYS_COUNTER; ++current_KEY) {
        char* start_of_heading = strstr(current_str,KEYS[current_KEY]);
        if (start_of_heading != NULL) {
            //printf("%c\n", current_str[current_sym]);
            //printf("%s\n", current_str);
            //printf("%zu", current_sym);
            *ind_of_end_of_heading = start_of_heading - current_str + strlen(KEYS[current_KEY]);
            *KEY = current_KEY;
            //printf("header_end: %c%c\n",current_str[*ind_of_end_of_heading], current_str[*ind_of_end_of_heading + 1]);
            break;
        }
    }
    //printf("%i",*ind_of_end_of_heading);
    if (*ind_of_end_of_heading >= 0) {
        return 1;
    }

    return 0;
}

void correct_reading(char *current_str, char* value, int ind_of_end_of_heading) {
    int is_last_sym_part_of_word = 0;
    int ind_of_end_of_value = 0;
    size_t value_sym = 0;
    //printf("%s",current_str);
    for (size_t current_sym = ind_of_end_of_heading + 1; current_sym < strlen(current_str); ++current_sym) {

        if ((is_last_sym_part_of_word || current_str[current_sym] != ' ') 
            && (current_str[current_sym] != '\r' || current_str[current_sym] != '\n')) {
            //printf("%c", current_str[current_sym]);
            value[value_sym] = current_str[current_sym];
            //printf("%c", value[value_sym]);
            ++value_sym;
            ++ind_of_end_of_value;
            //printf("%s", value);
            if (current_sym != ' ') {
                is_last_sym_part_of_word = 1;
            }

            if (current_sym == ' ') {
                is_last_sym_part_of_word = 0;
            }
        }
    }
    value[ind_of_end_of_value - 1] = '\0';
    //printf("%s", value);
}