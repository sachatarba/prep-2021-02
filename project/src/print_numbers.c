#include "print_numbers.h"
#include <stdio.h>


void print_numbers(int last_number) {
        if (last_number == 1) {
            printf("%i", last_number);
        } else {
            if (last_number > 1) {
                print_numbers(last_number-1);
            }
            if (last_number == -1) {
                    printf("%i %i", last_number+2, last_number+1);
            }
            if (last_number < -1) {
                print_numbers(last_number + 1);
            }
            printf(" %i", last_number);
        }
}
