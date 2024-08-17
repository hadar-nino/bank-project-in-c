#ifndef Bank_H
#define Bank_H

#include "Branch.h"
#include"Branch.h"

typedef struct node1
{
    int key;
    struct node1* next;
    struct node1* prev;
}NODE1;

typedef struct {
    int bankID;
    Branch* branches; // Array of Branch structures
    int branchCount;
    NODE1 branchesID;
    int numberOfEmployee;
    int sort;
} Bank;

void showLInkedlist(NODE1* node);
void addNewLink(NODE1* node, int id);
void initlLInkedList(NODE1* node);
void createBank(Bank* b);
void createNewBranch(Bank* bank, Branch* branch);
void freeBank(Bank* bank);
void freeBranchesID(Bank* bank);
#endif // Bank_H