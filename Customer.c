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

void createCustomer(Customer* customer) {
    printf("enter name (the name will be 100 words)");
    fgets(customer->name, 100, stdin);
    customer->name[strcspn(customer->name, "\n")] = '\0';
    
}

// Function implementations
void init(Customer * customer) {
    customer->headOfLoan.next = NULL;
    customer->headOfLoan.key = NULL;
}

void freeCustomer(Customer* customer) {
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


NODE* addLoan(Customer* customer, Loan* loan) {
    NODE* tmp;
    NODE* current;
    if (!customer) return NULL;
    tmp = (NODE*)malloc(sizeof(NODE)); // new node
    if (tmp != NULL) {
        tmp->key = (Loan*)malloc(sizeof(Loan));
        memcpy(tmp->key, loan, sizeof(Loan));
        current = &customer->headOfLoan;
        while (current->next != NULL) {
            current = current->next;
        }
        // Insert the new node
        tmp->next = current->next;
        current->next = tmp;
    }
    return tmp;
}

void printList(Customer customer) {
    NODE* current = &customer.headOfLoan;
    if (current->next ==NULL)
        printf("the list is empty");
    while (current->next != NULL) {
        showLoan(current->next->key);
        current = current->next;
    }
}
