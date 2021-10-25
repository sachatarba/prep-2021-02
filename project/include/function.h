#define PROJECT_INCLUDE_FUNCTIONS_H
#ifndef PROJECT_INCLUDE_FUNCTIONS_H
#include <stdio.h>
#include "masterRecord.h"
#include "utils.h"
#include "functions.h"
#define filename "transaction.dat"

void masterWrite(FILE *ofPTR, Data Client);

void transactionWrite(FILE *ofPtr, Data transfer);

void blackRecord(FILE *ofPTR, FILE *ofPTR_2, FILE *blackrecord, Data client_data, Data transfer)

#endif //PROJECT_INCLUDE_FUNCTIONS_H