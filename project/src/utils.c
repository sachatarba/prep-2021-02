#include "utils.h"
#include <stdio.h>
#include <stddef.h>


size_t timer_from(int from) {
    size_t counter = 0;
    for (int i = from; i >= 0; --i) {
        ++counter;
        if (i != 0) {
            printf("%i ", i);
        } else {
            printf("%i\n", i);
        }
    }
    return counter;
}


int custom_pow(int base, int power) {
    if (power < 0) {
        return 0;
    }
    int result = 1;
    for (int i = 0; i < power; ++i) {
       result *= base;
    }
    return result;
}
