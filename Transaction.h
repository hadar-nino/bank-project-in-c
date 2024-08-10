#ifndef Transaction_H
#define Transaction_H

typedef struct {
    int transactionID;
    char date[11];
    float amount;
    int accountID;
} Transaction;

#endif // !Transaction_H