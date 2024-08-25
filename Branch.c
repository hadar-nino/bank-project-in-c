#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Branch.h"

void freeBranch(Branch* branch)
{
    processArr(branch->customers,branch->customerCount,sizeof(Customer), freeCustomer);
    free(branch->customers);
    branch->customers = NULL;
    free(branch->employees);
    branch->employees = NULL;
}

void addEmployeeToBranch(Branch* branch, Employee* employee) {
    Employee* temp = NULL;
    temp = (Employee*)realloc(branch->employees, (branch->employeesCount + 1) * sizeof(Employee));

    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    branch->employees = temp;
    employee->employeeID = branch->employeesCount;
    employee->branchID = branch->branchID;
    branch->employees[branch->employeesCount] = *employee;
    branch->employeesCount++;
    printf("the employee added successfully\n");
}

void addCustomerToBranch(Branch* branch, Customer* customer) {
    Customer* temp = NULL;
    temp = (Customer*)realloc(branch->customers, (branch->customerCount + 1) * sizeof(Customer));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    branch->customers = temp;
    customer->customerID = branch->customerCount;
    branch->customers[branch->customerCount] = *customer;
    branch->customerCount++;
    printf("the customer added successfully\n");
}

int compareEmployeesCount(const void* a, const void* b) {
    const Branch* branch1 = (const Branch*)a;
    const Branch* branch2 = (const Branch*)b;
    return branch1->employeesCount - branch2->employeesCount;
}
int compareCustomerCount(const void* a, const void* b) {
    const Branch* branch1 = (const Branch*)a;
    const Branch* branch2 = (const Branch*)b;
    return branch1->customerCount - branch2->customerCount;
}
void printBranch(const Branch* branch) {
    if (branch == NULL) {
        printf("Branch is NULL.\n");
        return;
    }

    printf("Branch Name: %s\n", branch->name);
    printf("Branch ID: %d\n", branch->branchID);
    printf("Customer Count: %d\n", branch->customerCount);

    if (branch->customerCount > 0) {
        printf("Customers:\n");
        processArr(branch->customers, branch->customerCount, sizeof(Customer), showCustomer);
    }
    else
        printf("No customers in this branch.\n");


    printf("Employee Count: %d\n", branch->employeesCount);

    if (branch->employeesCount > 0) {
        printf("Employees:\n");
        processArr(branch->employees, branch->employeesCount, sizeof(Employee), showEmployee);
    }
    else
        printf("No employees in this branch.\n");

}