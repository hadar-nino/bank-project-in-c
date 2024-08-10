#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Bank.h"
#include "Branch.h"


void addNewBranch(Bank* bank, Branch* branch) {
    Branch* temp = NULL;
    temp = (Branch*)realloc(bank->branches, (bank->branchCount + 1) * sizeof(Branch));
    if (temp != NULL) {
        // Update the manager's suppliers array and count
        bank->branches = temp;
        bank->branches[bank->branchCount] = *branch;
        bank->branchCount++;
        printf("the supplier added successfully\n");
    }
    else {
        printf("Memory allocation failed\n");
    }
}

