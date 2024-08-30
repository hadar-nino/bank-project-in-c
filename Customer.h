#ifndef Customer_H
#define Customer_H
#include "Loan.h"
#include"Account.h"
#include"CreditCard.h"




typedef struct node {
    void* key;
    struct node* next;
} NODE;

typedef struct Customer {
    Account account; 
    CreditCard* creditCard;
    NODE headOfLoan; 
    int customerID;
    char name[100]; 
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
int isDateBiggerThenOldDate(const char* date, const char* date2);

#endif 
