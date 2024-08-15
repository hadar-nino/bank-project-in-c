#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Branch.h"

void addEmployeeToBranch(Branch* branch, Employee* employee) {
    Employee* temp = NULL;
    temp = (Employee*)realloc(branch->employees, (branch->employeesCount + 1) * sizeof(Employee));
    if (temp != NULL) {
        branch->employees = temp;
        employee->employeeID = branch->employeesCount;
        employee->branchID = branch->branchID;
        branch->employees[branch->employeesCount] = *employee;
        branch->employeesCount++;
        printf("the employee added successfully\n");
    }
    else {
        printf("Memory allocation failed\n");
    }
}

void addCustomerToBranch( Branch* branch,Customer* customer) {
    Customer* temp = NULL;
    temp = (Customer*)realloc(branch->customers, (branch->customerCount+ 1) * sizeof(Customer));
    if (temp != NULL) {
        branch->customers = temp;
        customer->customerID = branch->customerCount;
        branch->customers[branch->customerCount] = *customer;
        branch->customerCount++;
        printf("the customer added successfully\n");
    }
    else {
        printf("Memory allocation failed\n");
    }
}

void showAllEmployee(Branch branch) {
    for (int i = 0; i < branch.employeesCount; i++)
    {

    }
}
int compareEmployeesCount(Branch* branch1, Branch* branch2) {
    return branch1->employeesCount - branch2->employeesCount;
}
int compareCustomerCount(Branch* branch1, Branch* branch2) {
    return branch1->customerCount - branch2->customerCount;
}
void printBranch(Branch* branch) {
    printf("branch name: %s, ", branch->name);
    printf("branch id: %d, ", branch->branchID);
    printf("branch customer count: %d, ", branch->customerCount);
    for (int i = 0; i < branch->customerCount; i++) {
        printEmploee(branch->customers[i]);
    }
    printf("branch employee count: %d, ", branch->employeesCount);
    for (int i = 0; i < branch->employeesCount; i++) {
        printEmploee(branch->employees[i]);
    }
}