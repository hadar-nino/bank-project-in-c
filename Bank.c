#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Bank.h"
#include "Branch.h"

void createBank(Bank* b) {
    int num;
    printf("creating bank \nplaese enter bank id: ");
    scanf("%d", &num);
    while (num<0){
        printf("the id number need to be positive, please enter again: ");
        scanf("%d", &num);
    }
    b->branchCount = 0;
    b->branches = NULL;
    b->bankID = num;
    b->sort = 0;
    initlLInkedList(b);
}

void initlLInkedList(Bank*bank) {
    bank->branchesID = (NODE1*)malloc(sizeof(NODE1));
    bank->branchesID->next = NULL;
    bank->branchesID->prev = NULL;
}

void createNewBranch(Bank* bank, Branch* branch) {
    Branch* temp = NULL;
    temp = (Branch*)realloc(bank->branches, (bank->branchCount + 1) * sizeof(Branch));

    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return; 
    }

    bank->branches = temp;
    branch->branchID = bank->branchCount;
    bank->branches[bank->branchCount] = *branch;
    bank->branchCount++;
    if (branch->branchID == 0)
        bank->branchesID->key = 0;
    else
        addNewLink(bank->branchesID, branch->branchID);
    printf("the branch added successfully\n");
}

void freeBank(Bank* bank)
{
    processArr(bank->branches, bank->branchCount, sizeof(Branch), freeBranch);
    free(bank->branches);
    bank->branches = NULL;
    freeBranchesID(bank);
}

void freeBranchesID(Bank* bank) {
    NODE1* current = bank->branchesID; 
    NODE1* next;

    while (current != NULL) {
        next = current->next; 
        free(current);        
        current = next;       
    }
    bank->branchesID->next = NULL;
    bank->branchesID->prev = NULL;
}


void addNewLink(NODE1* head, int data) {
    NODE1* tmp = NULL;
    NODE1* current = NULL;
    if (!head)
        return NULL;
    tmp = (NODE1*)malloc(sizeof(NODE1)); 

    if (tmp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    tmp->key = data;
    current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    tmp->next = NULL;
    current->next = tmp;
    tmp->prev = current;
}


void showLInkedlist(const NODE1* node) {
    NODE* current = &node;
    if (current->next == NULL)
        printf("the list is empty");
    printf("\n");
    while (current->next != NULL) {
        showLoan(current->next->key);
        current = current->next;
    }
}

void printBank(const Bank* bank) {
    printf("bank id: %d, number of braches in the bank: %d\n", bank->bankID, bank->branchCount);
    if (bank->branchCount > 0)
        processArr(bank->branches, bank->branchCount, sizeof(Branch), printBranch);
}