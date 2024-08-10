#ifndef Customer_H
#define Customer_H

typedef struct Customer {
    int customerID;
    char name[100]; // Allocating enough space for the name
    char address[100];
   // Account* accounts; // Array of Account structures
    int accountCount;
    void* loans; // Linked list of loans
    int loanCount;
  //  CreditCard* creditCards; // Array of CreditCard structures
    int creditCardCount;
} Customer;

#endif // !Customer_H
