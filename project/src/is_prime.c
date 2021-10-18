#include "is_prime.h"
#include <stddef.h>


int is_prime(size_t number) {
    if (number < 2) {
        return 0;
    }
    for (size_t i = 2; i < number; ++i) {
        if (number % i == 0) {
            return 0;
        }
    }
    return 1;
}



