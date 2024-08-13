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
    printf("\nEnter transaction date (YYYY-MM-DD): ");    // Prompt the user to enter the transaction date
    fgets(date, 11, stdin);
    date[strcspn(date, "\n")] = '\0';  // Remove the newline character if present
    printf("\nEnter transaction amount: ");    // Prompt the user to enter the transaction amount
    scanf("%f", &amount);
    account->balance += amount;
    clearInputBuffer();
    // Reallocate memory for the transactions array to add a new transaction
    Transaction** temp = (Transaction**)realloc(account->transactions, (account->transactionCount + 1) * sizeof(Transaction*));
    if (temp != NULL) {        // Update the transactions pointer to point to the newly allocated memory
        account->transactions = temp;

        // Allocate memory for the new transaction
        account->transactions[account->transactionCount] = (Transaction*)malloc(sizeof(Transaction));

        if (account->transactions[account->transactionCount] != NULL) {
            // Initialize the new transaction with the user-provided date and amount
            strcpy(account->transactions[account->transactionCount]->date, date);
            account->transactions[account->transactionCount]->amount = amount;

            // Increment the transaction count
            account->transactionCount++;

            printf("The transaction was added successfully.\n");
        }
        else {
            printf("Memory allocation for the new transaction failed.\n");
        }
    }
    else {
        printf("Memory allocation failed.\n");
    }
}
