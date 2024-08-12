#ifndef Bank_H
#define Bank_H

#include "Branch.h"
#include"Branch.h"


typedef struct {
    int bankID;
    Branch* branches; // Array of Branch structures
    int branchCount;
   // int* branchID; // Linked double list of branch IDs (implementation assumed elsewhere)
    int numberOfEmployee;
} Bank;

void createBank(Bank* b);
void createNewBranch(Bank* bank, Branch* branch);
#endif // Bank_H