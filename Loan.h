#ifndef Loan_H
#define Loan_H

typedef struct {
    float amount;
    float interestRate;
    char startDate[11];
    char endDate[11];
} Loan;

showLoan(const Loan* l);

#endif 
