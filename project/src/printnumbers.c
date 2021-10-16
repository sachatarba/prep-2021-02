#include "printnumbers.h"
#include <stdio.h>


void print_numbers(int n) {
        if (n == 1 || n == 0) {
            printf("%i", n);
        } else {
            if (n > 1) {
                print_numbers(n-1);
            }
            if (n <= -1) {
                if (n == -1) {
                    printf("%i ", n+2);
                }
                print_numbers(n+1);
            }
        printf(" %i", n);
        }
}
