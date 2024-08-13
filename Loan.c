#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Loan.h"
#include"Account.h"
#include"CreditCard.h"
#include"Customer.h"

showLoan(Loan *l) {
	printf("%f, %f, %s, %s\n", l->amount, l->interestRate, l->startDate, l->endDate);
}