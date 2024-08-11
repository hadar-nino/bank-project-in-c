#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Loan.h"
#include"Account.h"
#include"CreditCard.h"
#include"Customer.h"

// Function implementations
void L_init(Customer * customer) {
    if (customer->headOfLoan.key == NULL) {
        printf("No list to initialize");
        return;
    }
    customer->headOfLoan.next = NULL;
    customer->headOfLoan.key = NULL;
}

void freeSupermarketManager(Customer* customer) {
    NODE* current = customer->headOfLoan.next; // Start from the first node
    NODE* next;

    while (current != NULL) {
        next = current->next; // Save the next node
        free(current->key);   // Free the supermarket struct
        free(current);        // Free the node itself
        current = next;       // Move to the next node
    }

    customer->headOfLoan.next = NULL; // Reset the list head to indicate it's empty
}

/*
NODE* addSupermarket(Customer* customer, Loan* loan) {
    NODE* tmp;
    NODE* current;
    if (!customer) return NULL;
    tmp = (NODE*)malloc(sizeof(NODE)); // new node
    tmp->key = (Loan*)malloc(sizeof(Loan));
    strcpy(tmp->key->name, loan->name);
    strcpy(tmp->key->addresss, loan->addresss);
    tmp->key->code = loan->code;
    if (tmp != NULL) {
        // Find the correct position to insert alphabetically by name
        current = &customer->head;
        while (current->next != NULL && _stricmp(current->next->key->name, loan->name) < 0) {
            current = current->next;
        }
        // Insert the new node
        tmp->next = current->next;
        current->next = tmp;
    }
    return tmp;
}

*/