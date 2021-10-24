#include "masterRecord.h"
#include <stdio.h>


void masterWrite(FILE * ofPTR, Data Client);
void transactionWrite(FILE * ofPTR, Data transfer);
void blackRecord(FILE * ofPTR, FILE * ofPTR_2, FILE * blackrecord, Data client_data, Data transfer);