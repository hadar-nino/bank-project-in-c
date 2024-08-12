#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Branch.h"

void addNewEmployee(Branch* branch, Employee* employee) {
    Employee* temp = NULL;
    temp = (Employee*)realloc(branch->employees, (branch->employeesCount + 1) * sizeof(Employee));
    if (temp != NULL) {
        // Update the manager's suppliers array and count
        branch->employees = temp;
        employee->employeeID = branch->employeesCount;
        branch->employees[branch->employeesCount] = *employee;
        branch->employeesCount++;
        printf("the supplier added successfully\n");
    }
    else {
        printf("Memory allocation failed\n");
    }
}

void addNewCustomer( Branch* branch,Customer* customer) {
    Customer* temp = NULL;
    temp = (Customer*)realloc(branch->customers, (branch->customerCount+ 1) * sizeof(Customer));
    if (temp != NULL) {
        // Update the manager's suppliers array and count
        branch->customers = temp;
        customer->customerID = branch->customerCount;
        branch->customers[branch->customerCount] = *customer;
        branch->customerCount++;
        printf("the supplier added successfully\n");
    }
    else {
        printf("Memory allocation failed\n");
    }
}