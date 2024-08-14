#ifndef CreditCard_H
#define CreditCard_H

typedef struct {
    char cardNumber[16];
    float creditLimit;
    float balance;
    char expiryDate[6];
} CreditCard;

#endif // !CreditCard_H


