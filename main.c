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

#define displayMenu(){\
   printf("\nPlease choose an option:\n"\
"[1] Sort branches by type\n"\
"[2] Search branches by type\n"\
"[3] Load text file\n"\
"[4] Read text file\n"\
"[5] Print subtypes\n"\
"[6] Add new branch\n"\
"[7] Add new employee\n"\
"[8] Add new customer\n"\
"[9] Update customer\n"\
"[10] Load binary file of employees\n"\
"[11] Read binary file of employees\n"\
"[12] Find richest customer in bank\n"\
"[13] Find customer with most loans in bank\n"\
"[0] Exit\n");\
}

//using this function to print for free each element of a list
void processArr(const void* arr, int size, size_t typeSize, void (*process)(const void*)) {
    int i;
    for (i = 0; i < size; i++)
        process((char*)arr + i * typeSize);
}

void test(Bank* bank, Branch* branch, Employee* employee) {
    int choice = 1;

    for (int i = 0; i < 4; i++) {
        int sum;
        sprintf(branch->name, "Branch %d", i + 1);
        createNewBranch(bank, branch);

        for (int j = 0; j < 5; j++) {
            //            CreditCard creditCard = { "dk",12,44,"ff" };            
            Account account = { 0,NULL,0 };
            Customer customer = { account };
            customer.creditCard = (CreditCard*)malloc(sizeof(CreditCard));
            sprintf(customer.name, "Customer %d-%d", i + 1, j + 1);
            sprintf(customer.address, "Address %d-%d", i + 1, j + 1);
            customer.loanCount = 0;
            addCustomerToBranch(&bank->branches[i], &customer);
            init(&bank->branches[i].customers[j]); // Initialize the linked list head for loans

            // Assign a different balance to each customer
            sum = (i + 1) * 1000 + (j + 1) * 100;
            bank->branches[i].customers[j].account.balance = sum;
            bank->branches[i].customers[j].creditCard->balance = sum * 7;
            bank->branches[i].customers[j].creditCard->creditLimit = sum / 2;
            bank->branches[i].customers[j].account.transactionCount = 0;
            bank->branches[i].customers[j].account.transactions = NULL;
            sprintf(customer.creditCard->expiryDate, "%d", j);
            sprintf(customer.creditCard->cardNumber, "Customer %d-%d", i + 1, j + 1);
        }
    }
    printBank(bank);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


// Function prototypes
void sortBranchByType(Bank* bank, int type) {
    if (type == 0) {
        qsort(bank, bank->branchCount, sizeof(type), compareEmployeesCount);
        bank->sort = 1;
        printf("branches sorted");
    }
    else {
        qsort(bank, bank->branchCount, sizeof(type), compareCustomerCount);
        bank->sort = 2;
        printf("branches sorted");
    }
}

void searchBranchByType(Bank* bank, int type) {
    if ((type + 1) != bank->sort) {
        printf("the branches arent sorted by this type");
        return;
    }
    else {
        if (type == 0) {
            printf("enter the number if employees you want to find by");
            int numOf = 0;
            scanf("%d", numOf);
            printBranch((Branch*)bsearch(numOf, bank->branches, bank->branchCount, sizeof(Branch), compareEmployeesCount));
        }
        if (type == 1) {
            printf("enter the number if customer you want to find by");
            int numOf = 0;
            scanf("%d", numOf);
            printBranch((Branch*)bsearch(numOf, bank->branches, bank->branchCount, sizeof(Branch), compareCustomerCount));
        }
    }

}

void loadTextFile(Bank* bank) {
    FILE* file = fopen("bankInfo.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%d %d %d %d\n", bank->bankID, bank->branchCount, bank->numberOfEmployee, bank->sort);

    fprintf(file, "\n");

    // Write branch and employee data to text file
    for (int i = 0; i < bank->branchCount; i++) {
        Branch* branch = &bank->branches[i];
        fprintf(file, "%d %d %s\n", branch->customerCount, branch->employeesCount, branch->name);

        for (int j = 0; j < branch->customerCount; j++) {
            Customer* customer = &branch->customers[j];
            fprintf(file, "%s \n", customer->name);
            fprintf(file, "%d %d %s\n", customer->customerID, customer->loanCount, customer->address);

            fprintf(file, "%f %d\n", customer->account.balance, customer->account.transactionCount);
            for (int k = 0; k < customer->account.transactionCount; k++) {
                Transaction* transaction = customer->account.transactions[k];
                fprintf(file, "%f %s\n", transaction->amount, transaction->date);
            }
            fprintf(file, "%s\n", customer->creditCard->cardNumber);
            fprintf(file, "%f %f %s\n", customer->creditCard->creditLimit, customer->creditCard->balance, customer->creditCard->expiryDate);

            NODE* loanNode = customer->headOfLoan.next;
            while (loanNode) {
                Loan* loan = (Loan*)loanNode->key;
                fprintf(file, "%f %f %s %s\n", loan->amount, loan->interestRate, loan->startDate, loan->endDate);
                loanNode = loanNode->next;
            }
        }
    }


    fclose(file);
    FILE* empFile = fopen("employees.bin", "wb");
    if (!empFile) {
        printf("Error opening employee file for writing.\n");
        return;
    }

    for (int i = 0; i < bank->branchCount; i++) {
        Branch* branch = &bank->branches[i];
        fwrite(branch->employees, sizeof(Employee), branch->employeesCount, empFile);
    }

    fclose(empFile);
}


void readTextFile(Bank* bank) {
    FILE* file = fopen("bankInfo.txt", "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    fscanf(file, "%d %d %d %d\n", &bank->bankID, &bank->branchCount, &bank->numberOfEmployee, &bank->sort);

    int numOfBranch = bank->branchCount;
    bank->branchCount = 0;
    initlLInkedList(&bank->branchesID);
    Branch* branch = NULL;
    for (int i = 0; i < numOfBranch; i++) {
        branch = (Branch*)malloc(sizeof(Branch));
        if (branch != NULL) {
            fscanf(file, "%d %d\n", &branch->customerCount, &branch->employeesCount);
            fgets(branch->name, sizeof(branch->name), file);
            branch->name[strcspn(branch->name, "\n")] = '\0';
            if (branch->employeesCount > 0) {
                branch->employees = (Employee*)malloc(branch->employeesCount * sizeof(Employee));

                // Read employees data from binary file if there are employees
                FILE* empFile = fopen("employees.bin", "rb");
                if (!empFile) {
                    printf("Error opening employee file for reading.\n");
                    return;
                }

                // Read employees data for this branch
                fread(branch->employees, sizeof(Employee), branch->employeesCount, empFile);
                fclose(empFile);
            }
            else {
                branch->employees = NULL;
            }
            branch->customers = (Customer*)malloc(branch->customerCount * sizeof(Customer));

            for (int j = 0; j < branch->customerCount; j++) {
                if ((branch->customerCount - 1) == j && i == (numOfBranch - 1))
                {
                    printf("pp");
                }
                Customer* customer = &branch->customers[j];

                // Read the customer's name, including spaces
                fgets(customer->name, sizeof(customer->name), file);
                customer->name[strcspn(customer->name, "\n")] = '\0'; // Remove the newline character

                // Read the customer's ID and loan count
                fscanf(file, "%d %d\n", &customer->customerID, &customer->loanCount);

                // Read the customer's address, including spaces
                fgets(customer->address, sizeof(customer->address), file);
                customer->address[strcspn(customer->address, "\n")] = '\0'; // Remove the newline character

                // Read the customer's account balance and transaction count
                fscanf(file, "%f %d\n", &customer->account.balance, &customer->account.transactionCount);

                // Allocate memory for the transactions array
                customer->account.transactions = NULL;
                if (customer->account.transactionCount > 0) {
                    customer->account.transactions = (Transaction**)malloc(customer->account.transactionCount * sizeof(Transaction*));
                    if (customer->account.transactions == NULL) {
                        printf("Memory allocation failed for transactions.\n");
                        return; // Handle error or exit the function
                    }

                    // Read each transaction's date and amount
                    for (int k = 0; k < customer->account.transactionCount; k++) {
                        customer->account.transactions[k] = (Transaction*)malloc(sizeof(Transaction));
                        if (customer->account.transactions[k] == NULL) {
                            printf("Memory allocation failed for transaction %d.\n", k);
                            return; // Handle error or exit the function
                        }
                        fscanf(file, "%s %f\n", customer->account.transactions[k]->date, &customer->account.transactions[k]->amount);
                    }
                }

                // Allocate memory for the customer's credit card and read the details
                customer->creditCard = (CreditCard*)malloc(sizeof(CreditCard));
                if (customer->creditCard == NULL) {
                    printf("Memory allocation failed for credit card.\n");
                    return; // Handle error or exit the function
                }
                fgets(customer->creditCard->cardNumber, sizeof(customer->creditCard->cardNumber), file);
                customer->creditCard->cardNumber[strcspn(customer->creditCard->cardNumber, "\n")] = '\0';

                fscanf(file, "%f %f %s\n", &customer->creditCard->creditLimit, &customer->creditCard->balance, customer->creditCard->expiryDate);


                init(customer);
                for (int l = 0; l < customer->loanCount; l++) {
                    Loan* loan = (Loan*)malloc(sizeof(Loan));
                    fscanf(file, "%f %f %s %s\n", &loan->amount, &loan->interestRate, loan->startDate, loan->endDate);
                    addLoan(customer, loan);
                }
            }
            createNewBranch(bank, branch);
        }
        else
            printf("Memory allocation for branch failed.\n");
    }
    fclose(file);
}

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
    customer->creditCard = (CreditCard*)malloc(sizeof(CreditCard));
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
    if (index == -1) {
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



int main() {
    Account account = { 0,NULL,0 };
    int choice = 1;
    Bank bank = { 111,NULL,0,0 };
    Branch branch = { 0,"",NULL,NULL,0,0 };
    Customer customer = { account };
    Employee employee;
    int type = 0;
    initlLInkedList(&bank.branchesID);
  //  test(&bank, &branch, &employee);
    while (choice != 0) {
        displayMenu();    // Display the menu
        printf("\nEnter your choice: ");
        scanf("%d", &choice);    // Get user choice
        clearInputBuffer();
        switch (choice) {

        case 1:
            // Call function to sort branches by type
            printf("please enter the type you want to sort by:\n[1] for employee count\n[2] for customer count\n");
            scanf("%d", &type);
            type--;
            sortBranchByType(&branch, type);
            break;
        case 2:
            // Call function to search branches by type
            printf("please enter the type you want to search bye by:\n[1] for employee count\n[2] for customer count\n");
            scanf("%d", &type);
            type--;
            searchBranchByType(&branch, type);
            break;
        case 3:
            // Call function to load a text file
            loadTextFile(&bank);
            break;
        case 4:
            // Call function to read a text file
            readTextFile(&bank);
            printBank(&bank);
            break;
            /*case 5:
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
            addNewCustomer(&bank, &customer);
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
    printBank(&bank);

    freeBank(&bank);
    return 0;
}