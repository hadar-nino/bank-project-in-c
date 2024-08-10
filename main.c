#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Bank.h"
#include "Branch.h"


int main() {
	Bank bank = { 0,NULL,0,0 };

	int s;

	free(bank.branches);
	return 0;
}