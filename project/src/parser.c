#define BOUNDARY 4

#include "parser.h"
#include "key_value.h"
#include "utils.h"

void emails_parser(const char *path_to_eml) {
    FILE *email = fopen(path_to_eml, "r");
    
    if (email == NULL) {
        return;
    }
    
    key_value values;
    create_kv(&values);
    size_t TEST_KEY;
    size_t KEY;
    char value[BUFFER_SIZE * 4];

    //char *KEYS[] = {"From:", "To:", "Date:", "Content-Type:"};

    int is_last_str_value = 0;
    while (!feof(email)) {
        char current_str[BUFFER_SIZE];
        fgets(current_str, BUFFER_SIZE, email);
        // printf("%s",current_str);
        int ind_of_end_of_heading = -1;

        if (has_str_heading(current_str, &ind_of_end_of_heading, &KEY))  {
            // size_t len_of_heading = ind_of_end_of_heading + 1;
            // char heading[len_of_heading + 1];
            // strncpy(heading, current_str, len_of_heading);
            // for (size_t current_sym = 0; current_sym < ind_of_end_of_heading; ++current_sym) {
            //    heading[current_sym] = current_str[current_sym];
            // }
            // printf("%s", current_str);
            correct_reading(current_str, value, ind_of_end_of_heading);
            //printf("%s",value);
                    /*int is_last_sym_part_of_word = 1;
                    char *value[500];
                    size_t  ind_of_end_of_value = 1;

                    for (size_t current_sym = ind_of_end_of_heading + 1; current_sym < strlen(current_str); ++current_sym) {
                        size_t value_sym = 0;

                        if ((is_last_sym_part_of_word || current_str[current_sym] != ' ') 
                            && (current_str[current_sym] != '\r' || current_str[current_sym] != '\n')) {
                                
                                value[value_sym] = current_str[current_sym];
                                ++value_sym;
                                ++ind_of_end_of_value;

                                if (current_sym == ' ') {
                                    is_last_sym_part_of_word = 0;
                                }
                        } 
                    }
                    */

            is_last_str_value = 1;

        } else if (is_last_str_value && !has_str_heading(current_str, &ind_of_end_of_heading, &TEST_KEY)) {
            char add_value[BUFFER_SIZE];
            correct_reading(current_str, add_value, ind_of_end_of_heading);
            strcat(value, add_value);
            //printf("%s",value);
            set_value(&values, value, KEY);
        } else {
            set_value(&values, value, KEY);
        }
            
    }
    print_values(values);
    fclose(email);
}