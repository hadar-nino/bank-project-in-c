#ifndef Customer_H
#define Customer_H
#include "Loan.h"
#include"Account.h"
#include"CreditCard.h"


// Node
typedef struct node {
    Loan* key;
    struct node* next;
} NODE;

typedef struct Customer {

    NODE headOfLoan; // Linked list of loans
    int customerID;
    char name[100]; // Allocating enough space for the name
    char address[100];
    Account* accounts; // Array of Account structures
    int accountCount;
    CreditCard* creditCards; // Array of CreditCard structures
    int creditCardCount;
} Customer;

void freeSupermarketManager(Customer* customer);
void L_init(Customer* customer);


#endif // !Customer_H
