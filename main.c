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

void test() {

	Loan loan1 = { 1001, 5000.00f, 3.5f, "2024-01-01", "2025-01-01" };
	Loan loan2 = { 1002, 12000.00f, 4.2f, "2024-02-15", "2026-02-15" };
	Loan loan3 = { 1003, 7500.00f, 3.8f, "2024-03-10", "2025-03-10" };
	Loan loan4 = { 1004, 10000.00f, 4.0f, "2024-04-20", "2027-04-20" };
	Loan loan5 = { 1005, 15000.00f, 3.9f, "2024-05-30", "2028-05-30" };
	
	Customer c1 = { NULL,0,"","",NULL,0,NULL,0 };
	Customer* c = &c1;
	init(c);
	addLoan(c, &loan1);
	addLoan(c, &loan2);
	addLoan(c, &loan3);
	addLoan(c, &loan4);
	addLoan(c, &loan5);
	printList(c1);
}

int main() {
	Bank bank = { 0,NULL,0,0 };
	int s;

	test();

	free(bank.branches);
	printf("fix");
	return 0;
}