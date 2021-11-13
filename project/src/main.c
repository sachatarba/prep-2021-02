#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

int main(int argc, const char **argv) {
    if (argc != 2) {
        return -1;
    }

    const char *path_to_eml = argv[1];
    puts(path_to_eml);

    emails_parser(path_to_eml);
    return 0;
}
