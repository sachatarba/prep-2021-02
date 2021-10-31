#include "print_numbers_from_one.h"
#include <stdio.h>


void print_numbers_from_one(int last_number) {
        if (last_number == 1) {
            printf("%i", last_number);
            return;
        }
        if (last_number > 1) {
            print_numbers_from_one(last_number-1);
        }
        if (last_number < 1) {
            print_numbers_from_one(last_number + 1);
        }
        printf(" %i", last_number);
}
