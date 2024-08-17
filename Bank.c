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
    printf("plaese enter bank id: ");
    scanf("%d", &num);
    while (num<0){
        printf("the id number need to be positive, please enter again: ");
        scanf("%d", &num);
    }
    b->bankID = num;
    b->sort = 0;
}

void initlLInkedList(NODE1* node) {
    node->next = NULL;
    node->prev = NULL;
}

void createNewBranch(Bank* bank, Branch* branch) {
    Branch* temp = NULL;
    temp = (Branch*)realloc(bank->branches, (bank->branchCount + 1) * sizeof(Branch));
    if (temp != NULL) {
        bank->branches = temp;
        branch->branchID = bank->branchCount;
        bank->branches[bank->branchCount] = *branch;
        bank->branchCount++;
        addNewLink(&bank->branchesID, branch->branchID);
        printf("the branch added successfully\n");
    }
    else {
        printf("Memory allocation failed\n");
    }
}

void freeBank(Bank* bank)
{
    for (int i = 0; i < bank->branchCount; i++)
    {
        freeBranch(&bank->branches[i]);
    }
    free(bank->branches);
    bank->branches = NULL;
    freeBranchesID(bank);
}

void freeBranchesID(Bank* bank) {
    NODE1* current = bank->branchesID.next; // Start from the first node
    NODE1* next;

    while (current != NULL) {
        next = current->next; // Save the next node
        free(current);        // Free the node itself  
        current = next;       // Move to the next node
    }
    bank->branchesID.next = NULL;
    bank->branchesID.prev = NULL;
}


void addNewLink(NODE1* head, int data) {
    NODE1* tmp;
    NODE1* current;
    if (!head)
        return NULL;
    tmp = (NODE1*)malloc(sizeof(NODE1)); // new node
    if (tmp != NULL) {
        tmp->key = data;
        current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Insert the new node
        tmp->next = NULL;
        current->next = tmp;
        tmp->prev = current;
    }
}


void showLInkedlist(NODE1* node) {
    NODE* current = &node;
    if (current->next == NULL)
        printf("the list is empty");
    printf("\n");
    while (current->next != NULL) {
        showLoan(current->next->key);
        current = current->next;
    }
}