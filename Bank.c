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

void addNewLink(NODE1** head, int data) {
    NODE1* lst, * current, * pNode;

     NODE1* newNode=(NODE1*)malloc(sizeof(NODE1));
     if (newNode == NULL) {
         printf("Memory allocation failed\n");
         return;
     }
     newNode->key = data;
    // assign NULL to next of newNode
    newNode->next = NULL;

     NODE1* temp = *head;    // store the head node temporarily (for later use)

    if (*head == NULL) {    // if the linked list is empty, make the newNode as head node
        newNode->prev = NULL;
        *head = newNode;
        return;
    }
    // if the linked list is not empty, traverse to the end of the linked list
    while (temp->next != NULL)
        temp = temp->next;
    // now, the last node of the linked list is temp

    temp->next = newNode;    // point the next of the last node (temp) to newNode.
    newNode->prev = temp;    // assign prev of newNode to temp
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