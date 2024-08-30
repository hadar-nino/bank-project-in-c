
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
    myGets(customer->name, sizeof(customer->name));
    printf("enter address (the address will be 100 words): ");
    myGets(customer->address, sizeof(customer->address));
    customer->loanCount = 0;
    createCreditCard(customer->creditCard);
}

void createCreditCard(CreditCard* creditCard) {
    printf("enter credit card number (this will be 16 words): ");
    myGets(creditCard->cardNumber, sizeof(creditCard->cardNumber));
    printf("enter credit Card Limit: ");
    scanf("%f", &creditCard->creditLimit);
    while (creditCard->creditLimit<0)
    {
        printf("enter credit Card Limit need to be bigger than zero: ");
        scanf("%f", &creditCard->creditLimit);
    }
    printf("enter credit Card balance: ");
    scanf("%f", &creditCard->balance);
    while (!isValidDate(creditCard->expiryDate))
    {
        printf("enter credit card expiry date (format: YYYY-MM-DD): ");
        myGets(creditCard->expiryDate, sizeof(creditCard->expiryDate));
    }
}


// Function implementations
void init(Customer* customer) {
    customer->headOfLoan.next = NULL;
    customer->headOfLoan.key = NULL;
}

void freeCustomer(Customer* customer) {
    free(customer->creditCard);
    customer->creditCard = NULL;
    freeLoan(customer);
    freeAccount(&customer->account);
}


void addLoan(Customer* customer,const Loan* loan) {
    NODE* tmp=NULL;
    NODE* current=NULL;
    if (!customer)
        return;
    tmp = (NODE*)malloc(sizeof(NODE)); 
    if (tmp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    tmp->key = loan;
    current = &customer->headOfLoan;
    for (int i = 0; i < customer->loanCount; i++) {
        current = current->next;
    }
    
    tmp->next = current->next;
    current->next = tmp;
    customer->loanCount++;
}

void printList(const Customer customer) {
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


void showCreditCard(const CreditCard* creditCard) {
    printf("\nCredit card number: %s, ", creditCard->cardNumber);
    printf("Credit card balance: %.2f, ", creditCard->balance);
    printf("Credit card limit: %.2f, ", creditCard->creditLimit);
    printf("Credit card expiry date: %s\n", creditCard->expiryDate);
}

void showCustomer(const Customer* customer) {
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
    while (!isValidDate(temp->startDate)) {      
        printf("Enter start date (format: YYYY-MM-DD): ");
        myGets(temp->startDate, sizeof(temp->startDate));
    }

    while (!isValidDate(temp->endDate)) {
        printf("Enter end date (format: YYYY-MM-DD): ");
        myGets(temp->endDate, sizeof(temp->endDate));
    }

    if (!isDateBiggerThenOldDate(temp->endDate, temp->startDate))
    {
        free(temp);
        printf("loan failed the start date cant be greater than end date\n");
        return NULL;
    }
    return temp;
}


int isValidDate(const char* date) {
    if (strlen(date) != 10) {
        return 0;
    }
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') {
                return 0;
            }
        }
        else {
            if (!isdigit(date[i])) {
                return 0; 
            }
        }
    }

    
    int year = atoi(&date[0]);
    int month = atoi(&date[5]);
    int day = atoi(&date[8]);

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            daysInMonth[1] = 29;  
        }
    }

    if (day > daysInMonth[month - 1]) {
        return 0;
    }

    return 1;
}

int isDateBiggerThenOldDate(const char* date, const char* date2) {

    int year = atoi(&date[0]);
    int month = atoi(&date[5]);
    int day = atoi(&date[8]);
    int year2 = atoi(&date2[0]);
    int month2 = atoi(&date2[5]);
    int day2 = atoi(&date2[8]);
    if (year < year2) {
        return 0;
    }
    if (year == year2)
    {
        if (month < month2) {
            return 0;
        }
        if (month == month2)
        {
            if (day < day2) {
                return 0;
            }
        }
    }
    return 1;
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
            "[5] exit\n"
            "Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        switch (choice) {
        case 0:
            // Update name
            printf("enter name (the name will be 100 words): ");
            myGets(customer->name, sizeof(customer->name));
            printf("Customer name updated to: %s\n", customer->name);
            break;

        case 1:
            // Update address
            printf("enter address (the address will be 100 words): ");
            myGets(customer->address, sizeof(customer->address));
            printf("Customer address updated to: %s\n", customer->address);
            break;

        case 2:
            updateAccount(&customer->account);
            break;

        case 3:
            // Create new loan
            printf("Creating new loan for customer %s\n", customer->name);
            Loan* loan = NULL;
            loan = createLoan();
            if (loan==NULL)            
                break;           
            addLoan(customer,loan);
            customer->account.balance += loan->amount;
            break;

        case 4:
            createCreditCard(customer->creditCard);
            break;

        case 5:
            return;
        default:
            printf("Bad input, returning to the main menu.\n");
            break;
        }
        showCustomer(customer);
    }
}
