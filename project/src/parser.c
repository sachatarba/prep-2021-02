#define BOUNDARY 4

#include "parser.h"
#include "key_value.h"
#include "utils.h"

char* emails_parser(const char *path_to_eml) {
    FILE *email = fopen(path_to_eml, 'r');
    
    if (email == NULL) {
        return NULL;
    }

    const size_t KEYS_COUNTER = 4;
    char *KEYS[KEYS_COUNTER] = {"From:", "To:", "Date:", "Content-Type:"};
    int is_last_str_value = 0;
    while (!feof(email)) {
        char current_str[BUFFER_SIZE];
        fgets(current_str, BUFFER_SIZE, email);
        char *value[BUFFER_SIZE * 4];
        size_t ind_of_end_of_heading = 0;
        size_t KEY;

        if (check_str_heading(current_str, &ind_of_end_of_heading))  {
            // size_t len_of_heading = ind_of_end_of_heading + 1;
            // char heading[len_of_heading + 1];
            // strncpy(heading, current_str, len_of_heading);
            // for (size_t current_sym = 0; current_sym < ind_of_end_of_heading; ++current_sym) {
            //    heading[current_sym] = current_str[current_sym];
            // }
            
            for (size_t current_KEY = 0; current_KEY < KEYS_COUNTER; ++current_KEY) {
                
                if (!strncmp(current_str, *KEYS[current_KEY], ind_of_end_of_heading + 1)) {
                    correct_reading(current_str, value, ind_of_end_of_heading + 1);
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
                    KEY = current_KEY;
                    is_last_str_value = 1;
                } 
            }
        } else if (is_last_str_value) {
            char *add_value[BUFFER_SIZE];
            correct_reading(current_str, add_value, ind_of_end_of_heading);
            strcat(value, add_value);
            set_value(value, KEY);
        }
        
    }
    print_values(values);
    fclose(email);
}