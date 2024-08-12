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

void createNewEmployee(Branch* branch, Employee* employee);

void addNewCustomer(Branch* branch, Customer* customer);

#endif // Branch_H
