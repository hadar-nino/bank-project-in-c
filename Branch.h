#ifndef Branch_H
#define Branch_H

#include"Customer.h"
#include"Employee.h"

typedef struct Branch {
    int branchID;
    char name[50];
    Employee* employees;
    Customer* customers;
    int customerCount;
    int employeesCount;
} Branch;
void freeBranch(Branch* branch);
void addEmployeeToBranch(Branch* branch, Employee* employee);

void addCustomerToBranch(Branch* branch, Customer* customer);

int compareEmployeesCount(Branch* branch1,Branch* branch2);

int compareCustomerCount(Branch* branch1, Branch* branch2);

void printBranch(Branch* branch);
#endif // Branch_H
