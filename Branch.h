#ifndef Branch_H
#define Branch_H

#include"Customer.h"
#include"Employee.h"

typedef struct Branch {
    int branchID;
    char* name;
    Employee* employees;
    Customer* customers;
    int customerCount;
    int employeesCount;
} Branch;
void freeBranch(Branch* branch);
void addEmployeeToBranch(Branch* branch, Employee* employee);

void addCustomerToBranch(Branch* branch, Customer* customer);

int compareEmployeesCount(const void* a, const void* b);

int compareCustomerCount(const void* a, const void* b);
int compareIdCount(const void* a, const void* b);


void printBranch(const Branch* branch);
#endif // Branch_H
