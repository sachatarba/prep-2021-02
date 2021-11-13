#include "utils.h"

int check_str_heading(char *current_str, int *ind_of_end_of_heading)
{

    for (size_t current_sym = 0; current_sym < 500; ++current_sym) {
        if (current_str[current_sym] == ":") {

            ind_of_end_of_heading == current_sym;
            break;
        }
    }

    if (ind_of_end_of_heading >= 0) {
        return 1;
    }

    return 0;
}

void correct_reading(char *current_str, char* value, size_t ind_of_end_of_heading) {
    int is_last_sym_part_of_word = 1;
    char *value[BUFFER_SIZE * 4];
    size_t ind_of_end_of_value = 1;

    for (size_t current_sym = ind_of_end_of_heading + 1; current_sym < strlen(current_str); ++current_sym) {
        size_t value_sym = 0;

        if ((is_last_sym_part_of_word || current_str[current_sym] != ' ') && (current_str[current_sym] != '\r' || current_str[current_sym] != '\n')) {

            value[value_sym] = current_str[current_sym];
            ++value_sym;
            ++ind_of_end_of_value;

            if (current_sym == ' ')
            {
                is_last_sym_part_of_word = 0;
            }
        }
    }
    value[ind_of_end_of_value] = '\0';
}