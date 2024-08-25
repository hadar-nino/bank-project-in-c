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
"[10] Load binary file\n"\
"[11] Read binary file\n"\
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

void test(Bank* bank, Branch* branch) {
    int choice = 1;

    for (int i = 0; i < 4; i++) {
        int sum;
        int count = 0;
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

            Employee employee;
            sprintf(employee.name, "employee %d-%d", i + 1, j + 1);
            sprintf(employee.position, "position %d-%d", i + 1, j + 1);
            employee.branchID = i;
            employee.employeeID = count++;
            addEmployeeToBranch(&bank->branches[i], &employee);
        }
    }
    printBank(bank);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void sortBranchByType(Bank* bank, int type) {
    if (type == 0) {
        qsort(bank->branches, bank->branchCount, sizeof(Branch), compareEmployeesCount);
        bank->sort = 1;
        printf("Branches sorted by employee count.\n");
    }
    else {
        qsort(bank->branches, bank->branchCount, sizeof(Branch), compareCustomerCount);
        bank->sort = 2;
        printf("Branches sorted by customer count.\n");
    }
}

void searchBranchByType(Bank* bank, int type) {
    if ((type + 1) != bank->sort) {
        printf("The branches aren't sorted by this type.\n");
        return;
    }
    else {
        int numOf = 0;
        if (type == 0) {
            printf("Enter the number of employees you want to find by: ");
            scanf("%d", &numOf);
            Branch key = { .employeesCount = numOf };
            Branch* result = (Branch*)bsearch(&key, bank->branches, bank->branchCount, sizeof(Branch), compareEmployeesCount);
            if (result != NULL) {
                printBranch(result);
            }
            else {
                printf("Branch not found.\n");
            }
        }
        else if (type == 1) {
            printf("Enter the number of customers you want to find by: ");
            scanf("%d", &numOf);
            Branch key = { .customerCount = numOf };
            Branch* result = (Branch*)bsearch(&key, bank->branches, bank->branchCount, sizeof(Branch), compareCustomerCount);
            if (result != NULL) {
                printBranch(result);
            }
            else {
                printf("Branch not found.\n");
            }
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

    for (int i = 0; i < bank->branchCount; i++) {
        Branch* branch = &bank->branches[i];
        fprintf(file, "%d %d %d %s\n",branch->branchID, branch->customerCount, branch->employeesCount, branch->name);

        for (int j = 0; j < branch->customerCount; j++) {
            Customer* customer = &branch->customers[j];            
            fprintf(file, "%s\n%d %d %s\n", customer->name, customer->customerID, customer->loanCount, customer->address);

            fprintf(file, "%f %d\n", customer->account.balance, customer->account.transactionCount);
            for (int k = 0; k < customer->account.transactionCount; k++) {
                Transaction* transaction = customer->account.transactions[k];
                fprintf(file, "%f %s\n", transaction->amount, transaction->date);
            }
            fprintf(file, "%s\n%f %f %s\n", customer->creditCard->cardNumber, customer->creditCard->creditLimit, customer->creditCard->balance, customer->creditCard->expiryDate);

            NODE* loanNode = customer->headOfLoan.next;
            while (loanNode) {
                Loan* loan = (Loan*)loanNode->key;
                fprintf(file, "%s\n%f %f %s\n", loan->startDate, loan->amount, loan->interestRate, loan->endDate);
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
    printf("\nThe files were loaded successfully\n");
}


void readTextFile(Bank* bank) {
    FILE* file = fopen("bankInfo.txt", "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    fscanf(file, "%d %d %d %d\n", &bank->bankID, &bank->branchCount, &bank->numberOfEmployee, &bank->sort);
    bank->branches = NULL;
    if (bank->branchCount > 0)
    {
        bank->branches = (Branch*)malloc(bank->branchCount * sizeof(Branch));
        if (bank->branches != NULL)
        {
            initlLInkedList(&bank->branchesID);
            for (int i = 0; i < bank->branchCount; i++) {
                Branch* branch = &bank->branches[i];
                fscanf(file, "%d %d %d\n", &branch->branchID, &branch->customerCount, &branch->employeesCount);
                fgets(branch->name, sizeof(branch->name), file);
                branch->name[strcspn(branch->name, "\n")] = '\0';
                if (i == 0)
                    bank->branchesID.key = branch->branchID;
                else
                    addNewLink(&bank->branchesID, branch->branchID);

                branch->employees = NULL;
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

                branch->customers = (Customer*)malloc(branch->customerCount * sizeof(Customer));

                for (int j = 0; j < branch->customerCount; j++) {
                    Customer* customer = &branch->customers[j];

                    // Read the customer's name, including spaces
                    fgets(customer->name, sizeof(customer->name), file);
                    customer->name[strcspn(customer->name, "\n")] = '\0'; // Remove the newline character

                    // Read the customer's ID and loan count
                    int loanCount;
                    fscanf(file, "%d %d", &customer->customerID, &loanCount);
                    customer->loanCount = 0;

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
                    for (int l = 0; l < loanCount; l++) {
                        Loan* loan = (Loan*)malloc(sizeof(Loan));

                        fgets(loan->startDate, sizeof(loan->startDate), file);
                        loan->startDate[strcspn(loan->startDate, "\n")] = '\0';

                        fscanf(file, "%f %f", &loan->amount, &loan->interestRate, loan->startDate);
                        fgets(loan->endDate, sizeof(loan->endDate), file);
                        loan->endDate[strcspn(loan->endDate, "\n")] = '\0';

                        addLoan(customer, loan);
                    }
                }
            }
        }
        else
            printf("Memory allocation failed.\n");
    }
    fclose(file);
    printf("\nThe files were read successfully\n");
}

void printSubTypes(Bank* bank) {
    int choice = 1, index, index2;
    while (choice != 0)
    {
        printf("\n[1] print bank\n"
            "[2] print branch\n"
            "[3] print employee\n"
            "[4] print customer\n"
            "[0] exit");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);    // Get user choice
        clearInputBuffer();
        switch (choice) {
        case 1:
            printBank(bank);
            break;
        case 2:
            index = branchSelect(bank);
            if (index == -1) {
                return;
            }
            printBranch(&bank->branches[index]);
            break;
        case 4:
            index = branchSelect(bank);
            if (index == -1)
                return;
            index2 = CustomerSelect(&bank->branches[index]);
            if (index2 == -1)
                return;
            showCustomer(&bank->branches[index].customers[index2]);
        }
    }
}


void loadBinaryFile(Bank* bank) {
    FILE* file = fopen("bankInfo.bin", "wb");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Write bank info
    fwrite(&bank->bankID, sizeof(int), 1, file);
    fwrite(&bank->branchCount, sizeof(int), 1, file);
    fwrite(&bank->numberOfEmployee, sizeof(int), 1, file);
    fwrite(&bank->sort, sizeof(int), 1, file);

    for (int i = 0; i < bank->branchCount; i++) {
        Branch* branch = &bank->branches[i];

        // Write branch info
        fwrite(&branch->branchID, sizeof(int), 1, file);
        fwrite(&branch->customerCount, sizeof(int), 1, file);
        fwrite(&branch->employeesCount, sizeof(int), 1, file);


        fwrite(branch->name, sizeof(char), 50, file); // Assuming name is a fixed-size array

        for (int j = 0; j < branch->customerCount; j++) {
            Customer* customer = &branch->customers[j];

            // Write customer info
            int nameLength = strlen(customer->name) + 1; // Include null terminator
            fwrite(&nameLength, sizeof(int), 1, file);
            fwrite(customer->name, sizeof(char), nameLength, file);
            fwrite(&customer->customerID, sizeof(int), 1, file);
            fwrite(&customer->loanCount, sizeof(int), 1, file);

            int addressLength = strlen(customer->address) + 1; // Include null terminator
            fwrite(&addressLength, sizeof(int), 1, file);
            fwrite(customer->address, sizeof(char), addressLength, file);

            // Write account info
            fwrite(&customer->account.balance, sizeof(float), 1, file);
            fwrite(&customer->account.transactionCount, sizeof(int), 1, file);

            // Write transactions
            for (int k = 0; k < customer->account.transactionCount; k++) {
                Transaction* transaction = customer->account.transactions[k];
                fwrite(&transaction->amount, sizeof(float), 1, file);

                int dateLength = strlen(transaction->date) + 1; // Include null terminator
                fwrite(&dateLength, sizeof(int), 1, file);
                fwrite(transaction->date, sizeof(char), dateLength, file);
            }

            // Write credit card info
            int cardNumberLength = strlen(customer->creditCard->cardNumber) + 1; // Include null terminator
            fwrite(&cardNumberLength, sizeof(int), 1, file);
            fwrite(customer->creditCard->cardNumber, sizeof(char), cardNumberLength, file);
            fwrite(&customer->creditCard->creditLimit, sizeof(float), 1, file);
            fwrite(&customer->creditCard->balance, sizeof(float), 1, file);

            int expiryDateLength = strlen(customer->creditCard->expiryDate) + 1; // Include null terminator
            fwrite(&expiryDateLength, sizeof(int), 1, file);
            fwrite(customer->creditCard->expiryDate, sizeof(char), expiryDateLength, file);

            // Write loans
            NODE* loanNode = customer->headOfLoan.next;
            while (loanNode) {
                Loan* loan = (Loan*)loanNode->key;

                int startDateLength = strlen(loan->startDate) + 1; // Include null terminator
                fwrite(&startDateLength, sizeof(int), 1, file);
                fwrite(loan->startDate, sizeof(char), startDateLength, file);
                fwrite(&loan->amount, sizeof(float), 1, file);
                fwrite(&loan->interestRate, sizeof(float), 1, file);

                int endDateLength = strlen(loan->endDate) + 1; // Include null terminator
                fwrite(&endDateLength, sizeof(int), 1, file);
                fwrite(loan->endDate, sizeof(char), endDateLength, file);

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

    // Write employees to binary file
    for (int i = 0; i < bank->branchCount; i++) {
        Branch* branch = &bank->branches[i];
        fwrite(branch->employees, sizeof(Employee), branch->employeesCount, empFile);
    }
    fclose(empFile);
    printf("\nThe files were loaded successfully\n");
}


void readBinaryFile(Bank* bank) {
    FILE* file = fopen("bankInfo.bin", "rb");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Read bank info
    fread(&bank->bankID, sizeof(int), 1, file);
    fread(&bank->branchCount, sizeof(int), 1, file);
    fread(&bank->numberOfEmployee, sizeof(int), 1, file);
    fread(&bank->sort, sizeof(int), 1, file);

    bank->branches = NULL;
    if (bank->branchCount > 0) {
        bank->branches = (Branch*)malloc(bank->branchCount * sizeof(Branch));
        if (bank->branches == NULL) {
            printf("Memory allocation failed for branches.\n");
            fclose(file);
            return;
        }

        initlLInkedList(&bank->branchesID);  // Initialize the linked list for branch IDs

        for (int i = 0; i < bank->branchCount; i++) {
            Branch* branch = &bank->branches[i];
            fread(&branch->branchID, sizeof(int), 1, file);
            fread(&branch->customerCount, sizeof(int), 1, file);
            fread(&branch->employeesCount, sizeof(int), 1, file);
            fread(branch->name, sizeof(char), 50, file);  // Assuming name is a fixed-size array

            branch->name[strcspn(branch->name, "\n")] = '\0';  // Ensure null termination

            if (i == 0)
                bank->branchesID.key = branch->branchID;
            else
                addNewLink(&bank->branchesID, branch->branchID);

            branch->employees = NULL;
            if (branch->employeesCount > 0) {
                branch->employees = (Employee*)malloc(branch->employeesCount * sizeof(Employee));
                if (branch->employees == NULL) {
                    printf("Memory allocation failed for employees.\n");
                    fclose(file);
                    return;
                }

                // Read employees data from binary file if there are employees
                FILE* empFile = fopen("employees.bin", "rb");
                if (!empFile) {
                    printf("Error opening employee file for reading.\n");
                    fclose(file);
                    return;
                }

                fread(branch->employees, sizeof(Employee), branch->employeesCount, empFile);
                fclose(empFile);
            }

            branch->customers = (Customer*)malloc(branch->customerCount * sizeof(Customer));
            if (branch->customers == NULL) {
                printf("Memory allocation failed for customers.\n");
                fclose(file);
                return;
            }

            for (int j = 0; j < branch->customerCount; j++) {
                Customer* customer = &branch->customers[j];

                // Read customer data   
                int len;
                fread(&len, sizeof(int), 1, file);
                fread(customer->name, sizeof(char), len, file);  // Assuming name is a fixed-size array
                
                int loanCount;
                fread(&customer->customerID, sizeof(int), 1, file);
                fread(&loanCount, sizeof(int), 1, file);
                fread(&len, sizeof(int), 1, file);
                fread(customer->address, sizeof(char), len, file);  // Assuming name is a fixed-size array

                            
                // Read account info
                fread(&customer->account.balance, sizeof(float), 1, file);
                fread(&customer->account.transactionCount, sizeof(int), 1, file);

                customer->account.transactions = NULL;
                if (customer->account.transactionCount > 0) {
                    customer->account.transactions = (Transaction**)malloc(customer->account.transactionCount * sizeof(Transaction*));
                    if (customer->account.transactions == NULL) {
                        printf("Memory allocation failed for transactions.\n");
                        fclose(file);
                        return;
                    }

                    // Read transactions
                    for (int k = 0; k < customer->account.transactionCount; k++) {
                        customer->account.transactions[k] = (Transaction*)malloc(sizeof(Transaction));
                        if (customer->account.transactions[k] == NULL) {
                            printf("Memory allocation failed for transaction %d.\n", k);
                            fclose(file);
                            return;
                        }
                        fread(&customer->account.transactions[k]->amount, sizeof(float), 1, file);

                        int dateLength;
                        fread(&dateLength, sizeof(int), 1, file);
                        if (customer->account.transactions[k]->date == NULL) {
                            printf("Memory allocation failed for transaction date.\n");
                            fclose(file);
                            return;
                        }
                        fread(customer->account.transactions[k]->date, sizeof(char), dateLength, file);
                        customer->account.transactions[k]->date[dateLength - 1] = '\0';  // Ensure null termination
                    }
                }

                // Read credit card info
                customer->creditCard = (CreditCard*)malloc(sizeof(CreditCard));
                if (customer->creditCard == NULL) {
                    printf("Memory allocation failed for credit card.\n");
                    fclose(file);
                    return;
                }
                
                int cardNumberLength;
                fread(&cardNumberLength, sizeof(int), 1, file);
                if (customer->creditCard->cardNumber == NULL) {
                    printf("Memory allocation failed for card number.\n");
                    fclose(file);
                    return;
                }
                fread(customer->creditCard->cardNumber, sizeof(char), cardNumberLength, file);
                customer->creditCard->cardNumber[cardNumberLength - 1] = '\0';  // Ensure null termination

                fread(&customer->creditCard->creditLimit, sizeof(float), 1, file);
                fread(&customer->creditCard->balance, sizeof(float), 1, file);

                int expiryDateLength;
                fread(&expiryDateLength, sizeof(int), 1, file);
                if (customer->creditCard->expiryDate == NULL) {
                    printf("Memory allocation failed for expiry date.\n");
                    fclose(file);
                    return;
                }
                fread(customer->creditCard->expiryDate, sizeof(char), expiryDateLength, file);
                customer->creditCard->expiryDate[expiryDateLength - 1] = '\0';  // Ensure null termination

                // Initialize loan linked list
                customer->headOfLoan.key = NULL;
                customer->headOfLoan.next = NULL;
                NODE* lastLoanNode = &customer->headOfLoan;
                customer->loanCount = 0;
                // Read loans
                for (int l = 0; l < loanCount; l++) {
                    Loan* loan = (Loan*)malloc(sizeof(Loan));
                    if (loan == NULL) {
                        printf("Memory allocation failed for loan.\n");
                        fclose(file);
                        return;
                    }

                    int startDateLength;
                    fread(&startDateLength, sizeof(int), 1, file);
                    fread(loan->startDate, sizeof(char), startDateLength, file);
                    loan->startDate[startDateLength - 1] = '\0';  // Ensure null termination

                    fread(&loan->amount, sizeof(float), 1, file);
                    fread(&loan->interestRate, sizeof(float), 1, file);

                    int endDateLength;
                    fread(&endDateLength, sizeof(int), 1, file);
                    fread(loan->endDate, sizeof(char), endDateLength, file);
                    loan->endDate[endDateLength - 1] = '\0';  // Ensure null termination

                    addLoan(customer, loan);                    
                }
            }
        }
    }

    fclose(file);

    // Reading employees binary file
    FILE* empFile = fopen("employees.bin", "rb");
    if (!empFile) {
        printf("Error opening employee file for reading.\n");
        return;
    }

    for (int i = 0; i < bank->branchCount; i++) {
        Branch* branch = &bank->branches[i];
        if (branch->employeesCount > 0) {
            fread(branch->employees, sizeof(Employee), branch->employeesCount, empFile);
        }
    }

    fclose(empFile);
    printf("\nThe binary files were read successfully\n");
}


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

int CustomerSelect(Branch* branch) {
    int index;
    if (branch->customerCount == 0) {
        printf("the branch has no customer please add first in the mnue\n");
        return;
    }
    printf("all customers in the branch:\n");
    for (int i = 0; i < branch->customerCount; i++) {
        printf("[%d] customer: %s \n", i, branch->customers[i].name);
    }
    printf("\nplease chose a customer:");
    scanf("%d", &index);
    if (index<0 || index>branch->customerCount - 1) {
        printf("bad input, return to the menu");
        return-1;
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
    if (index == -1)
        return;
    int index2 = CustomerSelect(&bank->branches[index]);
    if (index2 == -1)
        return;
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

void test1(Customer* c1, Customer* c2) {
    FILE* file = fopen("Info.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(c1->name, sizeof(char), 100, file); 
    fclose(file);

    FILE* file1 = fopen("Info.bin", "rb");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    fread(c2->name, sizeof(char), 100, file); // fix this
    fclose(file1);
}

void test2(Customer* c1, Customer* c2) {
    // Write customer data to text file
    FILE* file = fopen("Info.bin", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(c1->name, sizeof(char), 100, file); // Write name with null terminator
    fclose(file);

    // Read customer data from binary file
    FILE* file1 = fopen("Info.bin", "rb");
    if (!file1) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Read customer name from binary file
    fread(c2->name, sizeof(char), 100, file1); // Read up to 100 characters, should handle null terminator
    fclose(file1);
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
  //  test(&bank, &branch);

    Customer c = { account,NULL,{NULL,NULL},333,"bob 4 5 n obo" };//is this ok
    Customer c1 = { account,NULL,{NULL,NULL}};//is this ok
    test2(&c,&c1);
    
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
            sortBranchByType(&bank, type);
            break;
        
        case 2:
            // Call function to search branches by type
            printf("please enter the type you want to search bye by:\n[1] for employee count\n[2] for customer count\n");
            scanf("%d", &type);
            type--;
            searchBranchByType(&bank, type);
            break;
        
        case 3:
            // Call function to load a text file
            loadTextFile(&bank);
            break;
        
        case 4:
            // Call function to read a text file
            readTextFile(&bank);
            break;
        
        case 5:
            printSubTypes(&bank);
            break;
        
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
            break;
            
        case 10:
            loadBinaryFile(&bank);
            break;

        case 11:
            readBinaryFile(&bank);
            break;
             
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
    freeBank(&bank);
    return 0;
}