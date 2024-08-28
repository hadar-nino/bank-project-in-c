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
"[13] Fire employee\n"\
"[0] Exit\n");\
}

/**
 * processArr
 * Input:
 *  - arr: Pointer to an array of any data type
 *  - size: Number of elements in the array
 *  - typeSize: Size of each element in bytes
 *  - process: Function pointer to a function that processes each element
 * Output: None
 * Description:
 *  Iterates over an array and applies a processing function to each element.
 */

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

/**
 * clearInputBuffer
 * Input: None
 * Output: None
 * Description:
 *  Clears the input buffer to remove any remaining characters after user input.
 */

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * sortBranchByType
 * Input: 
 *  - bank: Pointer to a Bank structure
 *  - type: Integer indicating sort type (0 for employee count, 1 for customer count)
 * Output: None
 * Description: 
 *  Sorts the branches of a bank based on the specified type, either by employee count or customer count.
 */

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

/**
 * searchBranchByType
 * Input:
 *  - bank: Pointer to a Bank structure
 *  - type: Integer indicating search type (0 for employee count, 1 for customer count)
 * Output: None
 * Description:
 *  Searches for a branch based on the specified type and criteria, and prints the branch details if found.
 */

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

/**
 * loadTextFile
 * Input:
 *  - bank: Pointer to a Bank structure
 * Output: None
 * Description:
 *  Writes the bank's data, including branches, customers, accounts, transactions, and employees, to a text file.
 */

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
        fprintf(file, "%d %d %d %s\n", branch->branchID, branch->customerCount, branch->employeesCount, branch->name);

        for (int j = 0; j < branch->customerCount; j++) {
            Customer* customer = &branch->customers[j];
            fprintf(file, "%s\n%d %d\n%s\n", customer->name, customer->customerID, customer->loanCount, customer->address);

            fprintf(file, "%f %d\n", customer->account.balance, customer->account.transactionCount);
            for (int k = 0; k < customer->account.transactionCount; k++) {
                Transaction* transaction = customer->account.transactions[k];
                fprintf(file, "%s\n%f\n", transaction->date, transaction->amount);
            }
            fprintf(file, "%s\n%s\n%f %f\n", customer->creditCard->cardNumber, customer->creditCard->expiryDate, customer->creditCard->creditLimit, customer->creditCard->balance);

            NODE* loanNode = customer->headOfLoan.next;
            while (loanNode) {
                Loan* loan = (Loan*)loanNode->key;
                fprintf(file, "%s\n%s\n%f %f\n", loan->startDate, loan->endDate, loan->amount, loan->interestRate);
                loanNode = loanNode->next;
            }
        }

        for (int u = 0; u < branch->employeesCount; u++)
        {
            fprintf(file, "%s\n%d %d\n%s\n", branch->employees[u].name, branch->employees[u].employeeID, branch->employees[u].branchID, branch->employees[u].position);
        }
    }
    fclose(file);
    printf("\nThe text files were loaded successfully\n");
}

/**
 * readTextFile
 * Input:
 *  - bank: Pointer to a Bank structure
 * Output: None
 * Description:
 *  Reads the bank's data from a text file and populates the bank structure with branches, customers, accounts, transactions, and employees.
 */

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
                    bank->branchesID->key = branch->branchID;
                else
                    addNewLink(bank->branchesID, branch->branchID);
                if (branch->customerCount > 0) {
                    branch->customers = (Customer*)malloc(branch->customerCount * sizeof(Customer));
                    if (branch->customers == NULL)
                    {
                        printf("Memory allocation failed.\n");
                        return; // Handle error or exit the function
                    }
                    for (int j = 0; j < branch->customerCount; j++) {
                        Customer* customer = &branch->customers[j];

                        // Read the customer's name, including spaces
                        fgets(customer->name, sizeof(customer->name), file);
                        customer->name[strcspn(customer->name, "\n")] = '\0'; // Remove the newline character

                        // Read the customer's ID and loan count
                        int loanCount;
                        fscanf(file, "%d %d\n", &customer->customerID, &loanCount);
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
                                fgets(customer->account.transactions[k]->date, sizeof(customer->account.transactions[k]->date), file);
                                customer->account.transactions[k]->date[strcspn(customer->account.transactions[k]->date, "\n")] = '\0';

                                fscanf(file, "%f\n", &customer->account.transactions[k]->amount);
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

                        fgets(customer->creditCard->expiryDate, sizeof(customer->creditCard->expiryDate), file);
                        customer->creditCard->expiryDate[strcspn(customer->creditCard->expiryDate, "\n")] = '\0';

                        fscanf(file, "%f %f\n", &customer->creditCard->creditLimit, &customer->creditCard->balance);                        

                        init(customer);
                        for (int l = 0; l < loanCount; l++) {
                            Loan* loan = (Loan*)malloc(sizeof(Loan));
                            if (loan == NULL) {
                                printf("Memory allocation failed.\n");
                                return; // Handle error or exit the function
                            }
                            fgets(loan->startDate, 1+sizeof(loan->startDate), file);
                            loan->startDate[strcspn(loan->startDate, "\n")] = '\0';

                            fgets(loan->endDate, 1 + sizeof(loan->endDate), file);
                            loan->endDate[strcspn(loan->endDate, "\n")] = '\0';

                            fscanf(file, "%f %f\n", &loan->amount, &loan->interestRate);
                            addLoan(customer, loan);
                        }
                    }
                }

                if (branch->employeesCount > 0) {
                    branch->employees = (Employee*)malloc(branch->customerCount * sizeof(Employee));
                    if (branch->employees == NULL) {
                        printf("Memory allocation failed.\n");
                        return; // Handle error or exit the function
                    }
                    for (int u = 0; u < branch->employeesCount; u++)
                    {
                        fgets(branch->employees[u].name, sizeof(branch->employees[u].name), file);
                        branch->employees[u].name[strcspn(branch->employees[u].name, "\n")] = '\0';

                        fscanf(file, "%d %d\n", &branch->employees[u].employeeID, &branch->employees[u].branchID);

                        fgets(branch->employees[u].position, sizeof(branch->employees[u].position), file);
                        branch->employees[u].position[strcspn(branch->employees[u].position, "\n")] = '\0';
                    }
                }
            }
        }
        else
            printf("Memory allocation failed.\n");
    }
    fclose(file);
    printf("\nThe text files were read successfully\n");
}

