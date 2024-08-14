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
    Account account;
    CreditCard* creditCard;
    NODE headOfLoan; // Linked list of loans
    int customerID;
    char name[100]; // Allocating enough space for the name
    char address[100];
    int loanCount;

} Customer;
void updateCustomer1(Customer* customer);

void createCustomer(Customer* customer);
void showCustomer(Customer* customer);
void freeCustomer(Customer* customer);
void init(Customer* customer);
NODE* addLoan(Customer* customer, Loan* loan);
void printList(Customer customer);


#endif // !Customer_H
