#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
#include"Transaction.h"
#include"Account.h"


void updateAccount(Account* account) {
    float amount;
    char date[11];
    while (!isValidDate(date)) {
        printf("\nEnter transaction date (YYYY-MM-DD): ");    
        myGets(date, sizeof(date));
    }
    printf("\nEnter transaction amount: "); 
    scanf("%f", &amount);
    account->balance += amount;
    clearInputBuffer();

    // Reallocate memory for the transactions array to add a new transaction
    Transaction** temp = (Transaction**)realloc(account->transactions, (account->transactionCount + 1) * sizeof(Transaction*));

    if (temp == NULL) {      
        printf("Memory allocation failed.\n");
        return;
    }

    account->transactions = temp;
    // Allocate memory for the new transaction
    account->transactions[account->transactionCount] = (Transaction*)malloc(sizeof(Transaction));

    if (account->transactions[account->transactionCount] == NULL) {
        printf("Memory allocation for the new transaction failed.\n");
        return;
    }
    // Initialize the new transaction with the user-provided date and amount
    strcpy(account->transactions[account->transactionCount]->date, date);
    account->transactions[account->transactionCount]->amount = amount;

    account->transactionCount++;    // Increment the transaction count
    printf("The transaction was added successfully.\n");
}

void freeAccount(Account* a)
{
    for (int i = 0; i < a->transactionCount; i++)
    {
        free(a->transactions[i]);
        a->transactions[i] = NULL;
    }
    free(a->transactions);
    a->transactions = NULL;
}

