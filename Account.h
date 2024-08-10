#ifndef Account_H
#define Account_H

#include"Transaction.h"

typedef struct {
    int accountNumber;
    float balance;
    Transaction** transactions; // Array of pointers to transactions
    int transactionCount;
} Account;

#endif // !Account_H
