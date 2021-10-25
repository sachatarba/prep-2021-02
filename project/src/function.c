#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "masterRecord.h"
#include "function.h"

void transactionWrite(FILE *ofPtr, Data transfer) {
    printf("%s\n%s\n",
           "1 Number account: ",
           "2 Client cash payments: ");
    while (scanf("%d %lf", &transfer.Number, &transfer.cash_payments) != -1) {
        fprintf(ofPtr, "%-3d%-6.2f\n", transfer.Number, transfer.cash_payments);
        printf("%s\n%s\n",
               "1 Number account:",
               "2 Client cash payments:");
    }
}
void blackRecord(FILE *ofPTR, FILE *ofPTR_2, FILE *blackrecord, Data client_data, Data transfer) {
    while (fscanf(ofPTR, "%d%s%s%s%s%lf%lf%lf", &client_data.Number, client_data.Name, client_data.Surname, client_data.addres, client_data.TelNumber, &client_data.indebtedness, &client_data.credit_limit, &client_data.cash_payments) != -1) {
        while (fscanf(ofPTR_2, "%d %lf", &transfer.Number, &transfer.cash_payments) != -1) {
            if (client_data.Number == transfer.Number && transfer.cash_payments != 0) {
                client_data.credit_limit += transfer.cash_payments;
            }
        }
        fprintf(blackrecord, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", client_data.Number, client_data.Name, client_data.Surname, client_data.addres, client_data.TelNumber, client_data.indebtedness, client_data.credit_limit, client_data.cash_payments);
        rewind(ofPTR_2);
    }
}