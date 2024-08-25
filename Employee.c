#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
#include "Employee.h"

void createEmployee(Employee* employee) {
    printf("enter name (the name will be 50 words): ");
    fgets(employee->name, 50, stdin);
    employee->name[strcspn(employee->name, "\n")] = '\0';
    printf("enter position: (the position: will be 50 words): ");
    fgets(employee->position, 50, stdin);
    employee->position[strcspn(employee->position, "\n")] = '\0';
}

void showEmployee(const Employee* employee) {
    printf("employee id: %d, employee name: %s, Branch id: %d, employee position: %s\n\n", employee->employeeID, employee->name, employee->branchID, employee->position);
}