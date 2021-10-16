#include "utils.h"
#include <stdio.h>
#include <stddef.h>

size_t timer_from(int from) {
    size_t counter = 0;
    for (int i = 0; i <= from; ++i) {
        ++counter;
        if (from-i != 0) {
        printf("%i ", from-i);
        } else {printf("%i\n", from-i);}
    }
    return counter;
}


int custom_pow(int base, int power) {
    int result = 1;
    for (int i = 0; i < power; ++i) {
       result *= base;
    }
    return result;
}