/**
 * printSubTypes
 * Input:
 *  - bank: Pointer to a Bank structure
 * Output: None
 * Description:
 *  Provides a menu for printing details of the bank, branches, employees, and customers.
 */

void printSubTypes(const Bank* bank) {
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
        case 3:            
            index = branchSelect(bank);
            if (index == -1) {
                return;
            }            
            index2 = EmployeeSelect(&bank->branches[index]);
            if (index2 == -1)
                return;
            showEmployee(&bank->branches[index].employees[index2]);
            break;
        case 4:
            index = branchSelect(bank);
            if (index == -1)
                return;
            index2 = CustomerSelect(&bank->branches[index]);
            if (index2 == -1)
                return;
            showCustomer(&bank->branches[index].customers[index2]);
            break;
        default:
            printf("bad input");
            return;
        }
    }
}

/**
 * loadBinaryFile
 * Input:
 *  - bank: Pointer to a Bank structure
 * Output: None
 * Description:
 *  Writes the bank's data, including branches, customers, and employees, to a binary file for persistent storage.
 */

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
        
        fwrite(branch->employees, sizeof(Employee), branch->employeesCount, file);


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
    printf("\nThe binary files were loaded successfully\n");
}

/* readBinaryFile
 * Input: A pointer to a Bank structure (Bank* bank).
 * Output: None (void function).
 * What the Function Does: Reads the bank, branch, customer, employee, transaction, loan, and credit card data from binary files
 * (bankInfo.bin and employees.bin) and populates the Bank structure accordingly.
 * It also dynamically allocates memory for branches, customers, employees, transactions, loans, and credit cards.
 */

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
            freeBank(bank);
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
            
            branch->employees = NULL;
            if (branch->employeesCount>0)
            {
                branch->employees = (Employee*)malloc(sizeof(Employee) * branch->employeesCount);
                if (branch->employees ==NULL)
                {
                    printf("Memory allocation failed.\n");
                    freeBank(bank);
                    return;
                }
                fread(branch->employees, sizeof(Employee), branch->employeesCount, file);
            }


            if (i == 0)
                bank->branchesID->key = branch->branchID;
            else
                addNewLink(bank->branchesID, branch->branchID);

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
    printf("\nThe binary files were read successfully\n");
}

/* addNewBranch
 * Input: A pointer to a Bank structure (Bank* bank) and a pointer to a Branch structure (Branch* branch).
 * Output: None (void function).
 * What the Function Does: Prompts the user to enter a branch name and then adds a new branch to the bank using createNewBranch function.
 */

void addNewBranch(Bank* bank, Branch* branch) {
    printf("enter name (the name will be 50 words): ");
    fgets(branch->name, 50, stdin);
    branch->name[strcspn(branch->name, "\n")] = '\0';
    createNewBranch(bank, branch);
}

/* branchSelect
 * Input: A constant pointer to a Bank structure (const Bank* bank).
 * Output: Returns the index of the selected branch, or -1 if no branches are available or input is invalid.
 * What the Function Does: Displays all branches of the bank and prompts the user to select one by entering its index.
 */


