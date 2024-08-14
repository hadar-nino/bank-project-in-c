#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
#include "Bank.h"
#include "Branch.h"
#include "Loan.h"
#include "Customer.h"
#include "Branch.h"
#include "Employee.h"
#include "CreditCard.h"
#include"Account.h"

#include "Bank.h"

void test(Bank* bank) {
    bank->branchCount = 4;
    bank->branches = (Branch*)malloc(bank->branchCount * sizeof(Branch));

    for (int i = 0; i < bank->branchCount; i++) {
        bank->branches[i].branchID = i + 1;
        sprintf(bank->branches[i].name, "Branch %d", i + 1);
        bank->branches[i].customerCount = 5;
        bank->branches[i].customers = (Customer*)malloc(bank->branches[i].customerCount * sizeof(Customer));

        for (int j = 0; j < bank->branches[i].customerCount; j++) {
            bank->branches[i].customers[j].customerID = j + 1;
            sprintf(bank->branches[i].customers[j].name, "Customer %d-%d", i + 1, j + 1);
            sprintf(bank->branches[i].customers[j].address, "Address %d-%d", i + 1, j + 1);
            bank->branches[i].customers[j].loanCount = 0;
            init(&bank->branches[i].customers[j]); // Initialize the linked list head for loans

            // Assign a different balance to each customer
            bank->branches[i].customers[j].account.balance = (i + 1) * 1000 + (j + 1) * 100;
            bank->branches[i].customers[j].account.transactionCount = 0;
            bank->branches[i].customers[j].account.transactions = NULL;
        }
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// Function prototypes
void sortBranchByType(Branch* branch, int type);

void searchBranchByType(Branch* branch, int type);

void loadTextFile(Bank* bank);

void readTextFile(Bank* bank);

void printSubTypes();


void loadBinaryFileOfEmploye(Bank* bank);

void readBinaryFileOfEmploye(Bank* bank);

void addNewBranch(Bank* bank, Branch* branch) {
    printf("enter name (the name will be 50 words): ");
    fgets(branch->name, 50, stdin);
    branch->name[strcspn(branch->name, "\n")] = '\0';
    createNewBranch(bank, branch);
}

int branchSelect(Bank* bank) {
    int index;
    if (bank->branchCount == 0) {
        printf("the bank has no branches please add first in the menu\n");
        return -1;
    }
    printf("all bank branches:\n");
    for (int i = 0; i < bank->branchCount; i++) {
        printf("[%d] branch: %s \n", i, bank->branches[i].name);
    }
    printf("\nplease choose a branch: ");
    scanf("%d", &index);
    if (index<0 || index>bank->branchCount - 1) {
        printf("bad input, return to the menu\n");
        return -1;
    }
    return index;
}

void addNewEmployee(Bank* bank, Employee* employee) {
    int index = branchSelect(bank);
    if (index == -1) {
        return;
    }
    clearInputBuffer();
    createEmployee(employee);
    addEmployeeToBranch(&bank->branches[index], employee);
}

void addNewCustomer(Bank* bank, Customer* customer) {
    int index = branchSelect(bank);
    if (index == -1) {
        return;
    }
    clearInputBuffer();
    createCustomer(customer);
    addCustomerToBranch(&bank->branches[index], customer);
}

void updateCustomer(Bank* bank) {
    int index = branchSelect(bank);
    if (index==-1){
        return;
    }
    int index2;
    if (bank->branches[index].customerCount == 0) {
        printf("the branch has no customer please add first in the mnue\n");
        return;
    }
    printf("all customers in the branch:\n");
    for (int i = 0; i < bank->branches[index].customerCount; i++) {
        printf("[%d] customer: %s \n", i, bank->branches[index].customers[i].name);
    }
    printf("\nplease chose the customer you want to update:");
    scanf("%d", &index2);
    if (index2<0 || index2>bank->branches[index].customerCount - 1) {
        printf("bad input, return to the menu");
        return;
    }      
    updateCustomer1(&bank->branches[index].customers[index2]);
}


Customer* richestCustomer(Bank* bank) {
    int indexBranch = 0, indexOfRichestCustomer = 0;
    for (int j = 0; j < bank->branchCount; j++) {
        for (int i = 0; i < bank->branches[j].customerCount; i++) {
            if (bank->branches[j].customers[i].account.balance > bank->branches[indexBranch].customers[indexOfRichestCustomer].account.balance) {
                indexOfRichestCustomer = i;
                indexBranch = j;
            }
        }
    }
    showCustomer(&bank->branches[indexBranch].customers[indexOfRichestCustomer]);
    return &bank->branches[indexBranch].customers[indexOfRichestCustomer];
}

Customer* mostLoansCustomer(Bank* bank) {
    int indexBranch = 0, mostLoansCustomer = 0;
    for (int j = 0; j < bank->branchCount; j++) {
        for (int i = 0; i < bank->branches[j].customerCount; i++) {
            if (bank->branches[j].customers[i].loanCount > bank->branches[indexBranch].customers[mostLoansCustomer].loanCount) {
                mostLoansCustomer = i;
                indexBranch = j;
            }
        }
    }
    showCustomer(&bank->branches[indexBranch].customers[mostLoansCustomer]);
    return &bank->branches[indexBranch].customers[mostLoansCustomer];
}

void displayMenu() {
    printf("\nPlease choose an option:\n"
        "[1] Sort branches by type\n"
        "[2] Search branches by type\n"
        "[3] Load text file\n"
        "[4] Read text file\n"
        "[5] Print subtypes\n"
        "[6] Add new branch\n"
        "[7] Add new employee\n"
        "[8] Add new customer\n"
        "[9] Update customer\n"
        "[10] Load binary file of employees\n"
        "[11] Read binary file of employees\n"
        "[12] Find richest customer in bank\n"
        "[13] Find customer with most loans in bank\n"
        "[0] Exit\n");
}


int main() {
    CreditCard creditCard = { "",0,0,"" };
    Account account = { 0,NULL,0 };
    int choice = 1;
    Bank bank = { 111,NULL,0,0 };
    Branch branch = { 0,"",NULL,NULL,0,0 };
    Customer customer = { account,&creditCard };
    Employee employee;

   // test(&bank);

    while (choice != 0) {
        displayMenu();    // Display the menu
        printf("\nEnter your choice: ");
        scanf("%d", &choice);    // Get user choice
        clearInputBuffer();
        switch (choice) {
            /*
            case 1:
                // Call function to sort branches by type
                // Example: sortBranchByType(&branch, type);
                break;
            case 2:
                // Call function to search branches by type
                // Example: searchBranchByType(&branch, type);
                break;
            case 3:
                // Call function to load a text file
                // Example: loadTextFile(&bank);
                break;
            case 4:
                // Call function to read a text file
                // Example: readTextFile(&bank);
                break;
            case 5:
                printSubTypes();
                break;
                */
        case 6:
            addNewBranch(&bank, &branch);
            break;

        case 7:
            addNewEmployee(&bank, &employee);
            break;
            
        case 8:
            addNewCustomer(&bank,&customer);
            break;
        case 9:
            updateCustomer(&bank);
            break;/*
        case 10:
            loadBinaryFileOfEmploye(&bank);
            break;
        case 11:
            readBinaryFileOfEmploye(&bank);
            break;
             */
        case 12:
            richestCustomer(&bank);
            break;
        case 13:
            mostLoansCustomer(&bank);
            break;
           
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }
    return 0;
}


