#include "isprime.h"
#include "stddef.h"



int is_prime(size_t num){
    if(num==0 || num==1 || num==2){return 0;}
    for(size_t i=2;i<num;++i){
        if(num % i == 0){
        return 0;}    
    }
    return 1;
}



