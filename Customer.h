#ifndef Customer_H
#define Customer_H
#include "Loan.h"
#include"Account.h"
#include"CreditCard.h"



// Node
typedef struct node {
    void* key;
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
void createCreditCard(CreditCard* creditCard);
void showCreditCard(const CreditCard* creditCard);
void createCustomer(Customer* customer);
void showCustomer(const Customer* customer);
void freeCustomer(Customer* customer);
void init(Customer* customer);
void addLoan(Customer* customer,const Loan* loan);
void printList(const Customer customer);
void freeLoan(Customer* c);
int isValidDate(const char* date);

#endif // !Customer_H
