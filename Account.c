#include"Transaction.h"
#include"Account.h"

void createAccount(Account account) {
    int num;
    printf("enter customer id: ");
    scanf("%d", &num);
    while (num < 0) {
        printf("the id number need to be positive, please enter again: ");
        scanf("%d", &num);
    }
    //customer->customerID = num;
}