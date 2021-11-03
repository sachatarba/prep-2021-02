#include "is_prime.h"


int is_prime(int number) {
    if (number < 2) {
        return 0;
    }
    int is_prime = 1;
    for (int delimiter = 2; delimiter < number; ++delimiter) {
        if (number % delimiter == 0) {
            is_prime = 0;
            break;
        }
    }
    return is_prime;
}



