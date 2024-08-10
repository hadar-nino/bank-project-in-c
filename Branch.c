#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Branch.h"



void addNewCustomer( Branch* branch,Customer* customer) {
    Customer* temp = NULL;
    temp = (Customer*)realloc(branch->customers, (branch->customerCount+ 1) * sizeof(Customer));
    if (temp != NULL) {
        // Update the manager's suppliers array and count
        branch->customers = temp;
        branch->customers[branch->customerCount] = *customer;
        branch->customerCount++;
        printf("the supplier added successfully\n");
    }
    else {
        printf("Memory allocation failed\n");
    }
}