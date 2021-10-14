#include "utils.h"

size_t timer_from(unsigned char from) {
    size_t counter=0;
    for (unsigned char i = 0; i <= from; ++i) {
        ++counter;
        printf("%i",i);
    }
    return counter;
}

// TODO: Implement `power of` function

int custom_pow(int base, int power) {
    int result=1;
    for(int i=0; i<power;++i){
       result*=base; 
    }
    return result;
}

