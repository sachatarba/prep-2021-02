#include "isprime.h"



int is_prime(int num){
    int counter=0;
    for(int i=2;i<num;++i){
        if(num % i == 0){
        return 0;}    
    }
    return 1;
}

int print_numbers(int n){
    if(n==1){
        printf("%i",n)
    }
    else()
}   