int branchSelect(const Bank* bank) {
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

/* CustomerSelect
 * Input: A constant pointer to a Branch structure (const Branch* branch).
 * Output: Returns the index of the selected customer, or -1 if no customers are available or input is invalid.
 * What the Function Does: Displays all customers in the branch and prompts the user to select one by entering its index.
 */

int CustomerSelect(const Branch* branch) {
    int index;
    if (branch->customerCount == 0) {
        printf("the branch has no customer please add first in the mnue\n");
        return-1;
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

/* addNewEmployee
 * Input: A pointer to a Bank structure (Bank* bank) and a pointer to an Employee structure (Employee* employee).
 * Output: None (void function).
 * What the Function Does: Prompts the user to select a branch and then adds a new employee to that branch.
 */

void addNewEmployee(Bank* bank, Employee* employee) {
    int index = branchSelect(bank);
    if (index == -1) {
        return;
    }
    clearInputBuffer();
    createEmployee(employee);
    addEmployeeToBranch(&bank->branches[index], employee);
}

/* addNewCustomer
 * Input: A pointer to a Bank structure (Bank* bank) and a pointer to a Customer structure (Customer* customer).
 * Output: None (void function).
 * What the Function Does: Prompts the user to select a branch and then adds a new customer to that branch.
 */

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

/* updateCustomer
  * Input: A pointer to a Bank structure (Bank* bank).
  * Output: None (void function).
  * What the Function Does: Prompts the user to select a branch and a customer, then updates the selected customer's information.
  */

void updateCustomer(Bank* bank) {
    int index = branchSelect(bank);
    if (index == -1)
        return;
    int index2 = CustomerSelect(&bank->branches[index]);
    if (index2 == -1)
        return;
    updateCustomer1(&bank->branches[index].customers[index2]);
}

/* richestCustomer
    * Input: A constant pointer to a Bank structure (const Bank* bank).
    * Output: Returns a pointer to the Customer structure of the richest customer in the bank.
    * What the Function Does: Finds and displays the customer with the highest account balance across all branches of the bank.
    */

Customer* richestCustomer(const Bank* bank) {
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

/* EmployeeSelect
 * Input: A constant pointer to a Branch structure (const Branch* branch).
 * Output: Returns the index of the selected employee, or -1 if no customers are available or input is invalid.
 * What the Function Does: Displays all employee in the branch and prompts the user to select one by entering its index.
 */

int EmployeeSelect(const Branch* branch) {
    int index;
    if (branch->employeesCount == 0) {
        printf("the branch has no employees please add first in the mnue\n");
        return-1;
    }
    printf("all employees in the branch:\n");
    for (int i = 0; i < branch->employeesCount; i++) {
        printf("[%d] employees: %s \n", i, branch->employees[i].name);
    }
    printf("\nplease chose a employees:");
    scanf("%d", &index);
    if (index<0 || index>branch->employeesCount - 1) {
        printf("bad input, return to the menu");
        return-1;
    }
    return index;
}

/**
 * Input: A pointer to a Bank structure (Bank* bank).
 * Output: None (void function).
 * What the Function Does: Allows the user to select a branch and an employee within that branch to fire.
 * It removes the selected employee from the branch's employee list by overwriting the employee's data with the last employee's data,
 * then reallocates the memory to shrink the list by one employee, effectively removing the selected employee.
 */

void fireEmployee(Bank* bank) {
    int branchIndex = branchSelect(bank);
    if (branchIndex == -1)
        return;
    int employeeIndex = EmployeeSelect(&bank->branches[branchIndex]);
    if (employeeIndex == -1)
        return;

    Branch* branch = &bank->branches[branchIndex]; // Get the selected branch

    if (branch->employeesCount==1)
    {
        free(branch->employees);
        branch->employees = NULL;
        printf("The employee has been fired and the staff list has been updated.");
        branch->employeesCount--;
        return;
    }

    Employee* employeeToFire = &branch->employees[employeeIndex]; // Get a pointer to the employee to be fired
    if (employeeIndex != branch->employeesCount - 1) {
        // Move the last employee's data to the position of the employee to be fired
        Employee* lastEmployeeInBranch = &branch->employees[branch->employeesCount - 1];
        memcpy(employeeToFire, lastEmployeeInBranch, sizeof(Employee));
    }

    branch->employeesCount--;
    // Resize the employees array
    Employee* temp = (Employee*)realloc(branch->employees, branch->employeesCount * sizeof(Employee));
    if (temp != NULL) {
        branch->employees = temp; // Update the pointer if realloc succeeds    
        printf("The employee has been fired and the staff list has been updated.");
    }
}

int main() {
    Account account = { 0,NULL,0 };
    int choice = 1;
    Bank bank;
    createBank(&bank);
    Branch branch = { 0,"",NULL,NULL,0,0 };
    Customer customer = { account };
    Employee employee;
    int type = 0;
    test(&bank, &branch);
    
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
            fireEmployee(&bank);
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