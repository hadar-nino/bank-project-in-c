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

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void test() {

    Loan loan1 = {5000.00f, 3.5f, "2024-01-01", "2025-01-01" };
    Loan loan2 = {12000.00f, 4.2f, "2024-02-15", "2026-02-15" };
    Loan loan3 = {7500.00f, 3.8f, "2024-03-10", "2025-03-10" };
    Loan loan4 = {10000.00f, 4.0f, "2024-04-20", "2027-04-20" };
    Loan loan5 = { 15000.00f, 3.9f, "2024-05-30", "2028-05-30" };

    Customer c1 = { 0,NULL,NULL,NULL,0,"","" };
    Customer* c = &c1;
    init(c);
    addLoan(c, &loan1);
    addLoan(c, &loan2);
    addLoan(c, &loan3);
    addLoan(c, &loan4);
    addLoan(c, &loan5);
    printList(c1);
}

void test2(Account* a) {
    for (int i = 0; i < 10; i++) {
        updateAccount(a);
        printf("%f", a->balance);
    }
}

// Function prototypes
void sortBranchByType(Branch* branch, int type);

void searchBranchByType(Branch* branch, int type);

void loadTextFile(Bank* bank);

void readTextFile(Bank* bank);

void printSubTypes();

void addNewBranch(Bank* bank, Branch* branch) {
    printf("enter name (the name will be 50 words): ");
    fgets(branch->name, 50, stdin);
    branch->name[strcspn(branch->name, "\n")] = '\0';
    createNewBranch(bank, branch);
}

void addNewEmployee(Bank* bank, Employee* employee) {
    int index;
    if (bank->branchCount == 0) {
        printf("the bank has no branches please add first in the mnue");
        return;
    }
    printf("all bank branches:\n");
    for (int i = 0; i < bank->branchCount; i++) {
        printf("[%d] branch: %s \n", i, bank->branches[i].name);
    }
    printf("\nplease chose the branch you want to add employee:");
    scanf("%d", &index);
    if (index<0 && index>bank->branchCount - 1) {
        printf("bad input, return to the menu");
        return;
    }
    clearInputBuffer();
    createEmployee(employee);
    addEmployeeToBranch(&bank->branches[index], employee);
}

void addNewCustomer(Bank* bank, Customer* customer) {
    int index;
    if (bank->branchCount == 0) {
        printf("the bank has no branches please add first in the mnue");
        return;
    }
    printf("all bank branches:\n");
    for (int i = 0; i < bank->branchCount; i++) {
        printf("[%d] branch: %s \n", i, bank->branches[i].name);
    }
    printf("\nplease chose the branch you want to add customer:");
    scanf("%d", &index);
    if (index<0 && index>bank->branchCount - 1) {
        printf("bad input, return to the menu");
        return;
    }
    clearInputBuffer();
    createCustomer(customer);
    addCustomerToBranch(&bank->branches[index], customer);
}

void updateCustomer(Bank* bank) {
    int index,index2;
    if (bank->branchCount == 0) {
        printf("the bank has no customer please add first in the mnue");
        return;
    }
    printf("all bank branches:\n");
    for (int i = 0; i < bank->branchCount; i++) {
        printf("[%d] branch: %s \n", i, bank->branches[i].name);
    }
    printf("\nplease chose the branch you want to update a customer:");
    scanf("%d", &index);
    if (index<0 && index>bank->branchCount - 1) {
        printf("bad input, return to the menu");
        return;
    }
    if (bank->branches[index].customerCount == 0) {
        printf("the branch has no customer please add first in the mnue");
        return;
    }
    printf("all customers in the branch:\n");
    for (int i = 0; i < bank->branches[index].customerCount; i++) {
        printf("[%d] customer: %s \n", i, bank->branches[i].customers[i].name);
    }
    printf("\nplease chose the customer you want to update:");
    scanf("%d", &index2);
    if (index2<0 && index2>bank->branches[index].customerCount - 1) {
        printf("bad input, return to the menu");
        return;
    }      
    updateCustomer(&bank->branches[index].customers[index2]);
}


void loadBinaryFileOfEmploye(Bank* bank);

void readBinaryFileOfEmploye(Bank* bank);

Customer* richestCustomer(Branch* branch);

Customer* mostLoansCustomer(Branch* branch);

void displayMenu() {
    printf("Please choose an option:\n"
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
        "[12] Find richest customer\n"
        "[13] Find customer with most loans\n"
        "[0] Exit\n");
}


int main() {
    CreditCard creditCard;
    Account account = { 0,NULL,0 };
    int choice = 1;
    Bank bank = { 111,NULL,0,0 };
    Branch branch = { 0,"",NULL,NULL,0,0 };
    Customer customer = { account,&creditCard };
    Employee employee;

    // Display the menu
    displayMenu();
    // Get user choice
    while (choice != 0) {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
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
        case 12:
            richestCustomer(&branch);
            break;
        case 13:
            mostLoansCustomer(&branch);
            break;
            */
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


