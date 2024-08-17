#ifndef Account_H
#define Account_H

#include"Transaction.h"

typedef struct {
    float balance;
    Transaction** transactions; // Array of pointers to transactions
    int transactionCount;
} Account;
void updateAccount(Account* account);
void freeAccount(Account* a);

#endif // !Account_H
