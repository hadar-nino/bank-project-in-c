
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
    printf("enter name (the name will be 100 words): ");
    fgets(customer->name, 100, stdin);
    customer->name[strcspn(customer->name, "\n")] = '\0';
    printf("enter address (the name will be 100 words): ");
    fgets(customer->address, 100, stdin);
    customer->address[strcspn(customer->address, "\n")] = '\0';
    customer->loanCount = 0;
    createCreditCard(customer->creditCard);
}

void createCreditCard(CreditCard* creditCard) {
    int limit, balance;
    printf("enter credit card number (this will be 16 words): ");
    fgets(creditCard->cardNumber, 16, stdin);
    creditCard->cardNumber[strcspn(creditCard->cardNumber, "\n")] = '\0';
    creditCard->creditLimit = 1000;
    creditCard->balance = 12345;
    printf("enter credit card expiry date (this will be 6 words): ");
    fgets(creditCard->expiryDate, 6, stdin);
    creditCard->expiryDate[strcspn(creditCard->expiryDate, "\n")] = '\0';
}


// Function implementations
void init(Customer* customer) {
    customer->headOfLoan.next = NULL;
    customer->headOfLoan.key = NULL;
}

void freeCustomer(Customer* customer) {
    if (customer == NULL) return;
    customer->creditCard = NULL;
    freeLoan(customer);
    freeAccount(&customer->account);
}


void addLoan(Customer* customer, Loan* loan) {
    NODE* tmp;
    NODE* current;
    if (!customer)
        return NULL;
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
    customer->account.balance += loan->amount;
    customer->loanCount++;
}

void printList(Customer customer) {
    NODE* current = &customer.headOfLoan;
    if (current->next == NULL)
        printf("the list is empty");
    printf("\n");
    while (current->next != NULL) {
        showLoan(current->next->key);
        current = current->next;
    }
}

void freeLoan(Customer* c){
    NODE* current = c->headOfLoan.next; // Start from the first node
    NODE* next;

    while (current != NULL) {
        next = current->next; // Save the next node
        free(current->key);   // Free the struct
        free(current);        // Free the node itself
        current = next;       // Move to the next node
    }
    c->headOfLoan.next = NULL; // Reset the list head to indicate it's empty
}


void showCreditCard(CreditCard* creditCard) {
    printf("\nCredit card number: %s, ", creditCard->cardNumber);
    printf("Credit card balance: %.2f, ", creditCard->balance);
    printf("Credit card limit: %.2f, ", creditCard->creditLimit);
    printf("Credit card expiry date: %s\n", creditCard->expiryDate);
}

void showCustomer(Customer* customer) {
    printf("Customer name: %s, ", customer->name);
    printf("Customer id: %d, ", customer->customerID);
    printf("Customer balance: %f, ", customer->account.balance);
    printf("Customer address: %s", customer->address);
    showCreditCard(customer->creditCard);
    printf("\nthe list of all loan:\n");
    printList(*customer);
}

Loan* createLoan() {
    float amount, interestRate;
    Loan* temp = malloc(sizeof(Loan));    // Allocate memory for the new Loan
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    printf("Please enter loan amount: ");
    scanf("%f", &amount);
    while (amount <= 0) {
        printf("The loan amount needs to be greater than zero. Please enter again: ");
        scanf("%f", &amount);
    }
    temp->amount = amount;
    printf("Please enter loan interest rate: ");
    scanf("%f", &interestRate);
    while (interestRate < 0) {
        printf("The loan interest rate needs to be greater than or equal to zero. Please enter again: ");
        scanf("%f", &interestRate);
    }
    temp->interestRate = interestRate;
    clearInputBuffer();
    printf("Enter start date (format: YYYY-MM-DD): ");
    fgets(temp->startDate, sizeof(temp->startDate), stdin);
    temp->startDate[strcspn(temp->startDate, "\n")] = '\0';
    printf("Enter end date (format: YYYY-MM-DD): ");
    fgets(temp->endDate, sizeof(temp->endDate), stdin);
    temp->endDate[strcspn(temp->endDate, "\n")] = '\0';
    return temp;
}


void updateCustomer1(Customer* customer) {
    int choice = 0;
    while (choice != 5)
    {
        printf("Update Customer\n"
            "[0] Update name\n"
            "[1] Update address\n"
            "[2] Create new transaction\n"
            "[3] Create new loan\n"
            "[4] Update credit card\n"
            "Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice) {
        case 0:
            // Update name
            printf("enter name (the name will be 100 words): ");
            fgets(customer->name, 100, stdin);
            customer->name[strcspn(customer->name, "\n")] = '\0';
            printf("Customer name updated to: %s\n", customer->name);
            break;

        case 1:
            // Update address
            printf("enter address (the address will be 100 words): ");
            fgets(customer->address, 100, stdin);
            customer->address[strcspn(customer->address, "\n")] = '\0';
            printf("Customer address updated to: %s\n", customer->address);
            break;

        case 2:
            updateAccount(&customer->account);
            break;

        case 3:
            // Create new loan
            printf("Creating new loan for customer %s\n", customer->name);
            addLoan(customer, createLoan());
            break;

        case 4:
            createCreditCard(customer->creditCard);
            break;

        default:
            printf("Bad input, returning to the main menu.\n");
            return;
        }
        showCustomer(customer);
    }
}
